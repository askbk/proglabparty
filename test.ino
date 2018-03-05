
#include <Pushbutton.h>
#include <ZumoMotors.h>
// Example code for PLab Zumo libary:
// API:
// void forward(int _speed, int length);
// void backward(int _speed, int _length);
// void turnRight(int _speed, int degrees);
// void turnLeft(int _speed, int degrees); 
//
// Dag Svanæs 2015, IDI, NTNU

Pushbutton button(ZUMO_BUTTON);
ZumoMotors motors;
int zumoSpeed = 400;  // Speed is 200 for all operations in this example

void setup()
{
  
  
}

void loop()   // Draw a triangle. 45, 90, 45 degrees...
{
 
  
  button.waitForButton();  
  delay(500);
  motors.setSpeeds(100,100); //set left and right motor speed
  delay(1000);
  motors.setSpeeds(150,-100);
  delay(1000);
  motors.setSpeeds(100,100);
  delay(1000);
  motors.setSpeeds(-100,70);
  delay(1000);
  motors.setSpeeds(-100,-100);
  delay(1000);   
  motors.setSpeeds(-100,70);
  delay(1000);
  motors.setSpeeds(0,0);
}
