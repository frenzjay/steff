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
  
  bioServo.attach(9);
  nonBioServo.attach(10);
  
  bioServo.write(0);
  nonBioServo.write(0);

  if (myVR.clear() == 0) {
    Serial.println("Recognizer cleared.");
  } else {
    Serial.println("Not find VoiceRecognitionModule.");
  }

  myVR.load(BIO_P1);
  myVR.load(NONBIO_P1);
  myVR.load(BIO_P2);
  myVR.load(NONBIO_P2);
}

void loop() {
  int ret = myVR.recognize(buf, 50);

  if (ret > 0) {
    if (buf[1] == BIO_P1 || buf[1] == BIO_P2) {
      Serial.println("Bio Command Detected");
      bioServo.write(90);
      delay(3000);
      bioServo.write(0);
    }
    
    else if (buf[1] == NONBIO_P1 || buf[1] == NONBIO_P2) {
      Serial.println("Non-Bio Command Detected");
      nonBioServo.write(90);
      delay(3000);
      nonBioServo.write(0);
    }
  }
}
