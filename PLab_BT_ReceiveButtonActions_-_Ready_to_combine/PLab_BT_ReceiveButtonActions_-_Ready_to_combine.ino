/*
 */

#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

const int txPin = 2; // Connected to tx on bt unit
const int rxPin = 3; // Connected to rx on bt unit

const int redLedPin =  4;       // the number of the red LED pin
const int greenLedPin =  5;     // the number of the green LED pin
char TACTIC = 'd';
boolean redLedState = false;
boolean greenLedState = false;

PLabBTSerial btSerial(txPin, rxPin);

//...........................................................................
// Always include this method.
// It reads from the BT port and calls BTSerialMessageReceived.
// 
char msg[100];
void updateBTSerial() {
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
//...........................................................................

void setup()
{  
  btSerial.begin(9600); // Open serial communication to Bluetooth unit
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  Serial.begin(9600);   // Open serial communication to Serial Monitor
  Serial.println("Hallo world!");
}

void loop() {
   updateBTSerial();  // Check if we have input on the BT serial port.
}

void BTSerialMessageReceived(String msgString,int msgValue) {
  Serial.print("Message:"); Serial.print(msgString); // Debug print
  Serial.print(", Value:"); Serial.println(msgValue);  // Debug print
  if (msgString == "g") {
    break; return;
  } 
  if (msgString == "d") {
    TACTIC = 'd'; return;
  } 
  if (msgString == "o") {
    TACTIC = 'o'; return;
  } 
  return;
}



