#ifndef LEDS_H
#define LEDS_H
#include <arduino.h> // Arduinon peruskirjasto

void initializeLeds(); // Alusta LEDit
void setLed(byte ledNumber); // Sytytä LEDit
void clearAllLeds(void); // Sammuta LEDit
void setAllLeds(void); // Sytytä kaikki LEDit
void show1(void); // Näyttää show1 LED tilan
void show2(int); // Näyttää show2 LED tilan

#endif
