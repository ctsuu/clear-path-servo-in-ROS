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
#include "Arduino.h"
#include "StepClearpath.h"


/*		
	This is an internal Function used by StepController to calculate how many pulses to send to each motor.
	It tracks the current command, as well as how many steps have been sent, and calculates how many steps
	to send in the next ISR.
*/
int StepClearpath::calcSteps()
{  
	_TX++;//increment time
	if(!_enabled)
		return 0;

	// Process current move state.
	switch(_moveStateX){
		case 3: // IdleState state, executed only once.

 if(_commandX == 0) //If no/finished command/, do nothing set everything to 0
{
                _PX=0;
                _VX=0;
                _OPX=0;
                _TX=0;
                _TX1=0;
                _TX2=0;
                _TX3=0;
				_BurstX=0;
}
else
{
			// Compute Move parameters
			_ZX = _commandX<<_bitshift;
			_ZX2 = abs(_ZX>>1);
			if(_ZX > 0)
				_AXS = _AXMX;
			else
				_AXS = -_AXMX;
			_AX = _AXS;
			// Do immediate move if half move length <= maximum acceleration.
			if(_ZX2 <= _AXMX) {
				_AX = 0;
				_VX = 0;
				_PX = _ZX;
				_moveStateX = 3;	//Set to Move Idle
                _commandX=0;		//Zero command
				break;
			}
			// Otherwise, execute move and go to Phase1
			_PX = _PX + _VX;// + (_AX>>1);
	                _VX = _VX + _AX;
			_moveStateX = 1;
}
			break;

		case 1:		//Phase 1 first half of move
			
			// Execute move
			_PX = _PX + _VX + (_AX>>1);
	                _VX = _VX + _AX;

			// Check position.
			if(abs(_PX) >= _ZX2) {
				// If half move reached, compute time parameters and go to PXhase2

				if(_flag)		//This makes sure you go one step past half in order to make sure Phase 2 goes well
				{
				if(_TX1 == 0) {
					_TX1 = _TX;
				}
				if(_TX2 == 0) {
					_TX2 = _TX;
				}
				_AX = -_AX;					//Set deccelleration
				_TX3 = (_TX2<<1) - _TX1;	//compute time params
				_TAUX = _TX2<<1;
				_moveStateX = 2;			//Start Phase 2
				}
				_flag=true;
				
			}
			else {
				// Otherwise, check velocity.
				if(labs(_VX) >= _VMX) {
					// If maximum velocity reached, compute TX1 and set AX = 0, and _VX=_VMX.
					if(_TX1 == 0) {
						_AX = 0;
						_TX1 = _TX;
						if(_VX > 0)
                          _VX=_VMX;
						else
                          _VX=-_VMX;
					}
				}
			}
			break;

		case 2:		//Phase 2 2nd half of move
			// Execute move
			_PX = _PX + _VX + (_AX>>1);
	                _VX = _VX + _AX;

			// Check time.
			if(_TX >= _TX3) {
				// If beyond TX3, wait for done condition.
				_AX = -_AXS;
				if((_TX > _TAUX) || (labs(_PX) > labs(_ZX)) || (_VX*_AX > 0)) {
            	// If done, enforce final position.
					_AX = 0;
					_VX = 0;
					_PX = _ZX;
					_moveStateX = 3;
					_commandX=0;
				}
			}
			break;
	}
	// Compute burst value
	_BurstX = (_PX - _OPX)>>_bitshift;
	// Update accumulated integer position
	_OPX += (long)(_BurstX)<<_bitshift;

	//check which direction, and incement absPosition
	if(_direction)
		_absPosition+=_BurstX;
	else
		_absPosition-=_BurstX;
	return _BurstX;

}


/*		
	This is the default constructor.  This intializes the variables.
*/
StepClearpath::StepClearpath()
{
  _moveStateX=3;
  _Apin=0;
  _Bpin=0;
  _Epin=0;
  _Hpin=0;
  _enabled=false;
_VMX=0;					
_AXMX=0;
_PX=0;				
_OPX=0;				
 _VX=0;				
_AX=0;					
_TX=0;					
_TX1=0;				
 _TX2=0;				
_TX3=0;			
_TAUX=0;					
_flag=0;
_AXS=0;					
 _ZX=0;				
_ZX2=0;					
_commandX=0;
_bitshift=10;
_BurstX=0;
_absPosition=0;
}

/*		
	This function clears the current move, and puts the motor in a
	move idle state, without disableing it, or clearing the position.

	This may cause an abrupt stop.
*/
void StepClearpath::stopMove()
{
	_PX=0;
	_VX=0;
     _OPX=0;
     _TX=0;
     _TX1=0;
     _TX2=0;
     _TX3=0;
	 _BurstX=0;
	 _moveStateX = 3;
     _commandX=0;
}

/*		
	This function sets the velocity in Counts/sec assuming the ISR frequency is 2kHz.
	The maximum value for velMax is 100,000, the minimum is 2
*/
void StepClearpath::setMaxVel(long velMax)
{
	int n = velMax/2000;
	if(n<51)
		_VMX=(velMax*(1<<_bitshift))/2000;
	else
		_VMX=50*(1<<_bitshift);

}
/*		
	This function sets the acceleration in Counts/sec/sec assuming the ISR frequency is 2kHz.
	The maximum value for accelMax is 2,000,000, the minimum is 4,000
*/
void StepClearpath::setMaxAccel(long accelMax)
{
  _AXMX=(accelMax*(1<<_bitshift))/4000000;
}
/*		
	This function is a debugging function
*/
long StepClearpath::Movestate()
{
	return _moveStateX;
}
/*		
	This function is a debugging function
*/
long StepClearpath::test()
{
	return _moveStateX;
}

/*		
	This function returns true if there is no current command
	It returns false if there is a current command
*/
boolean StepClearpath::commandDone()
{

	if(_commandX==0)
		return true;
	else
		return false;
}

/*		
	This function returns zeros out the current command, and digitally writes the enable pin LOW
	If the motor was not attached with an enable pin, then it just zeros the command
*/
void StepClearpath::disable()
{

	if(_Epin!=0)
		digitalWrite(_Epin,LOW);
	_PX=0;
    _enabled=false;
	_VX=0;
     _OPX=0;
     _TX=0;
     _TX1=0;
     _TX2=0;
     _TX3=0;
	 _BurstX=0;
	 _moveStateX = 3;
     _commandX=0;
}
