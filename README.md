# clear-path-servo-in-ROS
Integrate clear path servo motor into ROS


## Step and direction commands

Most of the move can summarize to move length, velocity and acceleration. For more smooth control, we can added jerk and jerk-derivative control. Jerk control also can achive higher acceleration without put too much stress on mechanical system.   
https://www.youtube.com/watch?v=pi3isfx8LAQ

https://www.youtube.com/watch?time_continue=1&v=QZLwDnGSaeI

## Use Arduino Library
```
#include <Stepper.h>

const int stepsPerRevolution = 3200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper1(stepsPerRevolution, 8, 9, 10, 3);

void setup() {
  // set the speed at 3000 rpm:
  myStepper1.setSpeed(3000);
  
  // initialize the serial port:
  Serial.begin(250000);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper1.setSpeed(1000);
  myStepper1.step(stepsPerRevolution);
    
  delay(500);
  
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper1.setSpeed(1000);
  myStepper1.step(0.2*stepsPerRevolution);
  
  delay(5000);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  
  myStepper1.setSpeed(3000);
  myStepper1.step(-5*stepsPerRevolution);
  delay(150);
}
```
## Random Input
```
#include <Stepper.h>

const int stepsPerRevolution = 400;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper1(stepsPerRevolution, 8, 9, 10, 3);
Stepper myStepper2(stepsPerRevolution, 11, 12, 13, 4);

int stepCount = 0;

void setup() {
  // set the speed at 3000 rpm:
  myStepper1.setSpeed(3000);
  myStepper2.setSpeed(3000);

  // initialize the serial port:
  Serial.begin(250000);
}

void loop() {
  // read the sensor value:
  int sensorReading = analogRead(A0);

  sensorReading = random(0,1023);
  int motorDirection = random(-1,1);
  // map it to a range from 0 to 100:
  int motorSpeed = map(sensorReading, 0, 1023, 0, 3000);
  // set the motor speed:

  if (motorSpeed > 0) {
    myStepper1.setSpeed(motorSpeed);
    // step 1/100 of a revolution:
    myStepper1.step(stepsPerRevolution / 100);
    if(motorDirection>0){
      // step one revolution  in one direction:
      Serial.println("clockwise");
      //myStepper1.setSpeed(1000);
      myStepper1.step(stepsPerRevolution);
    }else{
      // step one revolution  in one direction:
      Serial.println("counter clockwise");
      //myStepper1.setSpeed(1000);
      myStepper1.step(-1*stepsPerRevolution);
    }
  }

  
  // step one revolution  in one direction:
  Serial.println("clockwise");
  //myStepper1.setSpeed(1000);
  myStepper1.step(stepsPerRevolution);
  myStepper2.step(stepsPerRevolution);
  
}
```
## Simple Arduino driver code
Refer to the following link.
https://www.youtube.com/watch?v=fHAO7SW-SZI

According to Clear path driver, the enable pin is enabled at HIGH. 
```
digitalWrite(ENABLE_PIN, HIGH);
```

## Use Interrupt Service Rountine
Refer to 
http://www.instructables.com/id/External-Interrupt-in-arduino/

For some reason, the PORT assignment does not working. Therefore, I change back to digitalWrite pin to HIGH and LOW.
```
#define STEP_HIGH        PORTF |=  0b00000001;
#define STEP_LOW         PORTF &= ~0b00000001;
```
```
digitalWrite(STEP_PIN, HIGH);
digitalWrite(STEP_PIN, LOW);
//STEP_HIGH
//STEP_LOW
```

## Up to 6 motors controls
Refer to RAMPS 1.4 
```
// For RAMPS 1.4
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING
```

## Swap E0/E1 channel to Additional 3 channels
```
#define A_DIR_PIN          28
#define A_STEP_PIN         26
#define A_ENABLE_PIN       24

#define B_DIR_PIN          34
#define B_STEP_PIN         36
#define B_ENABLE_PIN       30

#define C_DIR_PIN          32
#define C_STEP_PIN         47
#define C_ENABLE_PIN       45
```
## 4 More Servo Channels
```
#define Servo_0_PIN        11
#define Servo_1_PIN        6
#define Servo_2_PIN        5
#define Servo_3_PIN        4
```



