/*
  PulseClearpath.h - Library for interfacing with Clearpath motors using an Arduino- Version 1
  Teknic 2014 Brendan Flosenzier

  This library is free software; you can redistribute it and/or
  modify it.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* 
  
  A PulseClearpath is activated by creating an instance of the PulseClearpath class. 

  There can several instances of PulseClearpath however each must be attached to different pins.

  This class is used in conjuntion with the StepController class, and is related to the StepClearpath class.
  All functions available in this class, are available in the StepClearpath.
  
  Note: Each attached motor must have its direction/B pin connected to one of pins 8-13 in order to work with
		The StepController object.  Other devices can be connected to pins 8-13 as well

  The functions for a pulse motor are:

   PulseClearpath - default constructor for initializing the motor

   attach() - Attachs pins to this motor, and declares them as input/outputs

   stopMove()  - Interupts the current move, the motor may abruptly stop
 
   calcSteps()     - Internal Function used by StepController to calculate how many pulses to send to each motor

   getDirPin() - Returns the pin number of the attached Direction Pin

   move() - sets the maximum veloctiy

   getCommand() - sets the acceleration

   Direction() - Sets the motor's direction bit
   
   commandDone() - returns wheter or not there is a valid current command

   disable() - disables the motor

   enable() - enables the motor

   getCommandedPosition() - Returns the absolute cmomanded position where the position on enable=0

   readHLFB() - Returns the value of the HLFB Pin

   altSpeed() - Turns off the Enable pin for a short time in order to select the alternate speed from MSP
 */
#ifndef PulseClearpath_h
#define PulseClearpath_h
#include "Arduino.h"
class PulseClearpath
{
  public:
  PulseClearpath();
  void attach(int);
  void attach(int, int);
  void attach(int, int, int);
  void attach(int, int, int, int);
  virtual void stopMove();
  virtual int calcSteps();
  int getDirPin();
  boolean move(long);
  long getCommand();
  long test();
  void Direction(uint8_t);
  virtual boolean commandDone();
  virtual void disable();
  void enable();
  long getCommandedPosition();
  boolean readHLFB();
  void altSpeed(int time);

protected:

uint8_t _Apin;
uint8_t _Bpin;
uint8_t _Epin;
uint8_t _Hpin;
 boolean _enabled;
 long _commandX;
 boolean _direction;
 uint8_t _BurstX;
long _absPosition;
};
#endif