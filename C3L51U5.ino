#include "C3L51U5Player.h"

Player C3L51U5;  // Create object for SumoBot.

void setup() {
  Serial.begin(9600);
  C3L51U5.initalizePins();
  delay(5000);  // Required 5 s delay.
}

void loop() {
  C3L51U5.setBotMotion(SCANNING);
  C3L51U5.setTireSpeed(SLOW);
  C3L51U5.updateMovement();

  while (!C3L51U5.getFieldDetectionFlag()) {
    C3L51U5.scanField();
    delay(DELAY_SCAN);
  }

  C3L51U5.strikePlayer();

  while (C3L51U5.getFieldDetectionFlag()) {
    C3L51U5.scanField();
    delay(DELAY_SCAN);
  }

  C3L51U5.setBotMotion(BRAKE);
  C3L51U5.updateMovement();

  C3L51U5.resetFlags();
  C3L51U5.iterativeControl();
}

/*

void loop() {
  C3L51U5.setBotMotion(SCANNING);
  C3L51U5.setTireSpeed(SLOW);
  C3L51U5.updateMovement();

  while (!C3L51U5.getFieldDetectionFlag()) {
    C3L51U5.scanField();
    delay(DELAY_SCAN);
  }

  // Step 2 — run strike forward
  C3L51U5.strikePlayer();

  // Step 3 — CONTINUE forward until no enemy OR ground found
  while (C3L51U5.getFieldDetectionFlag() && !C3L51U5.getGroundDetectionFlag()) {
    C3L51U5.scanField();
    C3L51U5.checkGround();
    C3L51U5.setBotMotion(LINEAR);
    C3L51U5.updateMovement();
    delay(DELAY_SCAN);
  }

  // Step 4 — If ground detected → dodge
  if (C3L51U5.getGroundDetectionFlag()) {
    C3L51U5.dodgePlayer();
  }

  // Step 5 — brake, reset, start over
  C3L51U5.setBotMotion(BRAKE);
  C3L51U5.updateMovement();
  C3L51U5.resetFlags();
  C3L51U5.iterativeControl();
}

*/
