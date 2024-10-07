#include "buttons.h" // Napinpainallusten käsittely

extern volatile int buttonNumber;  // Käytetään buttonNumber tilan viestimiseen
extern bool gameStarted;  // Viitataan pääohjelmassa määriteltyyn muuttujaan
unsigned long buttonPressStartTime = 0;   // Tallennetaan painalluksen aloitusaika
bool buttonBeingHeld = false;  // Muuttuja seuraa onko nappi pidetty alas painettuna

////////// DEBOUNCE //////////
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 300;  // 300 millisekunnin debounce viive

////////// PAINIKKEIDEN JA KESKEYTYSTEN ALUSTUS //////////
void initButtonsAndButtonInterrupts(void) {
    // Alustetaan pelin painikkeet (pinnit 2-5) sisääntuloiksi
    for (byte pin = firstPin; pin <= lastPin; pin++) {
        pinMode(pin, INPUT_PULLUP);  // Sisäinen pull-up vastus käytössä
    }

    // Otetaan käyttöön Pin Change Interrupt keskeytykset D väylälle (pinnit 2-5)
    PCICR |= (1 << PCIE2);  // Ota käyttöön portin D (pin 16-23) tilan muutoksen keskeytykset
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21); // Ota käyttöön keskeytykset
    //pinneille 2, 3, 4 ja 5, jotta ne huomaavat muutokset
}

////////// KESKEYTYKSEN KÄSITTELY JA DEBOUNCING //////////
ISR(PCINT2_vect) {
    // Tarkistetaan onko debounce aika kulunut
    unsigned long currentTime = millis();
    if ((currentTime - lastDebounceTime) > debounceDelay) {
        // Tarkistetaan mikä nappi on painettu ja päivitetään buttonNumber
        for (byte pin = firstPin; pin <= lastPin; pin++) {
            if (digitalRead(pin) == LOW) {  // Jos painike on painettu
                buttonNumber = pin;  // Päivitä buttonNumber
                lastDebounceTime = currentTime;  // Päivitä debounce-ajastin
                break;  // Lopeta tarkistus ensimmäisen painetun napin kohdalla
            }
        }
    }
}
