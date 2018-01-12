/*
  StepClearpath.h - Library for interfacing with Clearpath motors using an Arduino- Version 1
  Teknic 2014 Brendan Flosenzier

  This library is free software; you can redistribute it and/or
  modify it.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* 
  
  A StepClearpath is activated by creating an instance of the StepClearpath class.  
  The motors are pulsed in the background according to move information stored within each motor.
  Only motors of type; PulseClearpath, or substype StepClearpath, may be used.

  There can several instances of StepClearpath however each must be attached to different pins.

  This class is used in conjuntion with the StepController class, and is related to the PulseClearpath class.
  All functions available in the PulseClearpath class, are available in addition to Functions defined here.
  
  Note: Each attached motor must have its direction/B pin connected to one of pins 8-13 in order to work with
		The StepController object.  Other devices can be connected to pins 8-13 as well

  The unique functions for a step motor are:

   StepClearpath - default constructor for initializing the motor

   stopMove()  - Interupts the current move, the motor may abruptly stop
 
   calcSteps()     - Internal Function used by StepController to calculate how many pulses to send to each motor

   setMaxVel() - sets the maximum veloctiy

   setMaxAccel() - sets the acceleration

   commandDone() - returns wheter or not there is a valid current command

   disable() - disables the motor
   
 */
#ifndef StepClearpath_h
#define StepClearpath_h
#include "Arduino.h"
#include "PulseClearpath.h"		//The PulseClearpath Library must be included
class StepClearpath: public PulseClearpath
{
  public:
  StepClearpath();
  void stopMove();
  int calcSteps();
  void setMaxVel(long); 
  void setMaxAccel(long);
  long Movestate();
  long test();
  boolean commandDone();
  void disable();

private:



// All of the position, velocity and acceleration parameters are signed and in Q24.8,
// with all arithmetic performed in fixed point.

 long _VMX;					// Velocity limit
 long _AXMX;					// Acceleration limit
 long _PX;					// Current position
 long _OPX;				// Accumulated integer position
 long _VX;					// Current velocity
 long _AX;					// Current acceleration
 long _TX;					// Current time
 long _TX1;				// End of ramp up time
 long _TX2;				// Beginning of phase 2 time
 long _TX3;				// Beginning of ramp down time
 long _TAUX;					// Integer burst value
 boolean _flag;
 long _AXS;					// Signed acceleration limit
 long _ZX;						// Move length in Q24.8
 long _ZX2;	
 int _moveStateX;
uint8_t _bitshift;

};
#endif
