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
#include "Arduino.h"
#include "StepClearpath.h"
#include "StepController.h"


// Declare Variables used in this class
// They aren't private because the ISR needs to access them
PulseClearpath* _motors[6];					//6 clearpath motor pointers for up to 6 digital pins in PORTB
uint8_t _numAxis=0;							//this keeps track of how many pointers are active
uint8_t _BurstSteps[6]={0, 0, 0, 0, 0, 0};	//this is the container for the motors to dump however many steps need to be pulsed
uint8_t _pins[6]={0, 0, 0, 0, 0, 0};		//This holds the port address (Binary) for each motors Step Pin
uint8_t _SUMPINS=0;							//This holds the Binary Sum of all active motor Step Pin addresses
uint8_t _OutputBits;						//this is the container to write to output PORTB
boolean _flag=false;						//This is the flag to show when to finish pulsing the motors




//This is the Interupt Service Routine.
// It asks each motor how many steps to send, and then pulses to PORTB
ISR(TIMER2_COMPA_vect)
{  
	//Prevent Interupts
	cli();

//Turn on pin 2 to see how long the ISR takes
  digitalWrite(2,HIGH);

//Poll all axes to fill BurstSteps[]
  for(int i=0;i<_numAxis;i++)
  {
	  _BurstSteps[i]=_motors[i]->calcSteps();
  }
	  

	  
//loop through BurstSteps decrementing each value to 0	  
do
{

	 _flag=false;

 _OutputBits = PORTB;	//Read the port

if(_BurstSteps[0] && _BurstSteps[0]--)	//Assume at least one axis is active, and check/decrement BurstSteps
{
		_flag=true;
_OutputBits |= _pins[0];	//Activate the Direction Bit for motor 1
}
if(_pins[1] > 0 && _BurstSteps[1] && _BurstSteps[1]--)	//Check if Axis is active, then check/decrement BurstSteps
{
		_flag=true;
_OutputBits |= _pins[1];	//Activate the Direction Bit for motor 2
}
if(_pins[2] > 0 && _BurstSteps[2] && _BurstSteps[2]--)	//Check if Axis is active, then check/decrement BurstSteps
{
		_flag=true;
_OutputBits |= _pins[2];	//Activate the Direction Bit for motor 3
}
if(_pins[3] > 0 && _BurstSteps[3] && _BurstSteps[3]--)	//Check if Axis is active, then check/decrement BurstSteps
{
		_flag=true;
_OutputBits |= _pins[3];	//Activate the Direction Bit for motor 4
}
if(_pins[4] > 0 && _BurstSteps[4] && _BurstSteps[4]--)	//Check if Axis is active, then check/decrement BurstSteps
{
		_flag=true;
_OutputBits |= _pins[4];	//Activate the Direction Bit for motor 5
}
if(_pins[5] > 0 && _BurstSteps[5] && _BurstSteps[5]--)	//Check if Axis is active, then check/decrement BurstSteps
{
		_flag=true; 
_OutputBits |= _pins[5];	//Activate the Direction Bit for motor 6
}
PORTB = _OutputBits;			//Write to the ports
  delayMicroseconds(2);			//Short Delay
	_OutputBits &=63-_SUMPINS ;	//Turn off all active pins
  PORTB = _OutputBits;			//Write to the ports
  
} while(_flag);

//turn off debug pin
digitalWrite(2,LOW);
//allow interupts
sei();

}

/* This is the minimum constructor for StepController it requires a pointer to one PulseClearpath, or
	one StepClearpath motor.  It requires a pointer because this class must use the functions of the
	same clearpath object used in the main routine.

	This function saves the clearpath pointer, and establishes the pins of said motor
	NOTE: the DirPins of the passed motor must be one of pins 8-13
*/
StepController::StepController(PulseClearpath* motor1)
{
	_SUMPINS=0;
	_flag=false;
   _numAxis=1;
   _motors[0]=motor1;
   if(_motors[0]->getDirPin()-8 >= 0)
	_pins[0]=(1<<(_motors[0]->getDirPin()-8));
   _SUMPINS=_pins[0];
  
}

