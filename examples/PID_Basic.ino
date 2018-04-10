/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include "BasicPIDLibrary.h"

#define PIN_INPUT 0
#define PIN_OUTPUT 3

//Define Variables we'll be connecting to
double Setpoint, Input, Output;
double *output_pointer = &Output;

//Specify the links and initial tuning parameters
double Kp=2, Ki=5, Kd=1;
BasicPIDLibrary myPID(Kp, Ki, Kd);

void setup()
{
  Serial.begin(115200);
  //initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  Setpoint = 100;
  myPID.SetOutputLimits(-1000,1000);

}

void loop()
{
  Input = analogRead(PIN_INPUT);

  myPID.Compute(Setpoint, Input, output_pointer);
  analogWrite(PIN_OUTPUT, Output);
}