#include "C3L51U5Player.h"
#include <Arduino.h>

int Player::iterations = 0;

Player::Player()
{
  currentTireSpeed = ZERO_PWM; // Initialize members to ensure robot is not moving and no objects are detected.
  currentBotMotion = SCANNING;
  currentPWM = 0;
  detectFlagField = 0;
  detectFlagGround = 0;
  objectDistance = 0;
}

void Player::initalizePins() 
{
  pinMode(TRIGGER, OUTPUT); // First, take care of ultrasonic sensor pins TRIGGER & ECHO.
  digitalWrite(TRIGGER, LOW);
  pinMode(ECHO, INPUT);

  pinMode(IR, INPUT); // Set up IR sensor pin.

  pinMode(AIN1, OUTPUT); // Lastly, set up motor pins.
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(APWM, OUTPUT);
  pinMode(BPWM, OUTPUT);
}

void Player::setTireSpeed(TireSpeed newTireSpeed) 
{
  currentTireSpeed = newTireSpeed; // Set the speed to the new setting.
}

void Player::setBotMotion(BotMotion newBotMotion) 
{
  currentBotMotion = newBotMotion; // Set the motion to the new setting.
}

void Player::updateMovement()
{
  switch (currentTireSpeed) { // Set the PWM of the motor to the correct defined constant for speed control.
    case ZERO:
      currentPWM = ZERO_PWM;
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
      currentPWM = ZERO_PWM;
      break;
  }
      
  switch (currentBotMotion) { // Set the appropriate motor settings for the current state motion.

    case PIVOTING:
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);

      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, HIGH);

      break;

    case SCANNING:   
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);

      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);

      break;
    case LINEAR:
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);

      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);   

      break;  
    case COAST:
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);

      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, LOW);

      break;

    case BRAKE:
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, HIGH);

      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, HIGH);

      break;

    default: // By default, allow the bot to come to a hard stop (brake).
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, HIGH);

      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, HIGH);
      break;

  }

  analogWrite(APWM, currentPWM); // After settings are set, start running motor at current speed.
  analogWrite(BPWM, currentPWM);  

}

void Player::scanField() 
{
  sendPulse();
  float objectDistance = getObjectDistance();
  setObjectDetectionFlag(objectDistance);
  Serial.print(objectDistance);
  Serial.print("\n");
}

void Player::strikePlayer()
{
  setBotMotion(LINEAR); // When striking, set the motion to linear and fast to attack the player.
  setTireSpeed(FAST);
  updateMovement()
}

void Player::sendPulse()
{
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH); // Send a 10 us pulse to the sensor.
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

}
float Player::getObjectDistance()
{
  unsigned long duration = pulseIn(ECHO, HIGH, TIMEOUT); // Calculation and returning of the distance if the duration recorded is non-zero (indicating object detected).
  if (duration != 0) return duration/58.3f;
  else return 0;
}

void Player::setObjectDetectionFlag(float distance)
{
  (distance < 70 && distance > 0) ? detectFlagField = true : detectFlagField = false; // If distance is not 0 and less than 60, indicate that we have detected something.
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