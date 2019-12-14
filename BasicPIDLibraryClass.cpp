/**********************************************************************************************
 * Basic PID Library - Version 1.0
 * by Jay Wilhelm <jwilhelm@ohio.edu>
 * original by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 *
 * This Library is licensed under the MIT License
 **********************************************************************************************/

#include "Arduino.h"


#include "BasicPIDLibraryClass.h"

/*Constructor (...)*********************************************************
 *    The parameters specified here are those for for which we can't set up
 *    reliable defaults, so we need to have the user set them.
 ***************************************************************************/
BasicPIDLibrary::BasicPIDLibrary(double Kp, double Ki, double Kd)
{
    BasicPIDLibrary::SetOutputLimits(0, 255);	//default output limit corresponds to
												//the arduino pwm limits
    this->mSampleTime = 50;							//default Controller Sample Time is 0.1 seconds
    BasicPIDLibrary::SetTunings(Kp, Ki, Kd);
    this->mLastTime = millis()-mSampleTime;
	  EnableController();
}


/* Compute() **********************************************************************
 *     This, as they say, is where the magic happens.  this function should be called
 *   every time "void loop()" executes.  the function will decide for itself whether a new
 *   BasicPIDLibrary Output needs to be computed.  returns true when the output is computed,
 *   false when nothing has been done.
 **********************************************************************************/
bool BasicPIDLibrary::Compute(DECIMAL iSetpoint,DECIMAL iInput,DECIMAL &pOutput)
{
	if(!this->mEnabled)
		return false;
	unsigned long now = millis();
	unsigned long timeChange = (now - mLastTime);
	if(timeChange >= this->mSampleTime) //only run at a set controller sample rate
	{	
		DECIMAL error = iSetpoint - iInput;
		DECIMAL dInput = (error - mLastInput) / (this->mSampleTime/1000.0);
		this->mOutputSum += (error- mLastInput)*(this->mSampleTime/1000.0);

		DECIMAL output;
		output = (kp * error) +  (this->mOutputSum * ki) + (dInput * kd); //dt assumed in ki and kd

		if(output > mOutMax) 
			pOutput = mOutMax;
		else if(output < mOutMin) 
			pOutput = mOutMin;
		else{
			pOutput = output;	//assign output value to the pointer
		}

		//Remember some variables for next time 
		this->mLastInput = error;
		this->mLastTime = now;
		return true;
	}
	else 
		return false;
}


/* SetTunings(...)*************************************************************
 * Set Tunings using the last-rembered POn setting
 ******************************************************************************/
void BasicPIDLibrary::SetTunings(DECIMAL iKp, DECIMAL iKi, DECIMAL iKd){
   this->kp = iKp;
   this->ki = iKi;// * (DECIMAL)this->mSampleTime/1000.0;
   this->kd = iKd;// / (DECIMAL)this->mSampleTime/1000.0;
}

/* SetSampleTime(...) *********************************************************
 * sets the period, in Milliseconds, at which the calculation is performed
 ******************************************************************************/
void BasicPIDLibrary::SetSampleTime(int NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      //need ratio to keep relative to seconds
	    /*DECIMAL ratio  = (DECIMAL)NewSampleTime
                      / (DECIMAL)this->mSampleTime;
      this->ki *= ratio;
      this->kd /= ratio;*/
      this->mSampleTime = (unsigned long)NewSampleTime;
   }
}

/* SetOutputLimits(...)****************************************************
 *     This function will be used far more often than SetInputLimits.  while
 *  the input to the controller will generally be in the 0-1023 range (which is
 *  the default already,)  the output will be a little different.  maybe they'll
 *  be doing a time window and will need 0-8000 or something.  or maybe they'll
 *  want to clamp it from 0-125.  who knows.  at any rate, that can all be done
 *  here.
 **************************************************************************/
void BasicPIDLibrary::SetOutputLimits(DECIMAL Min, DECIMAL Max)
{
   if(Min >= Max) 
	   return;
   this->mOutMin = Min;
   this->mOutMax = Max;
}

void BasicPIDLibrary::DisableController(void)
{
	this->mEnabled = false;
}
void BasicPIDLibrary::EnableController(void)
{
	this->mEnabled = true;
	this->mOutputSum = 0.0;
	this->mLastInput = 0.0;
	this->mLastTime = 0;
}
