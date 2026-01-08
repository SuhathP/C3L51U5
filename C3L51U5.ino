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

    do {
      C3L51U5.resetFlags();
      while (!C3L51U5.getFieldDetectionFlag()) {
        C3L51U5.scanField();
        C3L51U5.checkGround();
        delay(DELAY_SCAN);
      } 
    } while (C3L51U5.getGroundDetectionFlag());

    C3L51U5.strikePlayer();

    while (C3L51U5.getFieldDetectionFlag()) {
      C3L51U5.scanField();
      C3L51U5.checkGround();
      delay(DELAY_SCAN);

      if (C3L51U5.getGroundDetectionFlag()) 
        C3L51U5.setBotMotion(PIVOTING);
        updateMovement(); 
        break;
    }

    C3L51U5.setBotMotion(COAST);
    C3L51U5.updateMovement();
  
    C3L51U5.iterativeControl();
}
