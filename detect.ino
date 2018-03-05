/*
Drive forward and turn left or right when border is detected
  - Only reflectionsensor 0 and 5 are used.
*/
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
 
#define LED 13
#define echoPin 2 // Echo Pin
#define trigPin 3 // Trigger Pin
int maximumRange = 30; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1800 // 
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     100
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     500 // ms

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray sensors;


 
void setup() {
   Serial.begin(9600);
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   sensors.init();
   button.waitForButton();
}

void loop() {
  
      sensors.read(sensor_values);     
      
      if (sensor_values[0] > QTR_THRESHOLD) {
        // if leftmost sensor detects line, reverse and turn to the right
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      } else if (sensor_values[5] > QTR_THRESHOLD) {
        // if rightmost sensor detects line, reverse and turn to the left
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      } else {
        // otherwise, go straight
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        avstand();
      }
}

void avstand() {
    //Avstandsmåleren sjekker om det er noe nærmere enn 60cm
   digitalWrite(trigPin, LOW); 
   delayMicroseconds(2);
  
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10); 
   
   digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   Serial.println(duration);
   //Calculate the distance (in cm) based on the speed of sound.
   distance = duration / 58.2;
   
   if (distance <= maximumRange && distance < minimumRange){
        delay(200);   
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(REVERSE_DURATION);
   }
 }

