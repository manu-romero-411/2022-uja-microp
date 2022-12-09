#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const String ssid     = "vodafone1B20";
const String password = "NP7UTSVGTW5858";
const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);



// COSAS DE LA COMUNICACIÓN SERIE CON EL UNO
int led = D3; //Led connected to pin 13
int data = 0; //Serial Data
int data2 = 0; //RX TX Data
SoftwareSerial mySerial(D2, D1); // RX, TX

void setup(){
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(led, OUTPUT);
  WiFi.begin(ssid, password);
  
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  
  
}

void enviar_hora(int selec){
    if(data == 2){
      mySerial.write(timeClient.getHours());
      data  = -1;
    } 
    
    if(data == 1){
        mySerial.write(timeClient.getMinutes());
        data  = -1;
    }   
}

void recibir_info(){
  int cual = mySerial.read();
  if(cual == 1){
    Serial.println("Temperatura normal");
  }
  if(cual == 2){
    Serial.println("[INFO] Temperatura bajita");
  }
  if(cual == 3){
    Serial.println("[MAL] Temperatura muy baja");
  }
  if(cual == 4){
    Serial.println("[INFO] Temperatura altita");
  }
  if(cual == 5){
    Serial.println("[MAL] Temperatura muy alta");
  }
  if(cual == 6){
    Serial.println("[INFO] Intruso en habitacion");
  }
  if(cual == 7){
    Serial.println("[MAL] El intruso sigue ahí. Llamar a la poli");
  }
  if(cual == 8){
    mySerial.write(timeClient.getHours());
  }
  if(cual == 9){
    mySerial.write(timeClient.getMinutes());
  }
}

void loop() {
  timeClient.update();

  if (Serial.available() > 0) {
    enviar_hora(1);
  }

  if (mySerial.available() > 0) {
    recibir_info();
  }

  delay(1000);
}
