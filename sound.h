#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h> // Arduinon peruskirjasto

void initializeSpeaker(void); // Alustaa kaiutin
void sound(uint8_t note_index); // Toistaa äänen joka vastaa annettua note_index-arvoa

#endif