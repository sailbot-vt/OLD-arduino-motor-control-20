#include "SailbotEncodedBLDC.h"
#include "../PID/PID_v1.h"
#include "../Encoder/Encoder.h"

/** Class for controlling a BLDC motor w/ Encoder using PID control.
 * REQUIRED: Arduino, L298N motor driver, BLDC motor w/ encoder, regulated 12V
 * power supply
 @param initialEncoderA - the first Encoder pin, must be interrupt enabled
 * for the arduino uno, use pin 2
 @param initialEncoderB - the second Encoder pin, must be interrupt enabled
 * for the arduino uno, use pin 3
 * Connect initialEncoderA to the 'A' pin of the motor, and initialEncoderB to
 * 'B'
 @param pwmSetSpeedPin - This pin connects to EnA of the L298N. This is the
 * enable pin for the driver. The PWM value sent to this pin will dictate the
 * speed at which the motor connected to the 1st output of the driver
 * will run.
 @param forwardPin - This pin connects to IN1 of the driver. It is apart of the
 * driver's directional control.
 @param backPin - This pin connects to IN2 of the driver. It is apart of the
 * driver's directional control.
 * When forwardPin is HIGH & backPin is LOW, the motor is supplied a positive
 * voltage. forwardPin = LOW & backPin = HIGH, a negative voltage.
 * If both pins are low the motor recieves no voltage. Not sure what happens for
 * both HIGH
 * See #TODO's in rotateToDegrees
 */
EncodedBLDC::EncodedBLDC(
    int initialEncoderA,
      int initialEncoderB, int pwmSetSpeedPin, int forwardPin,
        int backPin) {
  // Motor encoder, uses standard Encoder library to count pulses made by
  // The motor's encoder. initialEncoderA & initialEncoderB must be interrupts
  encoder = new Encoder(initialEncoderA, initialEncoderB);
  //PID object, uses the PID library in the github. 
  thePID  = new PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
  thePID->SetMode(AUTOMATIC);
  thePID->SetTunings(Kp, Ki, Kd);
  thePID->SetSampleTime(1);
  thePID->SetOutputLimits(-maxSpeed, maxSpeed);
}
/** Sets the tunings for the PID. Must be reconfigured for ANY change to set up
 * by default these are set to 0 and must be changed in order to do anything
 * previously working non-load values are 7, 0, .05
 @param newKp - the new proportional tuning to be applied to the algorithm
 @param newKi - the new integral tuning to be applied to the algorithm
 @param newKd - the new derivative tuning to be applied to the algorithm
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
 *
 * The PID algorithm takes in a Setpoint (numDeg) and maps the number of degrees
 * To the number of pulses (from the encoder) that need to be counted for the
 * motor to be in the correct position relative to the new Setpoint
 * The PID thus calculates an Output, a PWM signal to apply to the motor, and
 * then adjusts the PWM value based on the feedback by the encoder. When the
 * motor gets close to the correct position it lessens the Output, even making
 * it negative if the encoder shows that the motor has overshot the Setpoint.
 * The while loop runs this feedback loop for a set amount of time, and the m
 * otor generally settles within that alloted time. See the block comment to
 * improve.
 @param numDeg - the number of degrees to turn the BLDC. This value
 can be anything... position/negative, integer/real. It also can be
 greater than 360.

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
 @param pwmOut - the pwm that dictates how fast the motor driver should rotate
 * The digitalWrite()'s dictate to the driver the correct orientation to apply
 * voltage to the motor.
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
