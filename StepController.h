/*
  StepController.h - Interrupt driven library for controlling Clearpath motors using an Arduino- Version 1
  Teknic 2014 Brendan Flosenzier

  This library is free software; you can redistribute it and/or
  modify it.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* 
  
  A StepController is activated by creating an instance of the StepController class, and passing the constructor references to the motors it will control.
  The motors are pulsed in the background according to move information stored within each motor.
  Only motors of type; PulseClearpath, or substype StepClearpath, may be used.

  There can only be one instance of Step Controller at any time.

  This class uses Timer2, so other functions and classes which use timer 2 will not work correctly ie: tone(), MsTimer2() etc.

  The ISR is set to 2KHz, nominally

  Note: Each attached motor must have its direction/B pin connected to one of pins 8-13

  other devices can be connected to pins 8-13 as well

  The methods are:

   StepController - Class for manipulating Clearpath motors connected to Arduino up to 6 motors may be controlled.

   HandleInterupt(pin )  - Attaches a servo motor to an i/o pin.
   
 
   Start(time)     - gets Direction pins for all connected motors (make sure all motors have been attached before this is called
						Configures the ISR to run at 2kHz, time must be 249

   Stop() - disables the ISR in this class
   
 */
#ifndef StepController_h
#define StepController_h

#include "Arduino.h"
#include "StepClearpath.h"

class StepController
{
  public:
  StepController(PulseClearpath* motor1);
  StepController(PulseClearpath* motor1, PulseClearpath* motor2);
  StepController(PulseClearpath* motor1, PulseClearpath* motor2, PulseClearpath* motor3);
  StepController(PulseClearpath* motor1, PulseClearpath* motor2, PulseClearpath* motor3, PulseClearpath* motor4);
  StepController(PulseClearpath* motor1, PulseClearpath* motor2, PulseClearpath* motor3, PulseClearpath* motor4, PulseClearpath* motor5);
  StepController(PulseClearpath* motor1, PulseClearpath* motor2, PulseClearpath* motor3, PulseClearpath* motor4, PulseClearpath* motor5, PulseClearpath* motor6);
  void Start(int time);
  void Stop();
  int getsum();


};
#endif
