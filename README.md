# arduino-motor-control
@@@Actual library in the MotorLibraries branch, not the master branch@@@ 
To do: 

1. Clean up all the debugging Serial.print()'s. The ones in the PID must stay though because for some reason the algorithm likes the delay's that println() causes... something that needs to be looked into. 

2. The while loop for the PID control needs to run in a time based scenerio: 

while (time < 2seconds) {

  calculatePID();

}

The reason for this is that if we only loop for certain positions:
while (abs(position - desiredPosition) > 5) {
 
  calculatePID();

}

Like above the motor will stop short while the algorithm hasn't been tuned to stop that early. It'll stop looping as the motor overshoots and continues rotation, therefore we need a loop that halts once a certain amount of time has passed and we know that the motor has settled. See the actual loop SailbotEncodedBLDC.cpp for more information.
A function for calculating the correct amount of time needs to be found, and this shouldn't be too hard of a task. Again, the block comment in the file has some ideas about how to go about this, but feel free to approach this problem as seen fit.

The problem with this is that this is not good code and more importantly if the motor needs to rotate a large amount of degrees, the function will stop too short.

3. See comments in the servo library. Feedback would be nice, and some code is wrong. 
