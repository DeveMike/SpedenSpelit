#include "display.h"

// Alustaa 7-segmenttinäytön
void initializeDisplay(void) {
  // See requirements for this function from display.h
  pinMode(12, OUTPUT); // Reset
  pinMode(11, OUTPUT); // Shift Clock
  pinMode(10, OUTPUT); // Latch Clock
  pinMode(9, OUTPUT);  // Out Enable
  pinMode(8, OUTPUT);  // Serial Input
}

// Kirjoittaa yhden numeron 7-segmenttinäyttöön
void writeByte(uint8_t number, bool last) {
  // See requirements for this function from display.h
  // TODO: Lisää koodi kirjoittamaan bitti
}

// Kirjoittaa numerot 0-99 7-segmenttinäyttöön
void writeHighAndLowNumber(uint8_t tens, uint8_t ones) {
  // TODO: Lisää koodi kirjoittamaan kympit ja ykköset
  // See requirements for this function from display.h

}

// Näyttää tuloksen 7-segmenttinäytössä
void showResult(byte result) {
  // Erota kympit ja ykköset ja kutsu writeHighAndLowNumber
  // See requirements for this function from display.h

}
