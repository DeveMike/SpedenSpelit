#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <EEPROM.h> // EEPROM muisti tallennusta varten

class Highscore {
public:
  Highscore(); // Alustetaan Highscore olio
  void setHighscore(int newScore); // Uusi highscore
  int getHighscore(); // Palauttaa tallennetun highscoren EEPROM:sta
private:
  const int highscoreAddress = 0; // EEPROM-osoite, johon highscore tallennetaan
};

#endif