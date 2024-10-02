#include "buttons.h"


extern volatile int buttonNumber;  // Käytetään buttonNumber tilan viestimiseen
bool gameStarted = false;          // Seurataan onko peli käynnissä
unsigned long buttonPressStartTime = 0;   // Tallennetaan painalluksen aloitusaika
const unsigned long startPressDuration = 2000;  // 2 sec painallus aloittaa pelin
bool buttonBeingHeld = false;  // Muuttuja seuraa onko nappi pidetty alas painettuna

// Debouncingin ajastin ja kesto
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 100;  // 100 millisekunnin debounce viive

void initButtonsAndButtonInterrupts(void)
{
    // Alustetaan pelin painikkeet (pinnit 2-5) sisääntuloiksi
    for (byte pin = firstPin; pin <= lastPin; pin++) {
        pinMode(pin, INPUT_PULLUP);  // Sisäinen pull-up vastus käytössä
    }

    // Otetaan käyttöön Pin Change Interrupt keskeytykset D väylälle (pinnit 2-5)
    PCICR |= (1 << PCIE2);  // Ota käyttöön portin D (pin 16-23) tilan muutoksen keskeytykset
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21); // Ota käyttöön keskeytykset
    //pinneille 2, 3, 4 ja 5, jotta ne huomaavat muutokset
}

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

// Tarkistetaan, onko pinni 2 painettuna vähintään 2 sekuntia
void checkStartButton(void) {
    if (!gameStarted) {
        if (digitalRead(2) == LOW) {
            // Jos nappi on painettuna ja ei ole jo pidettynä
            if (!buttonBeingHeld) {
                buttonPressStartTime = millis();  // Tallennetaan painalluksen aloitusaika
                buttonBeingHeld = true;  // Merkitään, että nappi on nyt alas painettuna
            }

            // Jos nappia on pidetty painettuna vähintään 2 sekuntia, peli alkaa
            if (millis() - buttonPressStartTime >= startPressDuration) {
                Serial.println("Peli alkaa!");
                gameStarted = true;  // Peli alkaa
                buttonNumber = -1;   // Nollataan buttonNumber
                buttonBeingHeld = false;  // Nollataan painalluksen tila
            }
        } else {
            // Jos nappia ei pidetä painettuna, nollataan ajastin
            buttonPressStartTime = 0;
            buttonBeingHeld = false;  // Nappi vapautettiin, nollataan tila
        }
    }
}

