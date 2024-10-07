#include <stdlib.h> // Standardi C-kirjasto
#include <EEPROM.h> // EEPROM muisti tallennusta varten
#include "display.h" // Näytön hallintaan liittyvät funktiot
#include "buttons.h" // Napinpainallusten käsittely
#include "leds.h" // LEDien hallinta
#include "SpedenSpelit.h" // Pääohjelman otsikkotiedosto
#include "highscore.h" // Highscore käsittely
#include "sound.h" // Ääniefektit

////////// VIRRANSÄÄSTÖTILA //////////
unsigned long lastActivityTime = 0;  // Aika viimeisestä toiminnosta
const unsigned long inactivityPeriod = 60000;  // odota 1 minuutti (60000 ms)
bool deviceSleeping = false;  // Onko laite lepotilassa vai ei

////////// PELIN TILAMUUTTUJAT JA KESKEYTYKSET //////////
// Volatile muuttujat keskeytysten ja ohjelman viestintään
volatile int buttonNumber = -1;           // Painetun napin tunnistus
volatile bool newTimerInterrupt = false;  // Timer keskeytys

int currentLed = -1;  // Pitää kirjaa mikä LED on tällä hetkellä sytytetty
int correctPressCount = 0;  // Oikeiden painallusten määrä
int hundredMultiplier = 0;  // Seuraa, montako kertaa pistemäärä on ylittänyt 99
float timerFrequency = 1.0;  // Timer1:n aloitustaajuus on 1 Hz
bool gameStarted = false; // Onko peli käynnissä
bool gameJustStarted = false;  // Peli on juuri aloitettu

Highscore highscore;  // Luodaan Highscore olio

////////// SEGMENTTINÄYTÖN SIIRTOREKISRERIN PINNIT //////////
int latchPin = 10;  // Kytketty ST_CP:hen 74HC595:ssä
int clockPin = 11;  // Kytketty SH_CP:hen 74HC595:ssä
int dataPin = 12;   // Kytketty DS:ään 74HC595:ssä

////////// TAULUKKO NUMEROILLE 0-9 SEGMENTTINÄYTÖLLE //////////
byte sevenSegDigits[] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01101101, // 5
  B01111101, // 6
  B00000111, // 7
  B01111111, // 8
  B01101111  // 9
};

////////// VIRRANSÄÄSTÖTILAN HALLINTA //////////
void wakeUp() {
    // Tyhjä funktio. Tämä herättää Arduinon lepotilasta
}

void sleepModeInterruptsSetup() {
    // Alustetaan keskeytys joka herättää laitteen napista D2
    attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);
}

void enterSleepMode() {
    Serial.println("Laitetaan laite lepotilaan...");

    // Sammutetaan kaikki LEDit ja näyttö
    clearAllLeds();
    clearDisplay();

    deviceSleeping = true;  // Asetetaan laite lepotilaan

    // Alustetaan sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    // Nollataan keskeytysliput
    EIFR = bit (INTF0);

    // Mahdollistetaan keskeytykset herättämään laite
    sleepModeInterruptsSetup();

    // Menään lepotilaan. Laite pysähtyy tässä kohtaa
    sleep_mode();

    // Tästä eteenpäin laite jatkaa, kun D2 on painettu ja se herää lepotilasta

    sleep_disable();  // Poistetaan lepotila käytöstä
    detachInterrupt(digitalPinToInterrupt(2));  // Poistetaan keskeytys käytöstä kun laite hereillä

    deviceSleeping = false; // Laitteen tila pois lepotilasta
    Serial.println("Laite heräsi lepotilasta.");
    lastActivityTime = millis();  // Päivitetään viimeisin aktiivisuusaika jotta lepotila voidaan laskea uudestaan tarvittaessa
}

////////// SETUP //////////
void setup() {
    initializeGameComponents(); // Alustetaan kaikki komponentit
    initializeSpeaker(); // Alustetaan kaiutin
    lastActivityTime = millis();  // Alustetaan viimeisin aktiivisuusaika
    detachInterrupt(digitalPinToInterrupt(2)); // Varmistetaan, että ei ole päällekkäisiä keskeytyksiä
    ///resetHighscore();  // Nollaa highscore (tätä käytetään vain testauksen aikana)
}

////////// LOOP //////////
void loop() {
    unsigned long currentTime = millis(); // Tallennetaan nykyinen aika millisekunteina
    // Tarkistetaan onko jotain nappia painettu
    if (digitalRead(2) == LOW || digitalRead(3) == LOW || digitalRead(4) == LOW || digitalRead(5) == LOW) {
        lastActivityTime = currentTime;  // Päivitetään viimeisin aktiivisuusaika kun nappia on painettu
    }
    
    // Tarkistetaan onko laite lepotilassa
    if (!deviceSleeping) {
        // Jos peli ei ole vielä alkanut niin tarkistetaan aloitusehdot
        if (!gameStarted) {
            checkStartCondition();
        }
        // Jos peli on käynnissä niin suoritetaan pelin pääsilmukka
        if (gameStarted) {
            handleGameLoop();
        }
        // Tarkistetaan, onko kulunut tarpeeksi aikaa viimeisestä napin painalluksesta
        if (currentTime - lastActivityTime >= inactivityPeriod) {
            enterSleepMode();  // Laitetaan laite lepotilaan
        }
    }
}

////////// TÄMÄ ON DEBUGGAUSTA VARTEN //////////
void resetHighscore() {
  // Nollataan high score tallentamalla arvo 0 EEPROM muistiin
  EEPROM.update(0, 0);             // Alempi tavu
  EEPROM.update(1, 0);             // Ylempi tavu
}
