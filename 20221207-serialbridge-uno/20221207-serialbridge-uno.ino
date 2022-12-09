#include <SoftwareSerial.h>

int led = 13; //Led connected to pin 13
int data = 0; //Serial Data
int data2 = 0; //RX TX Data

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
  int query = 9;
 if (Serial.available() > 0) {
    mySerial.write(Serial.parseInt());
  }

  if (mySerial.available() > 0) {
    data2 = mySerial.read();
    if(query == 9){
       Serial.print("minuto ");
          Serial.println(data2);
    }
  }
  delay(1000);
}
