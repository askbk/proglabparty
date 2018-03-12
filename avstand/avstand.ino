int echoPin1 = 6;
int triggerPin1i = 7;


int echoPin2 = 2;
int triggerPin2 = 3;

void setup() {
  Serial.begin(9600);
  
  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  

}

void loop() {

  Serial.println("Avstand 1: " + String(sjekk_avstand(echoPin1, triggerPin1)));
  Serial.println("Avstand 2: " + String(sjekk_avstand(echoPin2, triggerPin2)));

 

}

float sjekk_avstand(int ekko,int  trigger){
  digitalWrite(trigger, 0);
  delayMicroseconds(2);
  digitalWrite(trigger,1);
  delayMicroseconds(10);
  digitalWrite(trigger, 0);
  long duration = pulseIn(ekko, 1);
  float distance = duration * 0.00017;
  //Serial.println(distance);
  return (distance);
}

