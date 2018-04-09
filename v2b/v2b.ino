/*
  Kjører på banen med disse egenskaper:
    - Svinger vekk fra kant ved å bruke sensor 0 og 5
    - Bruker avstandsmåler til å sjekke om det er noe å kjøre på.
      - En i midten og en på hver side. Kan dermed snu til venstre og høyre og kjøre mot objekter.
    - Om det har gått STANDARD tid fra en sving skal den snurre i ring og sjekke om den finner noe å kjøre på. 
*/
#include <Adafruit_MCP23008.h>
#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define LED 13
#define portExpPin 0
Adafruit_MCP23008 mcp;

//Avstandsmåler
int ekko_m = 3;
int ekko_h = 0;
int ekko_v = A1;
int trigger1 = 6;
int maxDistace = 50;

NewPing sonar_m(trigger1, ekko_m, maxDistace);
NewPing sonar_h(trigger1, ekko_h, maxDistace);
NewPing sonar_v(trigger1, ekko_v, maxDistace);

//BT
const int txPin = A4; // Connected to tx on bt unit
const int rxPin = A5; // Connected to rx on bt unit


char msg[100];

// Diverse kjøreegenskaper
#define REVERSE_SPEED     400 // 0 is stopped, 400 is full speed
#define TURN_SPEED        400
#define FORWARD_SPEED     300
#define SUPER_SPEED       400
#define SUPER_DURATION    10
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     200 // ms

char TACTIC = 'o';
boolean isGo = false;
//Timer egenskaper
#define STANDARD          1000
double TIMER_M = 0;
double TIMER_S = STANDARD;
double time1 = 0;
double lastTime1 = 0;
double dT = 0;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

//Strekmåling
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS]; 
ZumoReflectanceSensorArray sensors;
// Overflate Treshold, satt til hvit kant
#define QTR_THRESHOLD  500 // 

void updateBTSerial(PLabBTSerial& btSerial) {
  int availableCount = btSerial.available();
  if (availableCount > 0) {
    btSerial.read(msg, availableCount);
    char *divided = strchr(msg,',');
    int msgValue = 0;
    if (divided != 0) {
       divided[0] = 0; divided++;
       String str(divided);
       msgValue = str.toInt();
    };
    String msgString(msg);
    BTSerialMessageReceived(msgString,msgValue);   
  }
}

void BTSerialMessageReceived(String msgString,int msgValue) {
  Serial.print("Message:"); Serial.print(msgString); // Debug print
  Serial.print(", Value:"); Serial.println(msgValue);  // Debug print
  if (msgString == "g") {
    isGo=true; return;
  } 
  if (msgString == "d") {
    TACTIC = 'd'; return;
  } 
  if (msgString == "o") {
    TACTIC = 'o'; return;
  } 
}

void setup(){
  Serial.begin(9600);
  PLabBTSerial btSerial(txPin, rxPin);  
  btSerial.begin(9600); // Open serial communication to Bluetooth unit
  
  //Avstandsmåler
  pinMode(trigger1,OUTPUT);
  pinMode(ekko_m, INPUT);
  pinMode(ekko_h, INPUT);
  pinMode(ekko_v, INPUT);
  sensors.init();
  //BT henting
  while(!button.isPressed() && !isGo){
      updateBTSerial(btSerial);  // Check if we have input on the BT serial port.
  }
  if(TACTIC == 'd'){    
     motors.setSpeeds(SUPER_SPEED, SUPER_SPEED);
  }
  else if (TACTIC == 'o'){
     motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
     TIMER_M = -1;
     TIMER_S = STANDARD;
  }
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
              float avstand_h = sonar_h.convert_cm(tid_h);              
              unsigned int tid_v = sonar_v.ping();               
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






