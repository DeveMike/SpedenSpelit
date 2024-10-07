#include "display.h" // Näytön hallintaan liittyvät funktiot
#include <Arduino.h> // Arduinon peruskirjasto

extern int latchPin; // Siirtorekisterin latch pinni
extern int clockPin; // Siirtorekisterin kellopinni
extern int dataPin; // Siirtorekisterin datapinni
extern byte sevenSegDigits[]; // Taulukko 7-segmenttinäytön numeroille

////////// NÄYTÖN ALOITUS //////////
void initializeDisplay(void) {
    pinMode(latchPin, OUTPUT); // Asetetaan latch pinni lähtötilaan
    pinMode(clockPin, OUTPUT); // Asetetaan kellopinni lähtötilaan
    pinMode(dataPin, OUTPUT); // Asetetaan datapinni lähtötilaan
    digi
}

////////// NÄYTÖN PÄIVITYS //////////
void updateShiftRegister(int tensDigit, int onesDigit, bool showDots, int hundreds) {
  digitalWrite(latchPin, LOW); // Valmistellaan datan lähetys siirtorekisteriin

  byte onesSegment = sevenSegDigits[onesDigit]; // Haetaan oikean segmentin arvo
  byte tensSegment = sevenSegDigits[tensDigit]; // Haetaan vasemman segmentin arvo

  // Jos halutaan näyttää pisteet Highscore tilanteessa
  if (showDots) {
    onesSegment |= B10000000;  // Sytytetään oikean segmentin piste
    tensSegment |= B10000000;  // Sytytetään vasemman segmentin piste
  }

  // Lisätään logiikka sadan pisteen ylityksen ilmaisuun
  if (hundreds == 1) {
    onesSegment |= B10000000;  // Sytytetään oikean segmentin piste
  } else if (hundreds >= 2) {
    onesSegment |= B10000000;  // Sytytetään oikean segmentin piste
    tensSegment |= B10000000;  // Sytytetään vasemman segmentin piste
  }

  // Siirretään segmenttien arvot siirtorekisteriin
  shiftOut(dataPin, clockPin, MSBFIRST, onesSegment);  // Lähetä oikea numero
  shiftOut(dataPin, clockPin, MSBFIRST, tensSegment);  // Lähetä vasen numero

  digitalWrite(latchPin, HIGH); // Lukitaan data siirtorekisteriin ja päivitetään näyttö
}

////////// NÄYTÖN TYHJENNYS //////////
void clearDisplay() {
    digitalWrite(latchPin, LOW); // Valmistellaan datan lähetys siirtorekisteriin

    // Lähetetään nollat molemmille segmenteille
    shiftOut(dataPin, clockPin, MSBFIRST, 0x00);  // Tyhjennä oikea numero
    shiftOut(dataPin, clockPin, MSBFIRST, 0x00);  // Tyhjennä vasen numero

    digitalWrite(latchPin, HIGH); // Lukitaan data ja päivitetään näyttö tyhjäksi
}
