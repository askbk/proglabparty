int echoPin = 6;
int triggerPin=7;


int echoPin = 2;
int triggerPin=3;

void setup() {
  Serial.begin(9600);
  
  pinMode(triggerPin,OUTPUT);
  pinMode(echoPin, INPUT);
  

}

void loop() {

  Serial.println("Avstand "+String(sjekk_avstand()));
  //Serial.println("Avstand1 "+String(sjekk_avstand()));
  

}

double sjekk_avstand(){
  digitalWrite(triggerPin, 0);
  delayMicroseconds(2);

  digitalWrite(triggerPin,1);
  delayMicroseconds(10);

  digitalWrite(triggerPin, 0);

  long duration = pulseIn(echoPin, 1);
  float distance = duration * 0.00017;
  //Serial.println(distance);
  
  return (distance1, distance2);
}
