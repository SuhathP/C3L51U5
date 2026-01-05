#include "C3L51U5Player.h"

Player C3L51U5; // Create object for SumoBot.

void setup() {
  C3L51U5.initalizePins(); 
  delay(1000); // Required 5 s delay.
}

void loop() {
  C3L51U5.setSpeed(SLOW); 
  C3L51U5.setMotion(LINEAR); 
  C3L51U5.updateMovement(); 

  delay(2000);
    /*
    C3L51U5.setMotion(BRAKE); 
    C3L51U5.updateMovement();
    
    delay(300);
    
    C3L51U5.setSpeed(FAST);
    C3L51U5.setMotion(PIVOTING);
    C3L51U5.updateMovement();

    delay(2000);

    */

    /*
    C3L51U5.setSpeed(SLOW); 
    C3L51U5.setMotion(ROTATIONAL); 
    C3L51U5.updateMovement();

    while (!C3L51U5.getFieldDetectionFlag())
      C3L51U5.scanField();

    C3L51U5.setMotion(BRAKE);
    C3L51U5.updateMovement();

    C3L51U5.strikePlayer();
    C3L51U5.checkGround();

    if (C3L51U5.getGroundDetectionFlag())
      C3L51U5.setMotion(BRAKE);
      C3L51U5.updateMovement();
      
    C3L51U5.resetFlags()
    */

}
