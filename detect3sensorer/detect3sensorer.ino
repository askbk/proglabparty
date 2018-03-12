/*
  Kjører på banen med disse egenskaper:
    - Svinger vekk fra kant ved å bruke sensor 0 og 5
    - Bruker avstandsmåler til å sjekke om det er noe å kjøre på.
      - En i midten og en på hver side. Kan dermed snu til venstre og høyre og kjøre mot objekter.
    - Om det har gått STANDARD tid fra en sving skal den snurre i ring og sjekke om den finner noe å kjøre på. 
*/
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>
#define LED 13
//Avstandsmåler
int ekko_m = 3;
int ekko_h = 0;
int ekko_v = A1;
int trigger1 = 6;
int maxDistace = 50;
NewPing sonar_m(trigger1, ekko_m, maxDistace);
NewPing sonar_h(trigger1, ekko_h, maxDistace);
NewPing sonar_v(trigger1, ekko_v, maxDistace);
  
// Diverse kjøre egenskaper
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        400
#define FORWARD_SPEED     200
#define SUPER_SPEED       400
#define SUPER_DURATION    10
#define REVERSE_DURATION  250 // ms
#define TURN_DURATION     200 // ms

//Timer egenskaper
#define STANDARD          1500
double TIMER_M = 0;
double TIMER_S = STANDARD;
double time1 = 0;
double lastTime1 = 0;
double dT=0;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

//Strekmåling
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS]; 
ZumoReflectanceSensorArray sensors;
// Overflate Treshold, satt til hvit kant
#define QTR_THRESHOLD  500 // 
 
void setup(){
   Serial.begin(9600);
   //Avstandsmåler
   pinMode(trigger1,OUTPUT);
   pinMode(ekko_m, INPUT);
   pinMode(ekko_h, INPUT);
   pinMode(ekko_v, INPUT);
   sensors.init();
   //Button for å starte
   button.waitForButton();
   motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
}

void loop(){
      //Leser strekmåling
      sensors.read(sensor_values);  
      //Leser avstand mid       
      unsigned int tid_m = sonar_m.ping();    
      float avstand_m = sonar_m.convert_cm(tid_m);
      
      //Sjekker noe svart underlag      
      if (sensor_values[0] > QTR_THRESHOLD && sensor_values[5] > QTR_THRESHOLD){   
          //Sjekker noe mid
          if (avstand_m > 0){        
              motors.setSpeeds(SUPER_SPEED, SUPER_SPEED);
              TIMER_M = 0;
          }else{     
              //Leser avstand høy, ven       
              unsigned int tid_h = sonar_h.ping();
              unsigned int tid_v = sonar_v.ping();
              float avstand_h = sonar_h.convert_cm(tid_h);
              float avstand_v = sonar_v.convert_cm(tid_v);
              //Sjekker avstand h
              if (avstand_h>0){
                   motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
                   TIMER_M = 0;
              }//Sjekker avstand v
              else if (avstand_v>0){
                  motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
                  TIMER_M = 0;
              }else{
              // Ellers, kjør rett frem
              motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
              //(time1-lastTime1) er tiden på en loop-runde -> deltaTid -> dT
              lastTime1 = time1;
              time1 = millis();
              dT = (time1-lastTime1);
              if (!(TIMER_M == 0)){ // trekker fra dT fra TIMER_M
                TIMER_M -= dT;
              }
              if(TIMER_M < 0){ //Snur i ring helt til noe annet inntreffer eller tiden for snurr har gått til 0
                if (TIMER_S > 0) {                  
                  motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
                  TIMER_S -= dT;
                }else{
                  TIMER_S = STANDARD;
                  TIMER_M = 0;
                }
              }
          }
        }
      }else if (sensor_values[0] < QTR_THRESHOLD){
        // om venstre sensor ser strek, bakk og snu høyre
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        TIMER_M = STANDARD;  
      }else if (sensor_values[5] < QTR_THRESHOLD){
        // om høyre sensor ser strek, bakk og snu venstre
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        TIMER_M = STANDARD;    
      }else{
        // overflødig?
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        
      }
}


  






