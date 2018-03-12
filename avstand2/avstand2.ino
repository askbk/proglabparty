#include <NewPing.h>

int ekko1 = 2;
int ekko2 = 4;
int ekko3 = 7;

int trigger1 = 3;



int maxDistace = 50;

NewPing sonar1(trigger1, ekko1, maxDistace);
NewPing sonar2(trigger1, ekko2, maxDistace);
NewPing sonar3(trigger1, ekko3, maxDistace);

void setup() {
  Serial.begin(9600);

  pinMode(trigger1,OUTPUT);
  pinMode(ekko1, INPUT);

  pinMode(ekko2, INPUT);

  pinMode(ekko3, INPUT);

}

 

void loop() {
  unsigned int tid1 = sonar1.ping();
  unsigned int tid2 = sonar2.ping();
  unsigned int tid3 = sonar3.ping();

  float avstand1 = sonar1.convert_cm(tid1);
  float avstand2 = sonar2.convert_cm(tid2);
  float avstand3 = sonar3.convert_cm(tid3);

  if (avstand1 > 0){
    //Serial.println("Nummer1 : "+String(avstand1));  
    Serial.println("Rett fram");
  }
  else if (avstand2 > 0){
    //Serial.println("Nummer2 :" + String(avstand2));
    Serial.println("Venstre");
  }
  else if (avstand3 > 0){
    //Serial.println("Nummer3 :" + String(avstand3));
    Serial.println("Høgre");
  }
  //else{
    //Serial.println("leit");
  //}


  
  
}


