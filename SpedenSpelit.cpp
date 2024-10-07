#include "SpedenSpelit.h" // Pääohjelman otsikkotiedosto
#include "display.h" // Näytön hallintaan liittyvät funktiot
#include "buttons.h" // Napinpainallusten käsittely
#include "leds.h" // LEDien hallinta
#include "highscore.h" // Highscore käsittely
#include "sound.h" // Ääniefektit
#include <EEPROM.h> // EEPROM muisti tallennusta varten
#include <Arduino.h> // Arduinon peruskirjasto

extern int latchPin; // Siirtorekisterin latch pinni
extern int clockPin; // Siirtorekisterin kellopinni
extern int dataPin; // Siirtorekisterin datapinni
extern byte sevenSegDigits[]; // Taulukko 7-segmenttinäytön numeroille
extern int correctPressCount; // Oikeiden painallusten määrä
extern bool gameJustStarted; // Peli on juuri alkanut
extern bool gameStarted; // Peli on aloitettu
extern Highscore highscore; // Highscore
extern float timerFrequency; // Ajastimen taajuus
extern int currentLed; // Tällä hetkellä aktiivinen LED
extern int buttonNumber; // Tieto painetusta napista
extern int hundredMultiplier; // Sadankertaiset pisteet
extern volatile bool newTimerInterrupt; // Keskeytyksen event lippu
extern unsigned long buttonPressStartTime; // Aika jolloin nappi painettiin alas
extern bool buttonBeingHeld; // Pidetäänkö nappia pohjassa

////////// KAHDEN SEKUNNIN PAINALLUS / PELI ALKAA ///////////
const unsigned long startPressDuration = 2000;  // 2 sekuntia

////////// PELIN ALUSTUS //////////
void initializeGame() {
  correctPressCount = 0;   // Nollataan oikeiden painallusten määrä
  timerFrequency = 1.0;    // Alustetaan timerin taajuus
  gameJustStarted = true;  // Asetetaan peli juuri alkaneeksi
  currentLed = -1;         // Ei sytytettyjä LEDejä alussa
  buttonNumber = -1;       // Nollataan painallus
}

////////// KOMPONENTIEN ALUSTUS //////////
void initializeGameComponents(void) {
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    Serial.begin(9600);  // Alustetaan sarjayhteys
    initButtonsAndButtonInterrupts();  // Alustetaan napit ja keskeytykset
    initializeLeds();  // Alustetaan LEDit

    randomSeed(analogRead(A0));  // Satunnaisluvun siemen
    timerFrequency = 1.0;  // Asetetaan aloitustaajuus
    initializeTimer();  // Alustetaan Timer1 1 Hz taajuudella
}

////////// PELIN ALOITUSEHDOT //////////
void checkStartCondition(void) {
    // Määritetään vilkkumisen aikaväli
    const unsigned long blinkInterval = 800; // LED ja Highscore vilkkuvat 800 ms välein
    static unsigned long lastBlinkTime = 0; // Seurataan vilkkumista
    static bool displayOn = true; // Onko näyttö päällä vai ei

    unsigned long currentTime = millis(); // Nykyinen aika millisekunteina

    // A1 LED vilkkuu ennen pelin alkua
    if (currentTime % 800 < 250) {
        setLed(0);  // LED A1 päälle
    } else {
        clearAllLeds();  // LED A1 sammutetaan jos 250 ms on kulunut
    }

    // Vilkutetaan Highscorea näytöllä ennen pelin alkua
    if (currentTime - lastBlinkTime >= blinkInterval) {
        lastBlinkTime = currentTime; // Päivitetään vilkuntajakso
        displayOn = !displayOn;  // Päälle / pois

        if (displayOn) {
            // Näytetään highscore. Jaetaan sadat ja yksiköt
            int score = highscore.getHighscore();
            int highscoreHundreds = score / 100;
            int displayScore = score % 100;

            // Päivitetään näyttöä
            updateShiftRegister(displayScore / 10, displayScore % 10, false, highscoreHundreds);
        } else {
            // Tyhjennetään näyttö jos displayOn on false
            clearDisplay();
        }
    }

    // Tarkistetaan onko D2 nappia painettu yli 2 sekuntia pelin aloittamiseksi
    if (digitalRead(2) == LOW) {
        if (!buttonBeingHeld) {
            buttonPressStartTime = currentTime; // Painallus alkoi
            buttonBeingHeld = true; // Merkitään nappi painetuksi
            Serial.println("Nappi D2 painettu alas");
        }

        // Jos nappia pidetään pohjassa yli 2 sekuntia = peli käynnistyy
        if (currentTime - buttonPressStartTime >= startPressDuration) {
            Serial.println("Pelin käynnistys");
            clearAllLeds();  // Sammutetaan kaikki LEDit ennen pelin alkua
            clearDisplay();  // Tyhjennetään näyttö
            sound(4);        // Soitetaan aloitusääni
            delay(700);      // Odotetaan 700 ms ennen pelin alkua
            startTheGame();  // Kutsutaan startTheGame funktiota
            buttonBeingHeld = false; // Nollataan painallus
        }
    } else {
        // Jos nappi vapautetaan ennen 2 sekuntia = nollataan tila
        if (buttonBeingHeld) {
            Serial.println("Nappi D2 vapautettu ennen 2 sekuntia");
        }
        buttonBeingHeld = false; // Nollataan painallus
        buttonPressStartTime = 0; // Nollataan aika
    }
}

////////// PELIN KÄYNNISTYS //////////
void startTheGame(void) {
    initializeGame();       // Alustetaan pelin muuttujat
    initializeTimer();      // Käynnistetään Timer1 keskeytykset
    gameStarted = true;     // Asetetaan peli käynnistyneeksi
}

