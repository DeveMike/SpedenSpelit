#ifndef DISPLAY_H
#define DISPLAY_H

void initializeDisplay(void); // Alustaa 7-segmenttinäytön
void clearDisplay(); // Tyhjentää näytön ja sammuttaa kaikki segmentit
// Päivittää 7- kymmenillä, ykkösillä, valinnaisella desimaalipisteellä ja sadoilla.
void updateShiftRegister(int tensDigit, int onesDigit, bool showDots = false, int hundreds = 0);

#endif
