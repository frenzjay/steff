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

// FIX 1: Your library wants pins directly, not a serial object
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
  // FIX 2: Your library handles the serial connection internally
  myVR.begin(9600); 
  
  Serial.begin(115200);
  
  bioServo.attach(9);
  nonBioServo.attach(10);
  
  // Safety Start
  bioServo.write(0);
  nonBioServo.write(0);
  delay(1000);

  // FIX 3: We cast to (uint8_t) to fix the "ambiguous" error
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
      moveServoSlowly(bioServo, 0, 90);
      delay(3000); 
      moveServoSlowly(bioServo, 90, 0); 
    }
    
    // --- NON-BIO COMMAND ---
    else if (buf[1] == NONBIO_P1 || buf[1] == NONBIO_P2) {
      Serial.println("Command: NON-BIO Detected");
      moveServoSlowly(nonBioServo, 0, 90);
      delay(3000); 
      moveServoSlowly(nonBioServo, 90, 0);
    }
  }
}

// Smooth Movement Function
void moveServoSlowly(Servo &s, int start, int end) {
  int stepDelay = 15; 
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
