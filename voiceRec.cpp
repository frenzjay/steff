#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>
#include <Servo.h>

// ----------------------------------------------------------
// HARDWARE SETTINGS (Check your wires!)
// ----------------------------------------------------------
// Voice Module TX  -> Connect to Arduino Pin 2
// Voice Module RX  -> Connect to Arduino Pin 3
// Bio Servo (Yellow Wire)     -> Connect to Arduino Pin 9
// Non-Bio Servo (Yellow Wire) -> Connect to Arduino Pin 10
// ----------------------------------------------------------

SoftwareSerial mySerial(2, 3); 
VR myVR(mySerial);

Servo bioServo;
Servo nonBioServo;

uint8_t buf[64];

// ----------------------------------------------------------
// COMMAND IDs (Must match what you "Trained")
// ----------------------------------------------------------
#define BIO_P1      (0)  // "Bio" (Person 1)
#define NONBIO_P1   (1)  // "Non-Bio" (Person 1)
#define BIO_P2      (2)  // "Bio" (Person 2)
#define NONBIO_P2   (3)  // "Non-Bio" (Person 2)

void setup() {
  mySerial.begin(9600);
  Serial.begin(115200);
  
  // 1. Attach Servos
  bioServo.attach(9);
  nonBioServo.attach(10);
  
  // 2. SAFETY STARTUP: Close lids immediately on power on
  Serial.println("System Starting... Closing Lids.");
  bioServo.write(0);
  nonBioServo.write(0);
  delay(1000); // Wait for them to settle

  // 3. Initialize Voice Module
  if (myVR.begin()) {
    Serial.println("VR Module Ready.");
  } else {
    Serial.println("VR Module NOT Found. Check RX/TX wires.");
  }

  // 4. Load the trained voice commands
  if (myVR.load(BIO_P1) >= 0) Serial.println("Bio P1 Loaded");
  if (myVR.load(NONBIO_P1) >= 0) Serial.println("Non-Bio P1 Loaded");
  if (myVR.load(BIO_P2) >= 0) Serial.println("Bio P2 Loaded");
  if (myVR.load(NONBIO_P2) >= 0) Serial.println("Non-Bio P2 Loaded");
}

void loop() {
  // Listen for a voice command
  int ret = myVR.recognize(buf, 50);

  if (ret > 0) {
    // ------------------------------------------------------
    // CASE 1: BIO COMMAND DETECTED
    // ------------------------------------------------------
    if (buf[1] == BIO_P1 || buf[1] == BIO_P2) {
      Serial.println("Command: BIO Detected");
      
      // Open Slowly (0 to 90 degrees)
      moveServoSlowly(bioServo, 0, 90);
      
      delay(3000); // Keep open for 3 seconds
      
      // Close Slowly (90 to 0 degrees)
      moveServoSlowly(bioServo, 90, 0); 
    }
    
    // ------------------------------------------------------
    // CASE 2: NON-BIO COMMAND DETECTED
    // ------------------------------------------------------
    else if (buf[1] == NONBIO_P1 || buf[1] == NONBIO_P2) {
      Serial.println("Command: NON-BIO Detected");
      
      // Open Slowly
      moveServoSlowly(nonBioServo, 0, 90);
      
      delay(3000); // Keep open for 3 seconds
      
      // Close Slowly
      moveServoSlowly(nonBioServo, 90, 0);
    }
  }
}

// ----------------------------------------------------------
// SMOOTH MOVEMENT FUNCTION
// (Prevents the servo from jerking and crashing the Arduino)
// ----------------------------------------------------------
void moveServoSlowly(Servo &s, int start, int end) {
  int stepDelay = 15; // Speed: Higher number = Slower/Safer
  
  if (start < end) {
    // Moving UP (0 -> 90)
    for (int pos = start; pos <= end; pos++) {
      s.write(pos);
      delay(stepDelay); 
    }
  } else {
    // Moving DOWN (90 -> 0)
    for (int pos = start; pos >= end; pos--) {
      s.write(pos);
      delay(stepDelay);
    }
  }
}
