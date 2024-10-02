#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#include <stdlib.h>  // Satunnaisluvun luomiseen
#include <avr/io.h>
#include <avr/interrupt.h>

// Volatile muuttujat keskeytysten ja ohjelman viestintään
volatile int buttonNumber = -1;           // Painetun napin tunnistus
volatile bool newTimerInterrupt = false;  // Timer keskeytys
volatile int randomLed = -1;  // Keskeytyksessä arvottu LED
int currentLed = -1;  // Pitää kirjaa, mikä LED on tällä hetkellä sytytetty
int correctPressCount = 0;  // Oikeiden painallusten määrä
float timerFrequency = 1.0;  // Timer1:n aloitustaajuus on 1 Hz

extern unsigned long buttonPressStartTime;  // Tämä viittaa buttons.cpp:ssä määriteltyyn muuttujaan

void setup() {
  Serial.begin(9600);  // Alustetaan sarjayhteys
  initButtonsAndButtonInterrupts();  // Alustetaan napit ja keskeytykset
  initializeLeds();  // Alustetaan LEDit

  randomSeed(analogRead(A0));  // Satunnaisluvun siemen
  initializeTimer(1.0);  // Alustetaan Timer1 1 Hz taajuudella
}

void loop() {
  // Peli ei ole vielä alkanut, A1 vilkkuu
  if (!gameStarted) {
    // Vilkutetaan A1 LEDiä ennen pelin alkua
    if (millis() % 500 < 250) {
      setLed(0);  // A1 vastaa setLed(0)
    } else {
      clearAllLeds();  // Sammutetaan A1 välillä
    }

    // Tarkistetaan, onko nappia 2 painettu yli 2 sekuntia pelin aloittamiseksi
    checkStartButton();
  }

  // Kun peli käynnistyy, Timer1 arpoo uudet LEDit
  if (gameStarted && newTimerInterrupt) {
    newTimerInterrupt = false;  // Nollataan keskeytyksen lippu

    // Varmistetaan, että uusi LED on eri kuin nykyinen LED
    int newLed;
    do {
      newLed = random(0, 4);  // Arvotaan uusi LED (0-3)
    } while (newLed == currentLed);  // Toistetaan, kunnes uusi LED on eri kuin nykyinen

    currentLed = newLed;  // Päivitetään nykyinen LED arvotuksi LEDiksi
    clearAllLeds();  // Sammutetaan kaikki LEDit ennen uuden sytyttämistä
    setLed(currentLed);  // Sytytetään uusi LED
  }

  // Tarkistetaan, onko painallus tullut ja onko se oikea
  if (gameStarted && buttonNumber >= 2 && buttonNumber <= 5) {
    int pressedButton = buttonNumber - 2;  // Muutetaan pinniarvo vastaamaan LED-numeroa
    
    if (pressedButton == currentLed) {
      Serial.println("Oikea nappi painettu!");
      correctPressCount++;  // Lisätään oikea painallus
      
      if (correctPressCount % 10 == 0) {
        // Nopeutetaan ajastinta joka kymmenes oikea painallus
        timerFrequency *= 1.1;  // Nopeutetaan taajuutta 10 %
        initializeTimer(timerFrequency);  // Päivitetään ajastimen taajuus
        Serial.print("Ajastintaajuus nousi: ");  // Tulostetaan uusi taajuus
        Serial.print(timerFrequency, 2);  // Näytetään taajuus kahden desimaalin tarkkuudella
        Serial.println(" Hz");
      }
      
      buttonNumber = -1;  // Nollataan painallus
    } else {
      Serial.println("Väärä nappi painettu!");
    }
  }
}

void initializeTimer(float frequency) {
  // Lasketaan Timer1:n rekisteriarvot halutulle taajuudelle
  cli();  // Estetään keskeytykset konfiguroinnin ajaksi
  TCCR1A = 0;  // Timer1 normaali tila
  TCCR1B = 0;  // Nollataan Timer1
  TCNT1 = 0;   // Nollataan Timerin laskuri

  // Lasketaan keskeytysväli
  int compareMatch = (int)(16000000 / (1024 * frequency)) - 1;
  
  OCR1A = compareMatch;  // Asetetaan ajastimen keskeytysarvo
  TCCR1B |= (1 << WGM12);  // CTC-tila
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Asetetaan prescaler 1024
  
  TIMSK1 |= (1 << OCIE1A);  // Mahdollistetaan Timer1 Compare Match A keskeytys
  sei();  // Mahdollistetaan keskeytykset
}

ISR(TIMER1_COMPA_vect) {
  randomLed = random(0, 4);  // Arvotaan uusi LED (0-3)
  newTimerInterrupt = true;  // Asetetaan keskeytyslippu
}
