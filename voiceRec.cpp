#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>
#include <Servo.h>

// ----------------------------------------------------------
// HARDWARE SETTINGS
// ----------------------------------------------------------
// Voice RX (Green/White) -> Pin 2
// Voice TX (Blue/Yellow) -> Pin 3
// Bio Servo -> Pin 9
// Non-Bio Servo -> Pin 10
// ----------------------------------------------------------

// FIX: Initialize library with Pins directly (Fixes "No matching function" error)
VR myVR(2, 3); 

Servo bioServo;
Servo nonBioServo;

uint8_t buf[64];

// ----------------------------------------------------------
// COMMAND IDs
// ----------------------------------------------------------
#define BIO_P1      (0)
#define NONBIO_P1   (1)
#define BIO_P2      (2)
#define NONBIO_P2   (3)

void setup() {
  // Initialize Serial for Monitor
  Serial.begin(115200);
  
  // Initialize Voice Module
  myVR.begin(9600);
  
  // --- CRITICAL FIX FOR ROTATING ON STARTUP ---
  // We write the position BEFORE attaching to prevent jumping
  bioServo.write(0);      
  nonBioServo.write(0);   
  
  // Now we connect the servo
  bioServo.attach(9);
  nonBioServo.attach(10);
  
  // Wait a moment for them to lock into position 0
  delay(1000); 

  Serial.println("System Ready. Servos Locked at 0.");

  // Load the voice commands
  if (myVR.load((uint8_t)BIO_P1) >= 0) Serial.println("Bio P1 Loaded");
  if (myVR.load((uint8_t)NONBIO_P1) >= 0) Serial.println("Non-Bio P1 Loaded");
  if (myVR.load((uint8_t)BIO_P2) >= 0) Serial.println("Bio P2 Loaded");
  if (myVR.load((uint8_t)NONBIO_P2) >= 0) Serial.println("Non-Bio P2 Loaded");
}

void loop() {
  int ret = myVR.recognize(buf, 50);

  if (ret > 0) {
    // --- BIO COMMAND ---
    if (buf[1] == BIO_P1 || buf[1] == BIO_P2) {
      Serial.println("Command: BIO Detected");
      moveServoSlowly(bioServo, 0, 90);  // Open
      delay(3000);                       // Wait 3 seconds
      moveServoSlowly(bioServo, 90, 0);  // Close
    }
    
    // --- NON-BIO COMMAND ---
    else if (buf[1] == NONBIO_P1 || buf[1] == NONBIO_P2) {
      Serial.println("Command: NON-BIO Detected");
      moveServoSlowly(nonBioServo, 0, 90); // Open
      delay(3000);                         // Wait 3 seconds
      moveServoSlowly(nonBioServo, 90, 0); // Close
    }
  }
}

// --- SPEED CONTROL ---
void moveServoSlowly(Servo &s, int start, int end) {
  // Change '5' to '15' if you want it slower. 
  // Change '5' to '2' if you want it faster.
  int stepDelay = 5; 
  
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
