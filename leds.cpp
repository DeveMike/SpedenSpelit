#include "leds.h" // LEDien hallinta

////////// LEDIEN PINNIT //////////
const byte ledPins[] = {A1, A2, A3, A4};  // Ledit ovat pinneissä A1-A4
byte lastLed = 255;  // Alustetaan arvolla joka ei ole minkään oikean ledin numero

////////// LEDIEN ALUSTUS //////////
void initializeLeds() {
    // Alustetaan kaikki pelissä käytettävät LEDit (4 kappaletta)
    for (byte i = 0; i < 4; i++) {
        pinMode(ledPins[i], OUTPUT);  // Määritetään jokainen LED pinni lähtötilaan
        digitalWrite(ledPins[i], LOW);  // Sammutetaan kaikki LEDit alussa
    }
}

////////// LEDIEN SYTYTYS JA SAMMUTUS //////////
void setLed(byte ledNumber) {
    // Jos jokin LED on jo päällä ja se ei ole sama kuin uusi LED niin sammutetaan se
    if (lastLed != 255 && lastLed != ledNumber) {
        digitalWrite(ledPins[lastLed], LOW);  // Sammutetaan edellinen LED
    }
    // Sytytetään uusi LED
    digitalWrite(ledPins[ledNumber], HIGH);
    lastLed = ledNumber;  // Päivitetään viimeksi sytytetty LED
}

////////// LEDIEN SAMMUTUS //////////
void clearAllLeds() {
    for (byte i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);  // Sammutetaan kaikki LEDit
    }
    lastLed = 255;  // Nollataan viimeksi sytytetty LED
}

////////// LEDIEN SYTYTYS //////////
void setAllLeds() {
    for (byte i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);  // Sytytetään kaikki LEDit
    }
}

////////// LEDIEN BINÄÄRIESITYS NUMEROILLE 0-15 //////////
void show1() {
    // Näytetään numerot 0-15 binääriesityksinä LEDeillä
    for (int i = 0; i < 16; i++) {
        for (byte j = 0; j < 4; j++) {
            if (i & (1 << j)) {
                digitalWrite(ledPins[j], HIGH);  // Sytytä LED jos bitti on 1
            } else {
                digitalWrite(ledPins[j], LOW);   // Sammuta LED jos bitti on 0
            }
        }
        delay(500);  // Pieni viive jotta binääriesitys näkyy
    }
}

////////// LEDIEN SYTYTYS NOPEUTUVASSA TAHDISSA //////////
void show2(int rounds) {
    // Sytytetään LEDit 0-3 yhä nopeammin toistuvalla tahdilla
    int delayTime = 500;  // Alussa viive on 500 ms
    for (int r = 0; r < rounds; r++) {
        for (byte i = 0; i < 4; i++) {
            clearAllLeds();  // Sammutetaan kaikki ennen uuden sytyttämistä
            digitalWrite(ledPins[i], HIGH);  // Sytytä LED 0-3
            delay(delayTime); // Odotetaan nykyisen viiveen verran
        }
        // Vähennetään viivettä joka kierroksella jolloin tahti nopeutuu
        if (delayTime > 100) {
            delayTime -= 50;  // Vähennetään viivettä jotta tahti kiihtyy
        }
    }
}
