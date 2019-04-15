/********************************************************
 * PID Basic Example
 * Jay Wilhelm, jwilhelm@ohio.edu, 2019
 ********************************************************/
#include "BasicPIDLibraryClass.h"
#include "math.h"

double gSetpoint=-1.0;
double Kp=0.5, Ki=0.0, Kd=0.0;

BasicPIDLibrary myPID(Kp, Ki, Kd);
#define tdelay 5

void setup()
{
  Serial.begin(115200);
  myPID.SetOutputLimits(-1,1);
  myPID.SetSampleTime(tdelay);
}
void loop()
{
  static double simsys = 0;
  double dOutput = 0.0;
  double dInput = simsys;
  myPID.Compute(gSetpoint, dInput, dOutput);
  double t = millis() / 1000.0;
  double dt = 1.0/tdelay;
  simsys = simsys + dOutput*exp(-dOutput)*dt;
  /*Serial.print(t);
  Serial.print("\t");
  Serial.print(dt);
  Serial.print("\t");
  Serial.print(gSetpoint);
  Serial.print("\t");
  Serial.print(gSetpoint-dInput);
  Serial.print("\t");
  Serial.print(dInput);
  Serial.print("\t");
  Serial.print(dOutput);
  Serial.print("\t");*/
  Serial.println(simsys*100);
  delay(tdelay);

  static int otime = millis()+5000;
  if(millis() > otime)
  { 
    gSetpoint = -gSetpoint;
    otime = millis()+5000;
  }
}
