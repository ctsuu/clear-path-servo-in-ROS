/*StepAndDirection
  Runs a Teknic Clearpath motor in step and direction mode, back and forth 
  This example code is in the public domain.
 ** all step/dir pins have to be inserted into pins 8-13 (not including ~enable - enable can go anywhere)!
 */

#include "PulseClearpath.h"
#include "StepController.h"
#include "StepClearpath.h"

StepClearpath X; // initialize a StepClearpath Motor

StepController machine(&X);  //initialize the controller and pass the reference to the motor we are controlling

boolean IsSDHPmodel=false;
boolean IsSDSKmodel=true;

double TotalDistance=75000;     // distance in Steps - check step/rev in ClearPath MSP config file (1,600 steps/rev)  

void setup(){      
    //pinMode(2,OUTPUT);  //DEBUG declare pin 2 as output.  Abe, this shows when the arduino is in the ISR
    Serial.begin(9600);
    X.attach(8,9,10);      //Direction/A is pin 8, Step/B is pin 9, Enable is pin 10, HLFB is pin 4  //JWS: HFLB = high level feedback info coming from motor to arduino
    X.setMaxVel(95000);  // Set max Velocity.  Parameter can be between 2 and 100,000
    X.setMaxAccel(1500000);  // Set max Acceleration.  Parameter can be between 4000 and 2,000,000
    X.enable();  // Enable motor, reset the motor position to 0 //JWS: enable happens automatically per Clearpath (whenever enabled) - no actual arduino code required
  
    Serial.print("Homing...");      // wait until the command is finished and then 1 more second
    while(X.readHLFB()) { 
        Serial.print('.');
      }
    
    Serial.println();
    Serial.println("Homing Complete");
    delay(200);
  
    machine.Start(249);     // Set up the ISR to constantly check motor position.  PARAMETER MUST BE SET TO 249

  }

void loop(){
    X.move(TotalDistance);
    delay(20);
    Serial.print("Full Move...");
    while(X.readHLFB())
      { 
        Serial.print('.');
      }
      Serial.println();
    Serial.println("Move Done");

    // Move the motor back to home
    X.move(-TotalDistance);
    delay(20);
    Serial.print("Return Move...");
    while(X.readHLFB())
      { 
        Serial.print('.');
      }
      Serial.println();
    Serial.println("Move Done"); 
}
