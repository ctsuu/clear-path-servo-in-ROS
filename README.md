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