/* This is a constructor for StepController, it requires 2 pointer to PulseClearpath, or
	StepClearpath motors.  It requires pointers because this class must use the functions of the
	same clearpath objects used in the main routine.

	This function saves the clearpath pointers, and establishes the pins of said motors
	NOTE: the DirPins of the passed motor must each be one different of pins 8-13
*/
StepController::StepController(PulseClearpath* motor1, PulseClearpath* motor2)
{
	_SUMPINS=0;
	_flag=false;
   _numAxis=2;
   _motors[0]=motor1;
   _motors[1]=motor2;
   if(_motors[0]->getDirPin()-8 >= 0)
	_pins[0]=(1<<(_motors[0]->getDirPin()-8));
   if(_motors[1]->getDirPin()-8 >= 0)
    _pins[1]=(1<<(_motors[1]->getDirPin()-8));
   _SUMPINS=_pins[0]+_pins[1];
  
}

/* This is a constructor for StepController, it requires 3 pointer to PulseClearpath, or
	StepClearpath motors.  It requires pointers because this class must use the functions of the
	same clearpath objects used in the main routine.

	This function saves the clearpath pointers, and establishes the pins of said motors
	NOTE: the DirPins of the passed motor must each be one different of pins 8-13
*/
StepController::StepController(PulseClearpath* motor1, PulseClearpath* motor2, PulseClearpath* motor3)
{
	_SUMPINS=0;
	_flag=false;
   _numAxis=3;
   _motors[0]=motor1;
   _motors[1]=motor2;
   _motors[2]=motor3;
   if(_motors[0]->getDirPin()-8 >= 0)
	_pins[0]=(1<<(_motors[0]->getDirPin()-8));
   if(_motors[1]->getDirPin()-8 >= 0)
    _pins[1]=(1<<(_motors[1]->getDirPin()-8));
   if(_motors[2]->getDirPin()-8 >= 0)
    _pins[2]=(1<<(_motors[2]->getDirPin()-8));
   _SUMPINS=_pins[0]+_pins[1]+_pins[2];
  
}

/* This is a constructor for StepController, it requires 4 pointer to PulseClearpath, or
	StepClearpath motors.  It requires pointers because this class must use the functions of the
	same clearpath objects used in the main routine.

	This function saves the clearpath pointers, and establishes the pins of said motors
	NOTE: the DirPins of the passed motor must each be one different of pins 8-13
*/
StepController::StepController(PulseClearpath* motor1, PulseClearpath* motor2, PulseClearpath* motor3, PulseClearpath* motor4)
{
	_SUMPINS=0;
	_flag=false;
   _numAxis=4;
   _motors[0]=motor1;
   _motors[1]=motor2;
   _motors[2]=motor3;
   _motors[3]=motor4;
   if(_motors[0]->getDirPin()-8 >= 0)
	_pins[0]=(1<<(_motors[0]->getDirPin()-8));
   if(_motors[1]->getDirPin()-8 >= 0)
    _pins[1]=(1<<(_motors[1]->getDirPin()-8));
   if(_motors[2]->getDirPin()-8 >= 0)
    _pins[2]=(1<<(_motors[2]->getDirPin()-8));
   if(_motors[3]->getDirPin()-8 >= 0)
   _pins[3]=(1<<(_motors[3]->getDirPin()-8));
   _SUMPINS=_pins[0]+_pins[1]+_pins[2]+_pins[3];
  
}

