#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>
#include <Servo.h>

VR myVR(2, 3); 
Servo bioServo;
Servo nonBioServo;
uint8_t buf[64];

#define BIO_P1      (0)
#define NONBIO_P1   (1)
#define BIO_P2      (2)
#define NONBIO_P2   (3)

void setup() {
  myVR.begin(9600);
  Serial.begin(115200);
  
  // STARTUP FIX: Move to 0, then DETACH immediately
  bioServo.attach(9);
  nonBioServo.attach(10);
  bioServo.write(0);
  nonBioServo.write(0);
  delay(1000); 
  bioServo.detach();    // <--- Cut signal to stop jitter
  nonBioServo.detach(); // <--- Cut signal to stop jitter
  
  if (myVR.load((uint8_t)BIO_P1) >= 0) Serial.println("Bio P1 Loaded");
  if (myVR.load((uint8_t)NONBIO_P1) >= 0) Serial.println("Non-Bio P1 Loaded");
}

void loop() {
  int ret = myVR.recognize(buf, 50);

  if (ret > 0) {
    if (buf[1] == BIO_P1 || buf[1] == BIO_P2) {
      Serial.println("Command: Bio");
      bioServo.attach(9);       // 1. Wake up
      moveServoSlowly(bioServo, 0, 90);
      delay(3000);
      moveServoSlowly(bioServo, 90, 0);
      bioServo.detach();        // 2. Sleep (Stop Jitter)
    }
    else if (buf[1] == NONBIO_P1 || buf[1] == NONBIO_P2) {
      Serial.println("Command: Non-Bio");
      nonBioServo.attach(10);   // 1. Wake up
      moveServoSlowly(nonBioServo, 0, 90);
      delay(3000);
      moveServoSlowly(nonBioServo, 90, 0);
      nonBioServo.detach();     // 2. Sleep (Stop Jitter)
    }
  }
}

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
