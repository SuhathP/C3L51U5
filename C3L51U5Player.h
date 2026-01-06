#ifndef C3L51U5PLAYER_H // Include Guards
#define C3L51U5PLAYER_H

#define TRIGGER 8 // Define constants for all of the trigger pins.
#define ECHO 7
#define IR 6
#define AIN1 4
#define AIN2 5
#define BIN1 3
#define BIN2 2
#define APWM 10
#define BPWM 9

#define ZERO_PWM 0 // Define constants for PWM.
#define SLOW_PWM 50
#define MODERATE_PWM 128
#define FAST_PWM 255

#define MOVEMENT_DELAY 100 // Define constant for our pulse movement.
#define ULTRASONIC_PULSE_US 10 // Define constant for our ultrasonic sensor pulse.
#define TIMEOUT 38000 // Define constant for our timeout in ultrasonic sensor.

enum TireSpeed {ZERO, SLOW, MODERATE, FAST}; // Enumeration to control the speed of the robot, corresponding with power levels / PWM.
enum BotMotion {SCANNING, PIVOTING, LINEAR, COAST, BRAKE}; // Enumeration to control the motion of the robot

class Player {
  private: // Private members for the class including speed, motion, pwm, detection flag, and distance.
    enum TireSpeed currentTireSpeed; 
    enum BotMotion currentBotMotion; 
    int currentPWM;
    bool detectFlagField;
    bool detectFlagGround;
    float objectDistance;
    static int iterations;

  public: // Public members, specifically functions for movement, scanning, etc.
    Player(); // Constructor
    void initalizePins(); // Pin Initializer (Helper, Called In Constructor)

    void updateMovement(); // Motor Implementation Function
    void setTireSpeed(TireSpeed); // Speed Setter
    void setBotMotion(BotMotion); // Motion Setter

    void scanField(); // Field Scanner Function
    void strikePlayer(); // Send Robot Towards Opponent Function

    void checkGround();
    void setObjectDetectionFlag(float); // Object Detection Flag Setter
    void setGroundDetection(int); // Ground Detection Flag Setter
    void resetFlags(); // Flag-Resetting Function

    float getObjectDistance(); // Getter For Object Distance
    bool getGroundDetectionFlag(); // Getter For Ground Detection Flag
    bool getFieldDetectionFlag(); // Getter For Field Detection Flag

    int getTotalIterations(); // Total Iteration Getter
    void incrementIterations(); // Iteration Incrementor
};


#endif