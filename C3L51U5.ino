#include "C3L51U5Player.h"

Player C3L51U5; // Declare the player globally.

void setup() {
  C3L51U5.initalizePins(); // Initalize pins.
  delay(5000);  // Required 5 second delay before match
}

void loop() {
  C3L51U5.setBotMotion(SCANNING); // Begin the code in scanning (rotate) mode.
  C3L51U5.setTireSpeed(SLOW);
  C3L51U5.updateMovement();

  while (!C3L51U5.getFieldDetectionFlag()) {
    C3L51U5.scanField(); // Scan the field, but also include a delay to ensure no faulty measurement.
    delay(DELAY_SCAN); 
  }

  C3L51U5.strikePlayer(); // Accelerate and strike the player.

  while (C3L51U5.getFieldDetectionFlag()) { // If we are still detecting the player, keep striking.
    C3L51U5.scanField();

    if (C3L51U5.getGroundDetectionFlag()) { // To avoid confusion, simply use the getter for detection flag for the ground to avoid helper functions within the class.
      break; // Exit if we detect the rim to the logic below.
    }
    delay(DELAY_SCAN); // Always have a delay between scans.
  }

  C3L51U5.setBotMotion(BRAKE); // Always stop the robot before anything else happens.
  C3L51U5.updateMovement();

  if (C3L51U5.getGroundDetectionFlag()) { // If we stopped because of the ground, dodge and continue.
      delay(DELAY_SCAN); // Short settle, using the same constant for the SCAN delay.
      C3L51U5.dodgePlayer();
  }

  C3L51U5.resetFlags(); // Reset the flags and control our iterations.
  C3L51U5.iterativeControl();
}