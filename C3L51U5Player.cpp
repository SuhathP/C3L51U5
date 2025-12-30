#include "C3L51U5Player.h"
#include <Arduino.h>

Player::Player()
{
  currentSpeed = STATIONARY; //Current 
  objectDetected = 0;
  objectDistance = 0;
}