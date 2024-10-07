#ifndef SPEDENSPELIT_H
#define SPEDENSPELIT_H
#include <arduino.h> // Arduinon peruskirjasto
#include <avr/io.h> // I/O rekisterit
#include <avr/interrupt.h> // Keskeytyksiin liittyvät funktiot ja makrot
#include <avr/sleep.h> // Mikrokontrollerin lepotilat
#include <avr/power.h> // Virranhallinnan funktiot

void endGame(); // Päättää pelin ja tallentaa Highscoren
void initializeGame(); // Alustaa pelin muuttujat
void checkGame(int pressedButton); // Tarkistaa pelaajan painalluksen oikeellisuuden
void startTheGame(void); // Käynnistää pelin alusta
void checkStartCondition(void); // Täyttyvätkö pelin aloitusehdot
void handleGameLoop(void); // Pelin pääsilmukka. Hallitsee pelin etenemistä
void initializeTimer(void); // Alustaa Timer1:n ja keskeytykset
void initializeGameComponents(void); // Alustaa pelin komponentit kuten napit ja LEDit

#endif
