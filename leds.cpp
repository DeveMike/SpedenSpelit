#include "leds.h"

// LEDien pinnit
const byte ledPins[] = {A1, A2, A3, A4};  // Ledit ovat pinneissä A1-A4
byte lastLed = 255;  // Alustetaan arvolla, joka ei ole mikään oikea ledin numero

void initializeLeds()
{
    for (byte i = 0; i < 4; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);  // Sammutetaan kaikki LEDit alussa
    }
}

void setLed(byte ledNumber)
{
    // Jos jokin LED on jo päällä ja se ei ole sama kuin uusi LED, sammutetaan se
    if (lastLed != 255 && lastLed != ledNumber) {
        digitalWrite(ledPins[lastLed], LOW);  // Sammutetaan edellinen LED
    }
    // Sytytetään uusi LED
    digitalWrite(ledPins[ledNumber], HIGH);
    lastLed = ledNumber;  // Päivitetään viimeksi sytytetty LED
}


void clearAllLeds()
{
    for (byte i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);  // Sammutetaan kaikki LEDit
    }
    lastLed = 255;  // Nollataan viimeksi sytytetty LED
}

void setAllLeds()
{
    for (byte i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);  // Sytytetään kaikki LEDit
    }
}


void show1()
{
    // Näytetään numerot 0-15 binääriesityksinä LEDeillä
    for (int i = 0; i < 16; i++) {
        for (byte j = 0; j < 4; j++) {
            if (i & (1 << j)) {
                digitalWrite(ledPins[j], HIGH);  // Sytytä LED, jos bitti on 1
            } else {
                digitalWrite(ledPins[j], LOW);   // Sammuta LED, jos bitti on 0
            }
        }
        delay(500);  // Pieni viive, jotta binääriesitys näkyy
    }
}

void show2(int rounds)
{
    // Sytytetään LEDit 0-3 yhä nopeammin toistuvalla tahdilla
    int delayTime = 500;  // Alussa viive on 500 ms
    for (int r = 0; r < rounds; r++) {
        for (byte i = 0; i < 4; i++) {
            clearAllLeds();  // Sammutetaan kaikki ennen uuden sytyttämistä
            digitalWrite(ledPins[i], HIGH);  // Sytytä LED 0-3
            delay(delayTime);
        }
        // Vähennetään viivettä joka kierroksella, jolloin tahtia nopeutetaan
        if (delayTime > 100) {
            delayTime -= 50;  // Vähennetään viivettä, jotta tahti kiihtyy
        }
    }
}