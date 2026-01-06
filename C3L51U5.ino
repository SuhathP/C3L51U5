#include "C3L51U5Player.h"

Player C3L51U5; // Create object for SumoBot.

void setup() {
  Serial.begin(9600);
  C3L51U5.initalizePins(); 
  delay(1000); // Required 5 s delay.
}

void loop() {

  while (C3L51U5.getTotalIterations() == 0)
  {
    C3L51U5.setTireSpeed(SLOW); 
    C3L51U5.setBotMotion(SCANNING); 
    C3L51U5.updateMovement();

    while (!C3L51U5.getFieldDetectionFlag())
    {
      C3L51U5.scanField();
      delay(50);
    }

    C3L51U5.strikePlayer();
    C3L51U5.setBotMotion(BRAKE); 
    
    C3L51U5.incrementIterations();
  }
}
