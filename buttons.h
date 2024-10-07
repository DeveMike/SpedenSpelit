#ifndef BUTTONS_H
#define BUTTONS_H
#include <arduino.h> // Arduinon peruskirjasto
#include <avr/io.h> // I/O rekisterit
#include <avr/interrupt.h> // Keskeytyksiin liittyvät funktiot ja makrot

// Määritellään napinpainallusten keskeytyksiä varten käytettävät pinnit
const byte firstPin = 2; // Ensimmäinen PinChangeInterrupt D-väylällä
const byte lastPin =  5; // Viimeinen PinChangeInterrupt D-väylällä

void initButtonsAndButtonInterrupts(void); // Alustaa napit ja niiden keskeytykset
void checkStartButton(void);  // Uusi funktio pelin aloituksen tarkistamiseksi
extern bool gameStarted; // Tämä muuttuja kertoo onko peli aloitettu (true = aloitettu, false = ei aloitettu)

// Määritellään PCINT2_vect keskeytyspalvelurutiini Pin Change Interrupt keskeytykselle.
ISR(PCINT2_vect); 

#endif

