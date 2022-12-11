#include <SoftwareSerial.h>

int led = 13; //Led connected to pin 13
int data = 0; //Serial Data
int data2 = 0; //RX TX Data
String inputString;
bool stringComplete;


SoftwareSerial mySerial(6, 7); // RX, TX

void setup()
{
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(led, OUTPUT);
}


void loop()
{  
      mySerial.write(Serial.read());
      
/*
  if (mySerial.available() > 0) {
    char inChar = mySerial.read();
    if ((inChar != '\n'  && inChar != '\r' && inChar != '_')
    && !(stringComplete)){
      inputString += inChar;
    }
    
    if (inChar == '_') {
      stringComplete = true;
    }
    if (stringComplete){
      Serial.println(inputString);
    }
  }
  */
  delay(1000);
}
