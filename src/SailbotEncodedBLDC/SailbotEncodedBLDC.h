#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
class Encoder;
class PID;
softSerial1
class EncodedBLDC {
public:
  EncodedBLDC(int initialEncoderA, int initialEncoderB, int pwmSetSpeedPin, int forwardPin,
    int backPin);
  PID * thePID;
  Encoder * encoder;

  void setMotorPins(int pwmSetSpeedPin, int forwardPin,  int backPin);
  void setTunings(double newKp, double newKi, double newKd);

  double Setpoint = 0;
  double Input = 0;
  double Output = 0;
  int maxSpeed = 100;
  double Kp, Ki, Kd;
  void rotateToDegrees(int numDeg);
  void computePIDTestingTime(int numDeg);
private:
  int inputDeg;
  int numPulse = 0;
  int pwmSetSpeedPin = 9;   //L982N pin: ENA **MUST BE A PWM ENABLED PIN
  int inputForward = 10;    //L982N pin: IN2
  int inputBack = 11;       //L982N pin: IN1
  int speedInput;
  void rotate(int pwmOut);
};
