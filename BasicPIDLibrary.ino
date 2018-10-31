/********************************************************
 * PID Basic Example
 * Jay Wilhelm, jwilhelm@ohio.edu, 2018
 ********************************************************/
#include "BasicPIDLibraryClass.h"
#define PIN_INPUT 0
#define PIN_OUTPUT 3

double gSetpoint;
double Kp=1.0, Ki=0.001, Kd=0.001;
BasicPIDLibrary myPID(Kp, Ki, Kd);

void setup()
{
  gSetpoint = 100;
  myPID.SetOutputLimits(-1,1);
  myPID.SetSampleTime(100);
}
double t = 0;
void loop()
{
  static double dOutput=0.0;
  double dInput = analogRead(PIN_INPUT);
  myPID.Compute(gSetpoint, dInput, dOutput);
  analogWrite(PIN_OUTPUT, dOutput);
  delay(100);
}