/* This is a constructor for StepController, it requires 5 pointer to PulseClearpath, or
	StepClearpath motors.  It requires pointers because this class must use the functions of the
	same clearpath objects used in the main routine.

	This function saves the clearpath pointers, and establishes the pins of said motors
	NOTE: the DirPins of the passed motor must each be one different of pins 8-13
*/
StepController::StepController(PulseClearpath* motor1, PulseClearpath* motor2, PulseClearpath* motor3, PulseClearpath* motor4, PulseClearpath* motor5)
{
	_SUMPINS=0;
	_flag=false;
   _numAxis=5;
   _motors[0]=motor1;
   _motors[1]=motor2;
   _motors[2]=motor3;
   _motors[3]=motor4;
   _motors[4]=motor5;
   if(_motors[0]->getDirPin()-8 >= 0)
	_pins[0]=(1<<(_motors[0]->getDirPin()-8));
   if(_motors[1]->getDirPin()-8 >= 0)
    _pins[1]=(1<<(_motors[1]->getDirPin()-8));
   if(_motors[2]->getDirPin()-8 >= 0)
    _pins[2]=(1<<(_motors[2]->getDirPin()-8));
   if(_motors[3]->getDirPin()-8 >= 0)
    _pins[3]=(1<<(_motors[3]->getDirPin()-8));
   if(_motors[4]->getDirPin()-8 >= 0)
    _pins[4]=(1<<(_motors[4]->getDirPin()-8));
   _SUMPINS=_pins[0]+_pins[1]+_pins[2]+_pins[3]+_pins[4];
  
}

/* This is a constructor for StepController, it requires 6 pointer to PulseClearpath, or
	StepClearpath motors.  It requires pointers because this class must use the functions of the
	same clearpath objects used in the main routine.

	This function saves the clearpath pointers, and establishes the pins of said motors
	NOTE: the DirPins of the passed motor must each be one different of pins 8-13
*/
StepController::StepController(PulseClearpath* motor1, PulseClearpath* motor2, PulseClearpath* motor3, PulseClearpath* motor4, PulseClearpath* motor5, PulseClearpath* motor6)
{
	_SUMPINS=0;
	_flag=false;  
   _numAxis=6;
   _motors[0]=motor1;
   _motors[1]=motor2;
   _motors[2]=motor3;
   _motors[3]=motor4;
   _motors[4]=motor5;
   _motors[5]=motor6;
   if(_motors[0]->getDirPin()-8 >= 0)
	_pins[0]=(1<<(_motors[0]->getDirPin()-8));
   if(_motors[1]->getDirPin()-8 >= 0)
    _pins[1]=(1<<(_motors[1]->getDirPin()-8));
   if(_motors[2]->getDirPin()-8 >= 0)
    _pins[2]=(1<<(_motors[2]->getDirPin()-8));
   if(_motors[3]->getDirPin()-8 >= 0)
    _pins[3]=(1<<(_motors[3]->getDirPin()-8));
   if(_motors[4]->getDirPin()-8 >= 0)
    _pins[4]=(1<<(_motors[4]->getDirPin()-8));
   if(_motors[5]->getDirPin()-8 >= 0)
    _pins[5]=(1<<(_motors[5]->getDirPin()-8));
   _SUMPINS=_pins[0]+_pins[1]+_pins[2]+_pins[3]+_pins[4]+_pins[5];
  
}

/*	
	This function sets up the ISR to run at 2kHz if it is passed the value of 249.
	It also, rechecks the direction pins of each connected motor
*/
void StepController::Start(int time)
{
	_SUMPINS=0;
	for( int i=0; i<_numAxis; i++)
   {
	   if(_motors[i]->getDirPin()-8 >= 0)
			_pins[i]=(1<<(_motors[i]->getDirPin()-8));
	   _SUMPINS+=_pins[i];
   }
	
	cli();//stop interrupts

   // set up Timer 1
   TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0

  //Set compare match register to time
  OCR2A = time;// time should be 249, 1-256 will produce different frequencies

  // turn on CTC mode
  TCCR2A |= (1 << WGM21);

  // Set CS21 bit for 8 prescaler
  TCCR2B = 0;
  TCCR2B |= (1 << CS01) | (1 << CS00);  

  // enable timer compare interrupt
  TIMSK2=0;
  TIMSK2 |= (1 << OCIE2A);

  sei();//allow interrupts
}

/*	
	This function disables the ISR so any motor connected will no longer output pulses
	The motors should remember what command they were on, but if a move was interuppted
	by this function, large accelerations/deccelerations may be experienced.
*/
void StepController::Stop()
{
	cli();//stop interrupts
//   
   // set up Timer 1
   TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0

  sei();//allow interrupts
}

// This is a debugging function
int StepController::getsum()
{
	return _motors[0]->calcSteps();
}

