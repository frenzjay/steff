#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>
#include <Servo.h>

// Initialize Voice Module (RX -> Pin 2, TX -> Pin 3)
VR myVR(2, 3); 

Servo bioServo;
Servo nonBioServo;

uint8_t buf[64];

// Command IDs
#define BIO_P1      (0)
#define NONBIO_P1   (1)
#define BIO_P2      (2)
#define NONBIO_P2   (3)

void setup() {
  Serial.begin(115200);
  myVR.begin(9600);
  
  // 1. Force position to 0 BEFORE attaching to stop wild swings
  bioServo.write(0);
  nonBioServo.write(0);
  
  // 2. Attach servos briefly to lock them at 0
  bioServo.attach(9);
  nonBioServo.attach(10);
  delay(1000); 
  
  // 3. Detach them immediately. 
  // This cuts the signal and completely stops the "weird" jittering!
  bioServo.detach();    
  nonBioServo.detach(); 
  
  Serial.println("Smart Bin Ready. Waiting for command...");
  
  // Load Voice Commands
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
      Serial.println("Command: BIO");
      bioServo.attach(9);               // Wake up the motor
      moveServoSlowly(bioServo, 0, 90); // Open
      delay(3000);                      // Wait 3 seconds
      moveServoSlowly(bioServo, 90, 0); // Close
      bioServo.detach();                // Put motor back to sleep
    }
    
    // --- NON-BIO COMMAND ---
    else if (buf[1] == NONBIO_P1 || buf[1] == NONBIO_P2) {
      Serial.println("Command: NON-BIO");
      nonBioServo.attach(10);               // Wake up the motor
      moveServoSlowly(nonBioServo, 0, 90);  // Open
      delay(3000);                          // Wait 3 seconds
      moveServoSlowly(nonBioServo, 90, 0);  // Close
      nonBioServo.detach();                 // Put motor back to sleep
    }
  }
}

// Smooth Movement Function
void moveServoSlowly(Servo &s, int start, int end) {
  int stepDelay = 10; // Adjust speed: 5 is fast, 15 is slow
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
