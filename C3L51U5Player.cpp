#include "C3L51U5Player.h"
#include <Arduino.h>

Player::Player() { 
  currentTireSpeed = ZERO; // Initalize the values to be 0, or on brake for anything involving motion.
  currentVarTireSpeed = ZERO_PWM;
  currentBotMotion = BRAKE;
  currentPWM = 0;
  detectFlagField = 0;
  detectFlagGround = 0;
  accelerating = 0;
  objectDistance = 0;
  iterations = 0;
  linearErrors = 0;
}

void Player::initalizePins() {
  pinMode(TRIGGER, OUTPUT); // Setup pins for the ultrasonic sensor.
  digitalWrite(TRIGGER, LOW);
  pinMode(ECHO, INPUT);

  pinMode(IR, INPUT_PULLUP); // Setup pin for the IR sensor.

  pinMode(AIN1, OUTPUT); // Set up motor pins.
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(APWM, OUTPUT);
  pinMode(BPWM, OUTPUT);
}

void Player::setTireSpeed(TireSpeed newTireSpeed) {
  currentTireSpeed = newTireSpeed; // Set our current tire speed to the new, passed enumerative value.
}

void Player::setTireSpeed(int newTireSpeed) {
  currentVarTireSpeed = newTireSpeed; // Overloaded function to set tire speed to a changing integer during acceleration.
}

void Player::setBotMotion(BotMotion newBotMotion) {
  currentBotMotion = newBotMotion; // Set our new bot motion.
}

void Player::updateMovement() {

  if (!accelerating) { // If we are not accelerating, set the PWM to the right constant according to the state.
    switch (currentTireSpeed) {
      case ZERO: currentPWM = ZERO_PWM; break;
      case SLOW: currentPWM = SLOW_PWM; break;
      case MODERATE: currentPWM = MODERATE_PWM; break;
      case FAST: currentPWM = FAST_PWM; break;
      default: currentPWM = ZERO_PWM; break;
    }
  } else { // If we are not accelerating, set the current PWM to the speed we are currently on.
    currentPWM = currentVarTireSpeed;
  }

  switch (currentBotMotion) { // Control the PWM pins based on the state of the robot currently.

    case PIVOTING:
      digitalWrite(AIN1, LOW); digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, HIGH); digitalWrite(BIN2, HIGH);
      break;

    case SCANNING:
      digitalWrite(AIN1, LOW); digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW);
      break;

    case LINEAR:
      digitalWrite(AIN1, HIGH); digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW);
      break;

    case LINEAR_REVERSE:
      digitalWrite(AIN1, LOW); digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, LOW); digitalWrite(BIN2, HIGH);
      break;

    case COAST:
      digitalWrite(AIN1, LOW); digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, LOW); digitalWrite(BIN2, LOW);
      break;

    case BRAKE:
    default:
      digitalWrite(AIN1, HIGH); digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, HIGH); digitalWrite(BIN2, HIGH);
      break;
  }

  if (currentBotMotion == BRAKE) { // Function for the full brake of the robot.
    analogWrite(APWM, 255);   
    analogWrite(BPWM, 255);
  } else { // Update the PWM of the tires afterward, accounting for error if needed.
    analogWrite(APWM, ERROR_TIRES * currentPWM);
    analogWrite(BPWM, currentPWM);
  }

  if (currentBotMotion != LINEAR && currentBotMotion != SCANNING) // Always set a delay if it is not already accounted for in the main program.
    delay(MOVEMENT_DELAY);
}

void Player::scanField() {
  sendPulse(); // Send the pulse to the ultrasonic sensor.
  objectDistance = getObjectDistance(); // Get object distance using the helper function, then use it to set the object detection flag.
  setObjectDetectionFlag(objectDistance);
  checkGround(); // Also check the ground to ensure we are within the bounds of the arena.              

  if (currentBotMotion == LINEAR) // If we are striking the player, invoke the tolerance control to ensure we do not stop unless many zeroes/out of range values are detected.
    toleranceControl();
}

void Player::strikePlayer() {
  setBotMotion(LINEAR); // If we are striking the player, set the motion to linear and the speed to slow first.
  setTireSpeed(SLOW);
  updateMovement();

  accelerating = 1; // Switch on our acceleration flag.

  for (int i = 1; i <= FAST_PWM - SLOW_PWM; i++) { // Ramp up the PWM of the car progressively until it hits the max PWM (FAST state).
    setTireSpeed(SLOW_PWM + i);
    updateMovement();

    scanField(); // Always scan the field every chance we get.

    if (!detectFlagField || detectFlagGround) { // IMMEDIATE BREAK: If we lose the opponent or hit the edge during ramp-up
        accelerating = 0; // Reset flag so updateMovement uses standard speeds
        return; 
    }
    delay(ACCELERATION_DELAY); 
  }
  accelerating = 0; // Speeding up finished.
}

void Player::dodgePlayer() {
  setBotMotion(SCANNING); // Sharp fast turn, with the delay allowing for the action to take place.
  setTireSpeed(MODERATE);
  updateMovement();
  delay(DODGE_DELAY);

  setBotMotion(LINEAR); // Back up after the pivot to clear the line, extend the delay for a bit for safety.
  setTireSpeed(MODERATE);
  updateMovement();
  delay(DODGE_DELAY + 290); 
}

void Player::sendPulse() {
  digitalWrite(TRIGGER, LOW); // Send the pulse to the ultrasonic sensor, ensuring the trigger is on low first.
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
}

float Player::getObjectDistance() {
  unsigned long duration = pulseIn(ECHO, HIGH, TIMEOUT); // Use the pulse duration to grab the duration, then return if an out of range object (distance 0), is not present. Otherwise, return 0.
  if (duration != 0) return duration / 58.3f;
  else return 0;
}

void Player::setObjectDetectionFlag(float distance) {
  detectFlagField = (distance < DISTANCE_DETECTION && distance > 0); // Use the distance detection threshold (constant) to obtain our flag value (true/false)
}

void Player::setGroundDetectionFlag(int irValue) {
  detectFlagGround = (irValue == LOW); // LOW indicates white line, which is an edge.
}

void Player::checkGround() {
  int irValue = digitalRead(IR); // Check the ir value, an set the detection flag based on it.
  setGroundDetectionFlag(irValue);
}

void Player::resetFlags() {
  detectFlagField = 0; // Reset the flags to zero if needed.
  detectFlagGround = 0;
}

bool Player::getGroundDetectionFlag() {
  return detectFlagGround; // Get ground detection flag.
}

bool Player::getFieldDetectionFlag() {
  return detectFlagField; // Get field detection flag.
}

int Player::getTotalIterations() {
  return iterations; // Get the total number of iterations (Important for testing).
}

void Player::iterativeControl() {
  iterations++; // In our iterative control, increment the iterations, set our tolerance related value to a fresh zero after each iteration.
  linearErrors = 0;
  accelerating = 0;
}

void Player::toleranceControl() {
  if (objectDistance == 0 || objectDistance >= DISTANCE_DETECTION) // For tolerance, this is to ensure we only go back into linear mode if we detect a certain number of zeroes in a row.
    linearErrors++;
  else
    linearErrors = 0;

  if (linearErrors < TOLERANCE_VALUES)
    detectFlagField = 1;
}
