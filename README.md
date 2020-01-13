# arduino-motor-control
A library for controlling various motors, stepper, encoded bldc, servo.

Library is a WIP. Stepper works fine PID works well. PID must be looped in the library, not from the void loop() for latency issues. Also PID must use a certain time constant relative to the motor speed, transients will push the motor off the correct value if only looped until a certain operating point. Some sort of time constant with respect to the maximum speed, voltage applied, and degrees requested must be found in order to make the motor run as efficiently as possible. Other than that the PID does settle within about 0 - 8 steps which is great and about as close as our motor will allow for fine tuning. 

Still need to organize and clean up, include maestro control for servos, create abstract motor base class and then implement each type of motor into its own class. 
