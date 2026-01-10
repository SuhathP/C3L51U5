#ifndef C3L51U5PLAYER_H  // Include Guards
#define C3L51U5PLAYER_H

#define TRIGGER 8  // Define constants for all of the trigger pins.
#define ECHO 7
#define IR 6
#define AIN1 5
#define AIN2 4
#define BIN1 2
#define BIN2 3
#define APWM 10
#define BPWM 9

#define ZERO_PWM 0  // Define constants for PWM.
#define SLOW_PWM 25
#define MODERATE_PWM 100
#define FAST_PWM 255
#define ERROR_TIRES 1

#define DELAY_SCAN 200          // Define constant for our pulse movement.
#define ULTRASONIC_PULSE_US 10  // Define constant for our ultrasonic sensor pulse.
#define TIMEOUT 38000           // Define constant for our timeout in ultrasonic sensor.
#define MOVEMENT_DELAY 100
#define ACCELERATION_DELAY 5
#define TOLERANCE_VALUES 5
#define DISTANCE_DETECTION 55

enum TireSpeed { ZERO,
                 SLOW,
                 MODERATE,
                 FAST };  // Enumeration to control the speed of the robot, corresponding with power levels / PWM.
enum BotMotion { SCANNING,
                 PIVOTING,
                 LINEAR,
                 LINEAR_REVERSE,
                 COAST,
                 BRAKE };  // Enumeration to control the motion of the robot

class Player {
private:  // Private members for the class including speed, motion, pwm, detection flag, and distance.
  enum TireSpeed currentTireSpeed;
  int currentVarTireSpeed;
  enum BotMotion currentBotMotion;
  int currentPWM;
  bool accelerating;
  bool detectFlagField;
  bool detectFlagGround;
  float objectDistance;
  int iterations;
  int linearErrors;

public:                  // Public members, specifically functions for movement, scanning, etc.
  Player();              // Constructor
  void initalizePins();  // Pin Initializer (Helper, Called In Constructor)

  void updateMovement();         // Motor Implementation Function
  void setTireSpeed(TireSpeed);  // Speed Setter
  void setTireSpeed(int);
  void setBotMotion(BotMotion);  // Motion Setter

  void scanField();     // Field Scanner Function
  void sendPulse();     // Helper Function - Ultrasonic Pulses
  void strikePlayer();  // Send Robot Towards Opponent Function
  void dodgePlayer();

  void checkGround();
  void setObjectDetectionFlag(float);  // Object Detection Flag Setter
  void setGroundDetectionFlag(int);    // Ground Detection Flag Setter
  void resetFlags();                   // Flag-Resetting Function

  float getObjectDistance();      // Getter For Object Distance
  bool getGroundDetectionFlag();  // Getter For Ground Detection Flag
  bool getFieldDetectionFlag();   // Getter For Field Detection Flag

  int getTotalIterations();  // Total Iteration Getter
  void iterativeControl();   // Iteration Incrementor

  void toleranceControl();  // Error Control
};


#endif