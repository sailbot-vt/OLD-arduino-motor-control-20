#include "../PololuMaestro/PololuMaestro.h"
#include "SailbotServo.h"
#include "../SoftwareSerial/src/SoftwareSerial.h"

/** A class for controlling servos using the Pololu MicroMaestro
 * Requires: Arduino, Micromaestro, Digital Servos, optional: 5-12V power supply
 * The class works as follows: The Maestro platform needs an object to be able
 * to work from. This object is instantiated with a Stream - such as a serial
 * port. In this instance a SoftwareSerial port is made which is just a
 * Serial port you can make in lieu of using the arduino's hardware serial port.
 * The MicroMaestro object can then be used to one's heart's content. This Class
 * Just simplifies things a little bit. You can also just use the Maestro
 * on it's own using the getter method, getMaestro(), at the bottom.
 @param softPinRx - the Rx pin of the SoftwareSerial.
 @param softPinTx - the Tx pin of the SoftwareSerial.
 * Rx of the arduino goes to the maestro's Tx, Tx to the maestro's Rx
 * When choose Tx & Rx, try not to use dedicated
 * hardware serial pins, pwm pins, or hardware interrupt enabled pins
 * as the need to be preserved, however they do work if needed
 *
 * See #TODO's for rotateToDegrees(), getMaestro(), setSpeed(),
 * setAcceleration()
 * Some rotational reporting might be cool. Literally just return the
 * current degrees of a channel.
 */
SailbotServo::SailbotServo(int softPinRx, int softPinTx) {
  softSerial = new SoftwareSerial (softPinRx, softPinTx);
  softSerial->begin(9600);
  maestro = new MicroMaestro(*softSerial);
}
/** Sets a certain channel's servo's speed.
 * Gives no feed back if the channel num is out of bounds.
 @param channelNum - the channel Number to assign the speed to.
 @param speed - the speed the servo will move at. This value could be researched
 * and given a more concrete definition. #TODO 4
 */
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
 @param - channelNum - the channelNumber to use.
 @param minTarget - sets the minimum Target value for the channel. Defaults to
 4000.
 @param maxTarget - sets the maximum target value for the channel. Defaults to
 8000.
 */
void SailbotServo::setMinMaxTargets(
  int channelNum, int minTarget, int maxTarget) {
    minTargets[channelNum] = minTarget;
    maxTargets[channelNum] = maxTarget;
}

/** Rotates using specific degrees by mapping the input degrees to
 * the correct target value... the mapping isn't right
 * but shit I'm typing out comments right now not fixing code. #TODO 1
 @param channelNum - the specific channel you wish to use
 @param deg - the degree number to go to.
 */
void SailbotServo::rotateToDegrees(int channelNum, int deg) {
  int newTarget = map(deg, minDegrees[channelNum], maxDegrees[channelNum],
                      minTargets[channelNum], maxTargets[channelNum]);

  maestro->setTarget(channelNum, newTarget);
}

/** Rotates using the specific target values instead of degrees.
 @param channelNum - the channel number you wish to use.
 @param newTarget - the new target value you wish to set the servo to.
 *
 */
void SailbotServo::rotateToTarget(int channelNum, int newTarget) {
  maestro->setTarget(channelNum, newTarget);

/** Returns the maestro pointer so you can use the rest of the methods in the
 * MicroMaestro lib. Dunno if this is actually usable.
 @return returns the maestro pointer.
 * #TODO 2
 */
MicroMaestro * SailbotServo::getMaestro() {
  return maestro;
}
