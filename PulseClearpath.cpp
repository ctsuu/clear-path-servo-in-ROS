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
#include "Arduino.h"
#include "PulseClearpath.h"

/*		
	This is an internal Function used by StepController to calculate how many pulses to send to each motor.
	It tracks the current command, as well as how many steps have been sent, and calculates how many steps
	to send in the next ISR.
*/
int PulseClearpath::calcSteps()
{  
	if(!_enabled)
		return 0;
 if(_commandX == 0)
{
				_BurstX=0;
}
else
{
			if(_commandX>50)
			{
				_BurstX=50;
				_commandX=_commandX-50;
			}
			else
			{
				_BurstX=_commandX;
				_commandX=0;
			}
}
	// Update accumulated integer position
	if(_direction)
		_absPosition+=_BurstX;
	else
		_absPosition-=_BurstX;
	return _BurstX;

}

/*		
	This is the default constructor.  This intializes the variables.
*/
PulseClearpath::PulseClearpath()
{
  _Apin=0;
  _Bpin=0;
  _Epin=0;
  _Hpin=0;
  _enabled=false;
_commandX=0;
_BurstX=0;
_absPosition=0;
}

/*		
	This is the one pin attach function.  It asociates the passed number, as this motors Step Pin
*/
void PulseClearpath::attach(int BPin)
{
  _Apin=0;
  _Bpin=BPin;
  _Epin=0;
  _Hpin=0;
  pinMode(_Bpin,OUTPUT);
}

/*		
	This is the two pin attach function.  
	It asociates the 1st number, as this motors Direction Pin
	and the 2nd number with the Step Pin
*/
void PulseClearpath::attach(int APin, int BPin)
{
  _Apin=APin;
  _Bpin=BPin;
  _Epin=0;
  _Hpin=0;
  pinMode(_Apin,OUTPUT);
  pinMode(_Bpin,OUTPUT);
}

/*		
	This is the three pin attach function.  
	It asociates the 1st number, as this motors Direction Pin,
	the 2nd number with the Step Pin,
	and the 3rd number with the Enable Pin
*/
void PulseClearpath::attach(int APin, int BPin, int EPin)
{
  _Apin=APin;
  _Bpin=BPin;
  _Epin=EPin;
  _Hpin=0;
  pinMode(_Apin,OUTPUT);
  pinMode(_Bpin,OUTPUT);
  pinMode(_Epin,OUTPUT);
}

/*		
	This is the four pin attach function.  
	It asociates the 1st number, as this motors Direction Pin,
	the 2nd number with the Step Pin,
	the 3rd number with the Enable Pin,
	and the 4th number as the HLFB Pin
*/
void PulseClearpath::attach(int APin, int BPin, int EPin, int HPin)
{
  _Apin=APin;
  _Bpin=BPin;
  _Epin=EPin;
  _Hpin=HPin;
  pinMode(_Apin,OUTPUT);
  pinMode(_Bpin,OUTPUT);
  pinMode(_Epin,OUTPUT);
  pinMode(_Hpin,INPUT_PULLUP);
}

/*		
	This function clears the current move, and puts the motor in a
	move idle state, without disableing it, or clearing the position.

	This may cause an abrupt stop.
*/
void PulseClearpath::stopMove()
{ 
	 _BurstX=0;
     _commandX=0;
}

/*		
	This function returns the pin number for the Step Pin.
	It is mainly used by the StepController Class  
*/
int PulseClearpath::getDirPin()
{
  return _Bpin;
}

/*		
	This function commands a directional move
	The move cannot be longer than 2,000,000 counts
	If there is a current move, it will NOT be overwritten

	The function will return true if the move was accepted
*/
boolean PulseClearpath::move(long dist)
{
  if(_commandX==0)
  {
	  if(dist<0)
	  {
		  if(_Apin!=0)
		  {
			  digitalWrite(_Apin,HIGH);
			  _direction=true;
		  }
		  _commandX=-dist;
	  }
	  else
	  {
		  if(_Apin!=0)
		  {
			  digitalWrite(_Apin,LOW);
			  _direction=false;
		  }
		_commandX=dist;
	  }
	  return true;
  }
  else
	  return false;

}

/*		
	This function returns the current command
*/
long PulseClearpath::getCommand()
{
	return _commandX;
}

/*		
	This is a debugging function
*/
long PulseClearpath::test()
{
	return _Apin;
}

/*		
	This function returns true if there is no current command
*/
boolean PulseClearpath::commandDone()
{

	if(_commandX==0)
		return true;
	else
		return false;
}

/*		
	This function toggles the direction Bit
*/
void PulseClearpath::Direction(uint8_t dir)
{

	if(_Apin!=0)
		digitalWrite(_Apin,dir);
	if(dir==HIGH)
		_direction=true;
	if(dir==LOW)
		_direction=false;
}

/*		
	This function disable the motor
*/
void PulseClearpath::disable()
{

	if(_Epin!=0)
		digitalWrite(_Epin,LOW);
	_BurstX=0;
	_enabled=false;
    _commandX=0;
	_absPosition=0;
}

/*		
	This function enables the motor
*/
void PulseClearpath::enable()
{

	if(_Epin!=0)
		digitalWrite(_Epin,HIGH);
	_absPosition=0;
	_enabled=true;
}

/*		
	This function returns the absolute commanded position
*/
long PulseClearpath::getCommandedPosition()
{
		return _absPosition;
}

/*		
	This function returns the value of the HLFB Pin
*/
boolean PulseClearpath::readHLFB()
{
	if(_Epin!=0)
		return digitalRead(_Hpin);
	else
		return false;
}

/*		
	This function writes the enable pin low for the input number of ms in order to select the alternate speed from MSP
*/
void PulseClearpath::altSpeed(int time)
{
	if(_Epin!=0)
	{
		digitalWrite(_Epin, LOW);
		delay(time+1);
		digitalWrite(_Epin, HIGH);

	}
}