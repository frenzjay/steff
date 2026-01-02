#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>

/**
 * SIMPLE VOICE TRAINER
 * * Instructions:
 * 1. Upload this code.
 * 2. Open Serial Monitor (Magnifying Glass icon).
 * 3. Set Baud Rate to 115200.
 * 4. Type "train0" to train the Red Lid.
 * 5. Type "train1" to train the Yellow Lid.
 */

// Connection: TX to 2, RX to 3
VR myVR(2, 3);   
uint8_t buf[64];

void setup() {
  myVR.begin(9600);
  Serial.begin(115200);
  Serial.println("=== SYSTEM READY ===");
  Serial.println("Type 'train0' to record the BIO command.");
  Serial.println("Type 'train1' to record the NON-BIO command.");
}

void loop() {
  // Check if you typed something in the Serial Monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove extra spaces

    if (input == "train0") {
      performTraining(0, "BIODEGRADABLE");
    } 
    else if (input == "train1") {
      performTraining(1, "NON-BIO");
    }
    else {
      Serial.println("Unknown command. Type 'train0' or 'train1'");
    }
  }
}

// This function handles the "Speak Now" process
void performTraining(uint8_t recordID, String name) {
  Serial.print("Training: ");
  Serial.println(name);
  Serial.println("Please speak now...");

  // Send the training command to the red module
  if (myVR.train(recordID) >= 0) {
    Serial.print("Success! ");
    Serial.print(name);
    Serial.println(" has been recorded.");
  } else {
    Serial.println("Training Failed. Please try again.");
  }
}
