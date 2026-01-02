* **Microcontroller:** Arduino Uno R3
* **Voice Module:** Voice Recognition Module V3
* **Actuators:** Servo Motors
* **Power:** USB Cable (for testing) + 9V Battery
---

## Wiring

### Create the "Power Strip" (The Breadboard)

Your Arduino has only one "5V" slot, but you need to power two things. We use the white breadboard as a splitter.

 * Red Rail (+): Plug a wire from Arduino 5V --> to the Red Line (+) on the breadboard.
 * Blue Rail (-): Plug a wire from Arduino GND --> to the Blue Line (-) on the breadboard.

Now the whole Red line is power, and the whole Blue line is ground.

### Connect Everything (Final Step)
A. Connect the Red Voice Board (VRM2)
(Take the wires you just twisted onto the board in Part 2)
 * VCC Wire --> Plug into Red Line (+) on Breadboard.
 * GND Wire --> Plug into Blue Line (-) on Breadboard.
 * TXD Wire --> Plug into Arduino Pin 2.
 * RXD Wire --> Plug into Arduino Pin 3.
B. Connect the Blue Servo Motor
(Use 3 Male wires plugged into the black motor connector. Tape them if loose.)
 * Red (Middle) Wire --> Plug into Red Line (+) on Breadboard.
 * Brown (Dark) Wire --> Plug into Blue Line (-) on Breadboard.
 * Orange (Signal) Wire --> Plug into Arduino Pin 9.

If servo two servo:

 * Servo 1 (Red Side): Orange wire goes to Pin 9.
 * Servo 2 (Yellow Side): Orange wire goes to Pin 10.
 * (Both motors share the Red/Blue power rails on the breadboard).





| Component | Pin Label | Connect to Arduino Pin |
| :--- | :--- | :--- |
| **Voice Module V3** | VCC | **5V** |
| | GND | **GND** |
| | TX (Transmit) | **Pin 2** |
| | RX (Receive) | **Pin 3** |
| **Servo 1 (Bio)** | Red Wire | **5V** |
| | Brown Wire | **GND** |
| | Orange Wire | **Pin 9** |
| **Servo 2 (Non-Bio)**| Red Wire | **5V** |
| | Brown Wire | **GND** |
| | Orange Wire | **Pin 10** |


1.  **Install Library:** Open Arduino IDE → `Sketch` → `Include Library` → `Manage Libraries`. Search for and install **"VoiceRecognitionV3"** by Elechouse.
2.  **Upload Training Sketch:** Go to `File` → `Examples` → `VoiceRecognitionV3` → `vr_sample_train`. Upload this code to the Arduino.
3.  **Open Serial Monitor:**
    * Click the magnifying glass icon (top right).
    * Set the "Baud Rate" (bottom right dropdown) to **115200**.
4.  **Train Command 0 (Bio):**
    * Type `train 0` in the top bar and press Enter.
    * The monitor will say "Speak now". Clearly say **"Biodegradable"**.
    * It will ask you to speak again. Repeat the word.
    * If successful, it will say "Success".
5.  **Train Command 1 (Non-Bio):**
    * Type `train 1` and press Enter.
    * Clearly say **"Non-Bio"** (or "Yellow").
    * Repeat until success.




| Trial # | Intended Bin | Command Spoken | Did Lid Open? | Correct Bin? |
|---|---|---|---|---|
| 1 | Bio | "Biodegradable" | Yes | Yes |
| 2 | Non-Bio | "Non-Bio" | Yes | No (Error) |
| 3 | Bio | "Biodegradable" | No | - |
