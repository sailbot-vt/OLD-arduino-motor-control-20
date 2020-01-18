#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
class Stepper;
class StepperMotor{
public:
  StepperMotor(
    int numStepsPerRotation, int pin1, int pin2, int pin3, int pin4);
  Stepper * theStepper;
  void rotateToDegrees(int numDegrees);
  void changeSpeedTo(int speed);
private:
  int numStepsPerRotation;
  int currentDegrees;
  int speed;
};
