# Speden Spelit Reaction Game – Arduino Edition


**Speden Spelit** is an Arduino-based reaction and speed game, inspired by the Finnish TV show of the same name from the 1990s. The player’s goal is to collect points by pressing the button corresponding to the lit LED as quickly as possible.

The device is assembled using Arduino and several components, such as shift registers, 7-segment displays, a speaker, LEDs, and resistors, following a circuit diagram. All components are carefully soldered, installed in a case, and tested for functionality. The Arduino power cable is the only external physical element. The code is modularly programmed, making it easy to extend the game’s features as needed.

The game starts by holding the start button for 2 seconds. After that, a start sound plays, and the first LED lights up. The player’s objective is to press the correct button as soon as the corresponding light turns on. After every 10 successful presses, the LED blink speed increases by 10%. The game ends if the player presses the wrong button or fails to press the correct button in time.

During gameplay, the 7-segment displays indicate the game’s progress with the help of sound cues. When the player reaches 100 points, the first decimal point on the display lights up, showing that the score exceeds 99. This repeats at 200 points when the second decimal point turns on. At the end of the game, the score is saved to the device’s memory, where the player’s high score is displayed. If the device is inactive for 60 seconds, it automatically switches to power-saving mode. The device can be reactivated by pressing the start button once.

This introduction presents the **Speden Spelit** device, which combines the excitement of a traditional game with modern electronics. The project was carried out as part of the *Tietotekniikan sovellusprojekti* (Application Project in Information Technology) course at Oulu University of Applied Sciences.



![promo2](https://github.com/user-attachments/assets/f8ad9e48-5bbe-4b00-b72d-6d8116a723de)

# Contents

# Technical Specifications

•	**Circuit Diagram:** The project includes a circuit diagram that shows how the game’s electronic components, such as buttons, LEDs, buzzers, 7-segment displays, and shift registers, are connected to the Arduino.

<img width="1124" alt="Screenshot 2024-10-15 at 15 12 37" src="https://github.com/user-attachments/assets/fcd38a9b-6b49-4dce-ad9e-ea7b2446d286">

•	**Flowchart:** Explains the game’s logic and the interaction between different functions, helping to visualize the game’s flow.

<img width="795" alt="Screenshot 2024-10-15 at 15 13 28" src="https://github.com/user-attachments/assets/a457ce6e-351a-4e8a-901a-2e97b0b54861">


•	**Code Files:** The program code is divided into modular sections, such as handling buttons, displays, score calculation, and sound effects.

<img width="211" alt="Screenshot 2024-10-15 at 15 40 37" src="https://github.com/user-attachments/assets/ef2d5b4e-e004-434c-86c5-8dccc8ac24fa">


•	**Video Presentation:** A practical demonstration showing the completed device in action.
https://youtu.be/qdzAhDunAkY

# Assembly
**Required Components:**

• Arduino Uno (or equivalent)

• Arduino IDE

• 4x BROADCOM HLMP-4740 LEDs (or equivalent)

• 4x PANASONIC ESE20C321 Buttons

• 2x KINGBRIGHT SC52-11EWA 7-segment displays

• 2x TEXAS INSTRUMENTS SN74HC595N Shift Registers

• Crowtail Speaker

• A sufficient amount of jumper wires and resistors

• A breadboard large enough to accommodate the components

• If preferred, you can also solder the components to a printed circuit board (PCB) and enclose the device in a case.


![Mediasisältö](https://github.com/user-attachments/assets/0fc59c87-1206-473f-9d1a-f863519c6c72)

# Serial Monitor Event Tracking:

<img width="268" alt="Screenshot 2024-10-11 at 14 09 56" src="https://github.com/user-attachments/assets/fc799035-19d4-499e-88e8-967aa2689f7c">


•	**Connections:** The wiring must be done according to the circuit diagram to ensure the game works as intended by the software code. Special attention is needed with the buttons and LEDs to ensure they are connected to the correct ports on the Arduino.

•	**Software Requirements:** You will need the Arduino IDE installed on your computer. Through the IDE, you can upload all files directly to the Arduino.

•	**Uploading Code:** Upload the program code from the SpedenSpelit_v2_6.ino file, and make sure to include the necessary .cpp and .h files.
