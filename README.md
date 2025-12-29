* **Microcontroller:** Arduino Uno R3
* **Voice Module:** Voice Recognition Module V3
* **Actuators:** Servo Motors
* **Power:** USB Cable (for testing) + 9V Battery
---

## Wiring


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