////////// PELIN SEURANTA JA PISTEET //////////
void checkGame(int pressedButton) {
  // Onko painettu nappi oikea
  if (pressedButton == currentLed) {
    Serial.println("Oikea nappi painettu!");
    correctPressCount++;  // Lisätään oikea painallus
    sound(6);  // Soitetaan voittoääni

    // Tarkistetaan jos pisteet ylittävät 99
    if (correctPressCount > 99) {
      correctPressCount = 0;  // Nollataan pistemäärä ylityksen jälkeen
      hundredMultiplier++;    // Lisätään sadan pisteen ylitys
    }

    // Näytetään päivitetty pistemäärä näytöllä desimaalipilkulla
    updateShiftRegister(correctPressCount / 10, correctPressCount % 10, false, hundredMultiplier);

    // Nopeutetaan peliä 10 % jokaisen kymmenennen oikean painalluksen jälkeen.
    if (correctPressCount % 10 == 0 && correctPressCount != 0) {
      timerFrequency *= 1.1;   // Taajuus nousee 10 %
      initializeTimer();       // Uusi taajuus
      Serial.print("Ajastintaajuus nousi: ");
      Serial.print(timerFrequency, 2); // Tulostetaan uusi ajastintaajuus kahden desimaalin tarkkuudella
      Serial.println(" Hz");
    }

    buttonNumber = -1;  // Nollataan painallus
  } else {
    // Jos painettiin väärää nappia, peli päättyy
    Serial.println("Väärä nappi painettu! Peli päättyy.");
    sound(0);  // Soitetaan loser-ääni
    endGame();  // Peli loppuu
  }
}

////////// PELIN PÄÄLOOP JA LED-arvonta //////////
void handleGameLoop(void) {
    // Kun peli käynnistyy ensimmäistä kertaa niin odotetaan 1 sekunti
    if (gameJustStarted) {
        delay(1000);  // 1 sekunnin viive pelin käynnistyttyä
        gameJustStarted = false;  // Tämä tehdään vain kerran pelin alussa
    }

    // Kun peli on käynnissä niin Timer1 arpoo uudet LEDit
    if (newTimerInterrupt) {
        newTimerInterrupt = false;  // Nollataan keskeytykset

        // Varmistetaan, että uusi LED on eri kuin nykyinen LED
        int newLed;
        do {
            newLed = random(0, 4);  // Arvotaan uusi LED (0-3)
        } while (newLed == currentLed);  // Toistetaan arvonta kunnes uusi LED on eri kuin nykyinen

        currentLed = newLed;  // Päivitetään nykyinen LED uudeksi arvotuksi LEDiksi
        clearAllLeds();  // Sammutetaan kaikki LEDit ennen uuden sytyttämistä
        setLed(currentLed);  // Sytytetään uusi LED

        // Näytetään pelin pisteet segmenttinäytöllä
        updateShiftRegister(correctPressCount / 10, correctPressCount % 10, false, hundredMultiplier);  // Päivitä tulos näytöllä
    }

    // Tarkistetaan onko paienttu ja onko se oikea
    if (buttonNumber >= 2 && buttonNumber <= 5) {
        int pressedButton = buttonNumber - 2;  // Muutetaan painetun napin numero vastaamaan LED-numeroa (0-3)
        checkGame(pressedButton);              // Tarkistetaan checkGame funktiolla painalluksen oikeellisuus
        buttonNumber = -1;  // Nollataan buttonNumber seuraavaa painallusta varten
    }
}

////////// THE END & HIGHSCORE //////////
void endGame() {
    // Lasketaan kokonaispisteet
    int totalScore = (hundredMultiplier * 100) + correctPressCount;

    // Tallennetaan highscore vain jos uusi tulos on parempi
    if (totalScore > highscore.getHighscore()) {
        highscore.setHighscore(totalScore);  // Päivitetään Highscore
    }

    // Nollataan pelin muuttujat seuraavaa peliä varten
    correctPressCount = 0; // Nollataan oikeiden painallusten laskuri
    hundredMultiplier = 0; // Nollataan sadan pisteen kerroin
    gameJustStarted = false; // Pelin aloitustila nollataan
    gameStarted = false; // Merkitään peli päättyneeksi

    // Sammutetaan LEDit ja näyttö
    clearAllLeds(); // Sammutetaan LEDit
    clearDisplay(); // Tyhjennetään 7-segmenttinäyttö
}

////////// TIMER1 ALUSTUKSET JA KESKEYTYKSEN KONFIGUROINTI //////////
void initializeTimer(void) {
    float frequency = timerFrequency;  // Käytetään timerin taajuutta joka on määritelty globaalina muuttujana

    cli();  // Estetään keskeytykset konfiguroinnin ajaksi
    TCCR1A = 0;  // Asetetaan Timer1 normaaliin tilaan
    TCCR1B = 0;  // Nollataan Timer1
    TCNT1 = 0;   // Nollataan Timerin laskuri

    // Lasketaan keskeytysväli
    int compareMatch = (int)(16000000 / (1024 * frequency)) - 1;

    OCR1A = compareMatch;  // Asetetaan ajastimen keskeytysarvo
    TCCR1B |= (1 << WGM12);  // CTC-tila
    TCCR1B |= (1 << CS12) | (1 << CS10);  // Asetetaan prescaler 1024

    TIMSK1 |= (1 << OCIE1A);  // Mahdollistetaan Timer1 Compare Match A keskeytys
    sei();  // Mahdollistetaan keskeytykset konfiguroinnin jälkeen
}

////////// TIMER1 COMPARE MATCH A KESKEYTYSTURIINI //////////
ISR(TIMER1_COMPA_vect) {
    newTimerInterrupt = true;  // Asetetaan keskeytyslippu - uusi ajastinkeskeytys tapahtuu
}
