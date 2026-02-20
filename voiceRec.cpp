#include "DFRobot_DF2301Q.h"
#include <Servo.h>

// Initialize the I2C Voice Module
DFRobot_DF2301Q_I2C asr;

Servo bioServo;
Servo nonBioServo;

// --- CALIBRATION ANGLES ---
// If the lid still goes too far down, change 20 to 30.
// If it doesn't close completely, change 20 to 10.
int closedAngle = 20; 
int openAngle = 90;   

void setup() {
  Serial.begin(115200);

  // 1. Force target positions BEFORE attaching to prevent violent swings
  bioServo.write(closedAngle);
  nonBioServo.write(closedAngle);
  
  // 2. Attach briefly to lock them into position
  bioServo.attach(9);
  nonBioServo.attach(10);
  delay(500); 
  
  // 3. DETACH IMMEDIATELY (Stops all jitter)
  bioServo.detach();    
  nonBioServo.detach(); 

  // 4. Start the Voice Module
  while (!(asr.begin())) {
    Serial.println("Voice Module not found! Check A4 and A5 wiring.");
    delay(3000);
  }
  Serial.println("Voice Module Ready!");

  // Module Settings (Volume 1-7, Wake time in seconds)
  asr.setVolume(5);
  asr.setWakeTime(15); 
}

void loop() {
  // Check if the module heard a command
  uint8_t CMDID = asr.getCMDID(); 

  if (CMDID != 0) {
    Serial.print("Heard Command ID: ");
    Serial.println(CMDID);

    // ------------------------------------------------------
    // COMMAND ID 5: BIO 
    // ------------------------------------------------------
    if (CMDID == 5) {
      Serial.println("Opening BIO Bin...");
      
      bioServo.attach(9); // Wake up muscle
      moveServoSlowly(bioServo, closedAngle, openAngle); // Open
      
      delay(3000); // Wait 3 seconds
      
      moveServoSlowly(bioServo, openAngle, closedAngle); // Close
      bioServo.detach(); // Put to sleep to stop jitter
    }
    
    // ------------------------------------------------------
    // COMMAND ID 6: NON-BIO 
    // ------------------------------------------------------
    else if (CMDID == 6) {
      Serial.println("Opening NON-BIO Bin...");
      
      nonBioServo.attach(10); // Wake up muscle
      moveServoSlowly(nonBioServo, closedAngle, openAngle); // Open
      
      delay(3000); // Wait 3 seconds
      
      moveServoSlowly(nonBioServo, openAngle, closedAngle); // Close
      nonBioServo.detach(); // Put to sleep to stop jitter
    }
  }
  // Small delay for stability
  delay(100);
}

// --- SMOOTH MOVEMENT FUNCTION ---
void moveServoSlowly(Servo &s, int start, int end) {
  int stepDelay = 10; // Change to 5 for faster opening, 15 for slower
  
  if (start < end) {
    for (int pos = start; pos <= end; pos++) {
      s.write(pos);
      delay(stepDelay); 
    }
  } else {
    for (int pos = start; pos >= end; pos--) {
      s.write(pos);
      delay(stepDelay);
    }
  }
}
