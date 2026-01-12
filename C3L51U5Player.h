#ifndef C3L51U5PLAYER_H // Include Guards
#define C3L51U5PLAYER_H

#define TRIGGER 8 // Pin constants for the ultrasonic and IR sensor.
#define ECHO 7
#define IR 6

#define AIN1 5 // Define our motor pin constants.
#define AIN2 4
#define BIN1 2
#define BIN2 3
#define APWM 10
#define BPWM 9

#define ZERO_PWM 0 // Define our PWM values for each of the states of the finite state machine, along with error to account for tire resistance.
#define SLOW_PWM 45
#define MODERATE_PWM 100
#define FAST_PWM 250
#define ERROR_TIRES 1

#define DELAY_SCAN 50 // Define delays for scanning, ultrasonic sensor timeout, movement delays, tolerance/acceleration values, and distance which the sensor goes on by default.
#define TIMEOUT 38000
#define STOP_DELAY 100
#define DODGE_DELAY 60
#define ACCELERATION_DELAY 5
#define TOLERANCE_VALUES 3
#define DISTANCE_DETECTION 60

enum TireSpeed { ZERO, SLOW, MODERATE, FAST }; // Declare enumerations for each of our finite state machines, one for bot motion and one for tire speed.
enum BotMotion { SCANNING, PIVOTING, LINEAR, LINEAR_REVERSE, COAST, BRAKE };

class Player {
private: // Private members for each of the variables, including detection flags for the field and ground along with current speed, motion, and PWM.
  TireSpeed currentTireSpeed;
  int currentVarTireSpeed;
  BotMotion currentBotMotion;
  int currentPWM;
  bool accelerating;
  bool detectFlagField;
  bool detectFlagGround;
  float objectDistance;
  int iterations;
  int linearErrors;

public:
  Player(); // Constructor
  void initalizePins(); // Helper function to initialize the pins.

  void updateMovement(); // Function to update movement and control PWMs.
  void setTireSpeed(TireSpeed); // Setter functions, including an overloaded tireSpeed function for acceleration purposes.
  void setTireSpeed(int);
  void setBotMotion(BotMotion);

  void scanField(); // Function to scan the field.
  void checkGround(); // Function to check the ground.
  void sendPulse(); // Ultrasonic helper function to send pulse.
  void strikePlayer(); // Function to strike the player after being detected.
  void dodgePlayer(); // Function to dodge player based off IR sensor detection.

  void setObjectDetectionFlag(float); // Setter functions for the flags.
  void setGroundDetectionFlag(int);
  void resetFlags();

  float getObjectDistance(); // Getters for the object distance, and each of the detection flags.
  bool getGroundDetectionFlag();
  bool getFieldDetectionFlag();

  int getTotalIterations(); //Functions for iterative control.
  void iterativeControl();

  void toleranceControl(); // Function to control the tolerance of the zeroes detected.
};

#endif
