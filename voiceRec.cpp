#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>
#include <Servo.h>

VR myVR(2, 3);
Servo bioServo;   // For Biodegradable
Servo nonBioServo;// For Non-Biodegradable

uint8_t buf[64];

// for training
#define BIO_COMMAND   (0) 
#define NONBIO_COMMAND (1)

void setup() {
  myVR.begin(9600);
  Serial.begin(115200);
  
  bioServo.attach(9);   // Bio Bin on Pin 9
  nonBioServo.attach(10); // Non-Bio Bin on Pin 10
  
  bioServo.write(0);
  nonBioServo.write(0);

  if (myVR.clear() == 0) {
    Serial.println("+1");
  } else {
    Serial.println("-1");
  }

  // Load the trained commands
  myVR.load(BIO_COMMAND);
  myVR.load(NONBIO_COMMAND);
}

void loop() {
  int ret = myVR.recognize(buf, 50);

  // If a voice command is heard
  if (ret > 0) {
    
    // Check if Bio
    if (buf[1] == BIO_COMMAND) {
      Serial.println("0");
      bioServo.write(90);
      delay(3000);
      bioServo.write(0);
    }
    
    // Check if non bio
    else if (buf[1] == NONBIO_COMMAND) {
      Serial.println("1");
      nonBioServo.write(90);
      delay(3000);
      nonBioServo.write(0);
    }
  }
}
