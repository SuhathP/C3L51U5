#ifndef C3L51U5PLAYER_H
#define C3L51U5PLAYER_H

#define TRIGGER 1
#define ECHO 2
#define IR 3
#define A01 4
#define A02 5
#define B01 6
#define B02 7
#define APWM 8
#define BPWM 9

enum Speed {STATIONARY, SLOW, MODERATE, FAST} // Enumeration to control the speed of the robot.

class Player {
  private:
    enum Speed currentSpeed;
    bool objectDetected;
    float objectDistance;

  public:
    Player();
    
    
    
};

#endif