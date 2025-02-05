/*
Drive forward and turn left or right when border is detected
  - Only reflectionsensor 0 and 5 are used.
*/
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
 
#define LED 13
#define echoPin 3 // Echo Pin
#define triggerPin 6 // Trigger Pin
float maximumRange = 0.5; // Maximum range needed
float minimumRange = 0; // Minimum range needed

// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  500 // 
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        400
#define FORWARD_SPEED     100
#define SUPER_SPEED       400
#define SUPER_DURATION    10
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     200 // ms

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray sensors;


 
void setup()
{
   Serial.begin(9600);
   pinMode(triggerPin, OUTPUT);
   pinMode(echoPin, INPUT);
   sensors.init();
   button.waitForButton();
}

void loop(){
      
      
      sensors.read(sensor_values);   
      float distance = sjekk_avstand();
      //Serial.println(distance);
      //Sjekker noe forann, kjører på.
      if (distance <= maximumRange && distance > minimumRange && sensor_values[0] > QTR_THRESHOLD && sensor_values[5] > QTR_THRESHOLD){   
        motors.setSpeeds(SUPER_SPEED, SUPER_SPEED);
        
      }
      else if (sensor_values[0] < QTR_THRESHOLD)
      {
        // if leftmost sensor detects line, reverse and turn to the right
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      }
      else if (sensor_values[5] < QTR_THRESHOLD)
      {
        // if rightmost sensor detects line, reverse and turn to the left
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      }
      
      else
      {
        // otherwise, go straight
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        
      }
}

  float sjekk_avstand() {
    //Avstandsmåleren sjekker om det er noe nærmere enn 60cm
    digitalWrite(triggerPin, 0);
    delayMicroseconds(2);
  
    digitalWrite(triggerPin,1);
    delayMicroseconds(10);
  
    digitalWrite(triggerPin, 0);
  
    long duration = pulseIn(echoPin, 1);
    float distance = duration * 0.00017;
    
    return distance;
   
   
 }

