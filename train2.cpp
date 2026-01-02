#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>
#include <Servo.h>

/**
 * FINAL TRASH BIN CODE (2 SERVOS)
 * VR Module -> Pins 2 and 3
 * Servo 1 (Bio) -> Pin 9
 * Servo 2 (Non-Bio) -> Pin 10
 */

VR myVR(2, 3);    // The Red Voice Board

Servo bioServo;   // Motor 1 (Red Side)
Servo nonBioServo;// Motor 2 (Yellow Side)

uint8_t buf[64];

// These match the 'train' numbers you just did
#define BIO_COMMAND   (0) 
#define NONBIO_COMMAND (1)

void setup() {
  myVR.begin(9600);
  Serial.begin(115200);
  
  // Attach the two motors to Arduino pins
  bioServo.attach(9);
  nonBioServo.attach(10);
  
  // Start with both lids CLOSED (0 degrees)
  bioServo.write(0);
  nonBioServo.write(0);

  if (myVR.clear() == 0) {
    Serial.println("System Ready.");
  } else {
    Serial.println("Voice Module not found. Check wiring.");
  }

  // Load the voice commands you recorded
  myVR.load(BIO_COMMAND);
  myVR.load(NONBIO_COMMAND);
}

void loop() {
  int ret = myVR.recognize(buf, 50);

  // IF a voice is heard...
  if (ret > 0) {
    
    // === WAS IT COMMAND 0? (Biodegradable) ===
    if (buf[1] == BIO_COMMAND) {
      Serial.println("Opening Red Side...");
      bioServo.write(90); // Open Motor 1
      delay(3000);        // Wait 3 seconds
      bioServo.write(0);  // Close Motor 1
    }
    
    // === WAS IT COMMAND 1? (Non-Bio) ===
    else if (buf[1] == NONBIO_COMMAND) {
      Serial.println("Opening Yellow Side...");
      nonBioServo.write(90); // Open Motor 2
      delay(3000);           // Wait 3 seconds
      nonBioServo.write(0);  // Close Motor 2
    }
  }
}
