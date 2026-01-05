#include "C3L51U5Player.h"
#include <Arduino.h>

int Player::iterations = 0;

Player::Player()
{
  currentSpeed = STATIONARY; // Initialize members to ensure robot is not moving and no objects are detected.
  currentMotion = LINEAR;
  currentPWM = STATIONARY_PWM;
  detectFlagField = 0;
  detectFlagGround = 0;
  objectDistance = 0;
}

void Player::initalizePins() 
{
  pinMode(TRIGGER, OUTPUT); // First, take care of ultrasonic sensor pins TRIGGER & ECHO.
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  pinMode(ECHO, INPUT);

  pinMode(IR, INPUT); // Set up IR sensor pin.

  pinMode(A01, OUTPUT); // Lastly, set up motor pins.
  pinMode(A02, OUTPUT);
  pinMode(B01, OUTPUT);
  pinMode(B02, OUTPUT);
  pinMode(APWM, OUTPUT);
  pinMode(BPWM, OUTPUT);
}

void Player::setSpeed(Speed newSpeed) 
{
  currentSpeed = newSpeed; // Set the speed to the new setting.
}

void Player::setMotion(Motion newMotion) 
{
  currentMotion = newMotion; // Set the motion to the new setting.
}

void Player::updateMovement()
{
  switch (currentSpeed) { // Set the PWM of the motor to the correct defined constant for speed control.
    case STATIONARY:
      currentPWM = STATIONARY_PWM;
      break;
    case SLOW: 
      currentPWM = SLOW_PWM;
      break;
    case MODERATE:
      currentPWM = MODERATE_PWM;
      break;
    case FAST:
      currentPWM = FAST_PWM;
      break;
    default:
      currentPWM = STATIONARY_PWM;
      break;
  }
      
  switch (currentMotion) { // Set the appropriate motor settings for the current state motion.

    case PIVOTING:
      digitalWrite(A01, HIGH);
      digitalWrite(A02, LOW);

      digitalWrite(B01, HIGH);
      digitalWrite(B02, HIGH);

      break;

    case SCANNING:   
      digitalWrite(A01, HIGH);
      digitalWrite(A02, LOW);

      digitalWrite(B01, LOW);
      digitalWrite(B02, HIGH);

      break;
    case LINEAR:
      digitalWrite(A01, HIGH);
      digitalWrite(A02, LOW);

      digitalWrite(B01, HIGH);
      digitalWrite(B02, LOW);   

      break;  
    case COAST:
      digitalWrite(A01, LOW);
      digitalWrite(A02, LOW);

      digitalWrite(B01, LOW);
      digitalWrite(B02, LOW);

      break;

    case BRAKE:
      digitalWrite(A01, HIGH);
      digitalWrite(A02, HIGH);

      digitalWrite(B01, HIGH);
      digitalWrite(B02, HIGH);

      break;

    default: // By default, allow the bot to come to a hard stop (brake).
      digitalWrite(A01, HIGH);
      digitalWrite(A02, HIGH);

      digitalWrite(B01, HIGH);
      digitalWrite(B02, HIGH);
      break;

  }

  analogWrite(APWM, currentPWM); // After settings are set, start running motor at current speed.
  analogWrite(BPWM, currentPWM);  

  //if (currentMotion != PIVOTING || currentMotion != SCANNING) 
  //delay(MOVEMENT_DELAY); 
}

void Player::scanField() 
{
  sendPulse(); // Send a pulse, obtain the distance, and set the object detection flag while the bot is spinning.
  float distance = getObjectDistance();
  setObjectDetectionFlag(distance);
}

void Player::strikePlayer()
{
  setMotion(LINEAR); // When striking, set the motion to linear and fast to attack the player.
  setSpeed(FAST);
  updateMovement();

  setMotion(COAST); // Afterward, coast to a stop.
  updateMovement();
}

void Player::sendPulse()
{
  digitalWrite(TRIGGER, HIGH); // Send a 10 us pulse to the sensor.
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
}

float Player::getObjectDistance()
{
  float duration = pulseIn(ECHO, HIGH); // Calculation and returning of the distance if the duration recorded is non-zero (indicating object detected).
  if (!(int)duration) return duration/58.3;
  else return 0;
}

void Player::setObjectDetectionFlag(float distance)
{
  (distance < 70 && (int)distance != 0) ? detectFlagField = true : detectFlagField = false; // If distance is not 0 and less than 60, indicate that we have detected something.
}

void Player::checkGround()
{
  int color = digitalRead(IR);
  color ? detectFlagGround = true : detectFlagGround = false;
}

void Player::resetFlags()
{
  detectFlagField = 0; // Reset both detection flags to 0.
  detectFlagGround = 0;
}

bool Player::getGroundDetectionFlag()
{
  return detectFlagGround;
}

bool Player::getFieldDetectionFlag()
{
  return detectFlagField;
}

int Player::getTotalIterations()
{
  return iterations;
}

void Player::incrementIterations()
{
  iterations++;
}
// testing commit