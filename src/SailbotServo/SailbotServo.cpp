#include "../PololuMaestro/PololuMaestro.h"
#include "SailbotServo.h"
#include "../SoftwareSerial/src/SoftwareSerial.h"

/** A class for controlling servos using the Pololu MicroMaestro
 * param softPin's - the Tx and Rx pins used for serial commmunication
 * to the maestro. Can be any pins, try not to use dedicated
 * hardware serial pins, pwm pins, or hardware interrupt enabled pins
 * as the need to be preserved, however they do work if needed
 *
 * See #TODO's for rotateToDegrees() & getMaestro().
 * Some rotational reporting might be cool. Literally just return the
 * current degrees of a channel.
 */
SailbotServo::SailbotServo(int softPinRx, int softPinTx) {
  softSerial = new SoftwareSerial (softPinRx, softPinTx);
  softSerial->begin(9600);
  maestro = new MicroMaestro(*softSerial);
}

void SailbotServo::setServoSpeed(int channelNum, int speed) {
  maestro->setSpeed(channelNum, speed);
}

void SailbotServo::setServoAcceleration(int channelNum, int accel) {
  maestro->setAcceleration(channelNum, accel);
}
/** changes the max target values for a certain servo
 * Use the maestro control center to find these values
 * For example, certain servos only go to 178 or 179 degress instead of 180
 * You need to test these bounds in control center.
 */
void SailbotServo::setMinMaxTargets(
  int channelNum, int minTarget, int maxTarget) {
    minTargets[channelNum] = minTarget;
    maxTargets[channelNum] = maxTarget;
}

/** Rotates using specific degrees by mapping the input degrees to
 * the correct target value... the mapping isn't right
 * but shit I'm typing out comments right now not fixing code. #TODO 1
 *
 */
void SailbotServo::rotateToDegrees(int channelNum, int deg) {
  int newTarget = map(deg, minDegrees[channelNum], maxDegrees[channelNum],
                      minTargets[channelNum], maxTargets[channelNum]);

  maestro->setTarget(channelNum, newTarget);
}

/** Rotates using the specific target values instead of degrees.
 *
 */
void SailbotServo::rotateToTarget(int channelNum, int newTarget) {
  maestro->setTarget(channelNum, newTarget);

/** Returns the maestro pointer so you can use the rest of the methods in the
 * MicroMaestro lib. Dunno if this is actually usable.
 * #TODO 2
 */
MicroMaestro * SailbotServo::getMaestro() {
  return maestro;
}
