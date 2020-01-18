#include "../Stepper/src/Stepper.h"
#include "SailbotStepper.h"

/** A class for controlling generalized stepper setMotorPins
 *   Andy Heller-Jones 1.10.2019
 * param pins - the pins of the setMotorPins
 * param numSteps - the number of steps the stepper takes per rotation
 * consult motor specific data sheets to find this value
 */
StepperMotor::StepperMotor(int numStepsPerRotation,
    int pin1, int pin2, int pin3, int pin4) {
  Serial.begin(9600);
  theStepper = new Stepper(numStepsPerRotation, pin1, pin2, pin3, pin4);
  this->numStepsPerRotation = numStepsPerRotation;
  Serial.println("Init");
  currentDegrees = 0;
  speed = 10;
  theStepper->setSpeed(speed);
}
/** change the speed
 * param newSpeed - the new speed to turn the steper at
 */

void StepperMotor::changeSpeedTo(int newSpeed) {
  theStepper->setSpeed(newSpeed);
  speed = newSpeed;
}
/** rotates the stepper a certain amount of degrees using the
 * numStepsPerRotation.
 *
 */
void StepperMotor::rotateToDegrees(int numDeg){
  Serial.println(numStepsPerRotation);
  Serial.println((numDeg/360.0));
  int numStepsToMove = (int) numStepsPerRotation * (numDeg / 360.0);
  theStepper->step(numStepsToMove);
  Serial.println(numStepsToMove);
  currentDegrees += numDeg;
}
