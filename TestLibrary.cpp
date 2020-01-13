#include "TestLibrary.h"
#include "Stepper.h"
//#include "PID.h"
#include "../PID/PID_v1.h"
#include "../Encoder/Encoder.h"
//Constructor
TestLib::TestLib(bool displayMessage){
    Serial.begin(9600);
    if (displayMessage) { 
        Serial.println("Initializing...");
    }
    /*
    * Encoder declarations
    */
    myEnc = new Encoder(2,3);
    /*
    * Stepper delcarations
    */
    myStep = new Stepper(2038, 8, 9, 10, 11);
    myStep->setSpeed(10);
    
    /*
    * PID declarations
    */
    Kp= 0; 
    Ki= 0; 
    Kd= 0;
    myPID = new PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
    myPID->SetMode(AUTOMATIC); 
    //Adjust PID values
    myPID->SetTunings(Kp, Ki, Kd);
    myPID->SetSampleTime(1);
    myPID->SetOutputLimits(-maxSpeed, maxSpeed);
    Serial.println("Ended Initializing");
}

void TestLib::setTunings(double newKp, double newKi, double newKd) {
    myPID->SetTunings(newKp, newKi, newKd);    
}


void TestLib::computePID(){
    Serial.println(Ki);
    Serial.print("CurrentPulse: ");
    Serial.println(myEnc->read());
    if (Serial.available() > 0) { 
        inputDeg = Serial.parseInt();
        Serial.read();
        if (abs(inputDeg) > 360) { 
            Serial.println("Input degree's must be less than 360*");
        }
    
        Serial.println(inputDeg);
        Serial.print("Changing numPulse to: ");
        numPulse = map(inputDeg, -360, 360, -1632, 1632) + myEnc->read();
        Serial.println(String(numPulse));
    }
    Serial.print("Go to pulse: ");
    Serial.println(String(numPulse));
    
    Setpoint = numPulse;
    myPID->Compute();
        Input = myEnc->read();
        myPID->Compute();
        rotate(Output);
}

void TestLib::computePID(int inputDeg) { 
    
    numPulse = map(inputDeg, -360, 360, -1632, 1632) + myEnc->read();
    Serial.println("Num pulse to move:");
    Serial.println(String(numPulse));
    Setpoint = numPulse;
    myPID->Compute();
    while (abs(Output) > 1) {
        Serial.println("CurrentPulse: ");
        int readVal = myEnc->read();
        Serial.println(String(readVal));
        Input = myEnc->read();
        myPID->Compute();
        rotate(Output);
    }
    
}


void TestLib::rotate(int pwmOut){
    if (pwmOut > 0) { 
        digitalWrite(inputForward, HIGH);
        digitalWrite(inputBack, LOW);
        analogWrite(pwmSetSpeedPin, abs(pwmOut));
  } else if (pwmOut < 0) { 
        digitalWrite(inputForward, LOW);
        digitalWrite(inputBack, HIGH);
        analogWrite(pwmSetSpeedPin, abs(pwmOut));
  } else if (abs(pwmOut) < 30) {
        //digitalWrite(inputForward, LOW);
        //digitalWrite(inputBack, LOW);
  }
  

    
}


void TestLib::moveNumSteps(int numSteps) {
    myStep->step(numSteps);
}
