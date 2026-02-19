#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>
#include <Servo.h>

// --- 1. HARDWARE PINS ---
// Voice RX -> Pin 2, Voice TX -> Pin 3
VR myVR(2, 3); 

Servo bioServo;
Servo nonBioServo;

uint8_t buf[64];

// --- 2. COMMAND IDs ---
#define BIO_P1      (0)
#define NONBIO_P1   (1)
#define BIO_P2      (2)
#define NONBIO_P2   (3)

// --- 3. CALIBRATION ANGLES (Fixes the "Mo Lapas" / Overshoot) ---
// If the lid still goes too far down, change 20 to 30.
// If it doesn't close enough, change 20 to 10.
int closedAngle = 20; 
int openAngle = 90;   

void setup() {
  Serial.begin(115200);
  myVR.begin(9600);
  
  Serial.println("System Booting...");

  // Force target positions BEFORE attaching to prevent violent swings
  bioServo.write(closedAngle);
  nonBioServo.write(closedAngle);
  
  // Attach briefly to lock them into the starting position
  bioServo.attach(9);
  nonBioServo.attach(10);
  delay(500); 
  
  // DETACH IMMEDIATELY (This stops the Timer Clash and Jitter)
  bioServo.detach();    
  nonBioServo.detach(); 
  
  Serial.println("Servos locked and put to sleep. Loading Voice Commands...");
  
  // Load Voice Commands
  if (myVR.load((uint8_t)BIO_P1) >= 0) Serial.println("Bio P1 Loaded");
  if (myVR.load((uint8_t)NONBIO_P1) >= 0) Serial.println("Non-Bio P1 Loaded");
  if (myVR.load((uint8_t)BIO_P2) >= 0) Serial.println("Bio P2 Loaded");
  if (myVR.load((uint8_t)NONBIO_P2) >= 0) Serial.println("Non-Bio P2 Loaded");
  
  Serial.println("Ready! Say a command.");
}

void loop() {
  int ret = myVR.recognize(buf, 50);

  if (ret > 0) {
    // ------------------------------------------------------
    // CASE 1: BIO COMMAND DETECTED
    // ------------------------------------------------------
    if (buf[1] == BIO_P1 || buf[1] == BIO_P2) {
      Serial.println("Command: BIO");
      
      bioServo.attach(9); // Wake up the muscle
      moveServoSlowly(bioServo, closedAngle, openAngle); // Open
      
      delay(3000); // Wait 3 seconds
      
      moveServoSlowly(bioServo, openAngle, closedAngle); // Close
      bioServo.detach(); // Put back to sleep to STOP JITTER
    }
    
    // ------------------------------------------------------
    // CASE 2: NON-BIO COMMAND DETECTED
    // ------------------------------------------------------
    else if (buf[1] == NONBIO_P1 || buf[1] == NONBIO_P2) {
      Serial.println("Command: NON-BIO");
      
      nonBioServo.attach(10); // Wake up the muscle
      moveServoSlowly(nonBioServo, closedAngle, openAngle); // Open
      
      delay(3000); // Wait 3 seconds
      
      moveServoSlowly(nonBioServo, openAngle, closedAngle); // Close
      nonBioServo.detach(); // Put back to sleep to STOP JITTER
    }
  }
}

// --- SMOOTH MOVEMENT FUNCTION ---
void moveServoSlowly(Servo &s, int start, int end) {
  int stepDelay = 10; // Speed: 5 is fast, 15 is slow
  
  if (start < end) {
    // Moving UP
    for (int pos = start; pos <= end; pos++) {
      s.write(pos);
      delay(stepDelay); 
    }
  } else {
    // Moving DOWN
    for (int pos = start; pos >= end; pos--) {
      s.write(pos);
      delay(stepDelay);
    }
  }
}
