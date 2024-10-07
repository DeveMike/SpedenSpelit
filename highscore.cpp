#include "highscore.h" // Highscore käsittely

////////// ENNÄTYKSEN ALUSTUS //////////
Highscore::Highscore() {
  // Voimme alustaa highscoreksi 0, jos EEPROM:ssa ei ole arvoa
  if (EEPROM.read(highscoreAddress) == 0xFF) {
    setHighscore(0); // Asetetaan highscore arvoksi 0
  }
}

////////// ENNÄTYKSEN TALLENNUS //////////
void Highscore::setHighscore(int newScore) {
  // Tallennetaan highscore kahteen EEPROM-osoitteeseen (2 tavua)
  EEPROM.update(highscoreAddress, newScore & 0xFF);             // Alempi tavu
  EEPROM.update(highscoreAddress + 1, (newScore >> 8) & 0xFF);  // Ylempi tavu
}

////////// ENNÄTYKSEN LUKEMINEN EEPROM MUISTISTA //////////
int Highscore::getHighscore() {
  // Luetaan highscore kahdesta EEPROM-osoitteesta
  int lowerByte = EEPROM.read(highscoreAddress); // Luetaan alempi tavu
  int upperByte = EEPROM.read(highscoreAddress + 1); // Luetaan ylempi tavu
  // Yhdistetään ylempi ja alempi tavu 16-bittiseksi luvuksi ja palautetaan highscore
  return (upperByte << 8) | lowerByte; // Siirretään ylempi tavu 8 bittiä vasemmalle ja yhdistetään alempaan tavuun
}