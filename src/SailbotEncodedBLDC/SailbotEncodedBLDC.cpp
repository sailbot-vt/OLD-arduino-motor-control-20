#include "SailbotEncodedBLDC.h"
#include "../PID/PID_v1.h"
#include "../Encoder/Encoder.h"

/** Class for controlling a BLDC motor w/ Encoder using PID control.
 * param initialEncoder's - Must be hardware interrupt pins.
 * param pwmSetSpeedPin - must be pwm enabled.
 * param forward / backPin - can be any digital output pins
 *
 * See #TODO's in rotateToDegrees
 */
EncodedBLDC::EncodedBLDC(
    int initialEncoderA,
      int initialEncoderB, int pwmSetSpeedPin, int forwardPin,
        int backPin) {
  // Encoder initialization
  encoder = new Encoder(initialEncoderA, initialEncoderB);

  thePID  = new PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
  thePID->SetMode(AUTOMATIC);
  //Adjust PID values
  thePID->SetTunings(Kp, Ki, Kd);
  thePID->SetSampleTime(1);
  thePID->SetOutputLimits(-maxSpeed, maxSpeed);
}
/** Sets the tunings for the PID. Must be reconfigured for ANY change to set up
 * by default these are set to 0 and must be changed in order to do anything
 * previously working non-load values are 7, 0, .05
 * See below link for help tuning.
 https://robotics.stackexchange.com/questions/167/what-are-good-strategies-for-tuning-pid-loops
 */
void EncodedBLDC::setTunings(double newKp, double newKi, double newKd) {
    thePID->SetTunings(newKp, newKi, newKd);
}
/** Rotates the motor to a certain degrees. #TODO 1 #TODO 2
 * Value is always off and you will never get it 100% every single time.
 * However, with good tuning, the motor can typically settle within 2-3 pulses
 * value with 1-2 over/undershoots. See block comment on while loop
 * Serial print statements can be taken out, however there seems to be problems
 * without them as the provide a small delay that actually helps the algorithm
 * work. See if you can fix it.
 */
void EncodedBLDC::rotateToDegrees(int numDeg) {
  numPulse = map(numDeg, -360, 360, -1632, 1632) + encoder->read();
  Serial.print("Num pulse to move:");
  Serial.println(String(numPulse));
  Setpoint = numPulse;
  thePID->Compute();
  int startTime = millis();
  int timeGone = millis();
  /* The while loop condition needs to be better. Running for just 2 seconds is terrible
     The loop CANNOT simply check if the position is correct
     Checking based on position creates errors as sometimes the PID
     Overshoots and plans to correct, terminating quickly will cause any transients
     to continue to rotate the motor. It must settle.
     However, this loop condition needs to be improved as it will sit idle if
     the motor settles in 2 seconds, and will actually not have time to
     get to the correct position if the number of rotations is high enough
     This most likely can be fixed by finding some relation between
     time it takes to rotate, speed, and degrees requested.
     Write timing script for the arduino, make a new version of this method,
     adjust a serial output with the timings, capture the serial data in Processing,
     and analyze using matlab. I don't feel like it right now.
  */
  while ((timeGone - startTime < 2000)) {
      timeGone = millis();
      Serial.println(String(numPulse));
      Serial.println(String(encoder->read()));
      Input = encoder->read();
      thePID->Compute();
      this->rotate(Output);
  }
  Input = encoder->read();
  thePID->Compute();
  rotate(Output);
  Serial.print("Last : ");
  Serial.println(String(encoder->read()));
  digitalWrite(inputForward, LOW);
  digitalWrite(inputBack, LOW);
}
/** private method to rotate the motor, only ever called by rotateToDegrees
 * param pwmOut - the pwm that dictates how fast the motor driver should output
 *
 */
void EncodedBLDC::rotate(int pwmOut){
  if (pwmOut > 0) {
      digitalWrite(inputForward, HIGH);
      digitalWrite(inputBack, LOW);
      analogWrite(pwmSetSpeedPin, abs(pwmOut));
  } else {
      digitalWrite(inputForward, LOW);
      digitalWrite(inputBack, HIGH);
      analogWrite(pwmSetSpeedPin, abs(pwmOut));
  }
}
