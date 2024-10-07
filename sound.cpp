#include "sound.h" // Ääniefektit

#define SPEAKER_PIN 6 // Kaiuttimen pinni

////////// NUOTTIEN TAAJUUDET MIKROSEKUNTEINA //////////
int BassTab[] = {3000, 1702, 1516, 1431, 1275, 1136, 1012}; // Bass 1~7

////////// KAIUTTIMEN ALOITUSFUNKTIO //////////
void initializeSpeaker(void) {
    pinMode(SPEAKER_PIN, OUTPUT); // Määritetään kaiuttimen pinni
    digitalWrite(SPEAKER_PIN, LOW); // Varmistetaan, että kaiutin on aluksi pois päältä
}

////////// ÄÄNIEFEKTIEN TOISTO //////////
void sound(uint8_t note_index) {
    for (int i = 0; i < 100; i++) { // Toistetaan äänen sykli 100 kertaa
        digitalWrite(SPEAKER_PIN, HIGH); // Kaiutin päälle
        delayMicroseconds(BassTab[note_index]); // Odotetaan taulukosta haetun tajuuden ajan
        digitalWrite(SPEAKER_PIN, LOW); // Kaiutin pois päältä
        delayMicroseconds(BassTab[note_index]); // Odotetaan taulukosta haetun tajuuden ajan
    }
}
