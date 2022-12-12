#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// VARIABLES DE COMUNICACIÓN
String recibo;
bool stringComplete;

// VARIABLES COMUNES ENTRE UNO Y WEMOS
int hora;
int minuto;
float temperatura;
int isOnOff;
int isIntruso;
int isEmergencia;

// conectarse al wifi y ntc//
const String ssid     = "manuwifi2";
const String password = "l33tsupa";
const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// comunicación serie //
int led = D3; //Led connected to pin 13
int data = 0; //Serial Data
int data2 = 0; //RX TX Data

SoftwareSerial mySerial(D2, D1); // RX, TX

void setup(){


  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  Serial.begin(9600);
  mySerial.begin(9600);
  WiFi.begin(ssid, password);
  
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

// DIVIDIR STRINGS PARA TRANSFORMARLAS EN VARIABLES
// fuente: https://arduino.stackexchange.com/a/1237
String getValue(String data, char separator, int index){
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

float serialToFloat(String cadena, int pos){
  Serial.print("cadena: ");
  Serial.println(cadena);
  Serial.print("trozo: ");
  Serial.println(getValue(cadena, ';', 1));
  String temp = getValue(cadena, ';', pos);
  char temp2[8];
  temp.toCharArray(temp2,temp.length() + 1);
  return atof(temp2);
}

void uno_enviar(){

  // CODIFICACIÓN DE LA HORA
  timeClient.update();
  
  char hora[7];
  char minu[7];
  
  itoa(timeClient.getHours(),hora,10);
  itoa(timeClient.getMinutes(),minu,10); 
  
  mySerial.write(hora);
  mySerial.write(':');
  mySerial.write(minu);

  // CODIFICACIÓN DEL ESTADO DE LA BOMBILLA

  // CODIFICACIÓN DE LA DETENCIÓN COMPLETA DEL SISTEMA
  
  mySerial.write('_');

}

void uno_recibir(){
  if (mySerial.available()){
    while (!stringComplete){
      char inChar = mySerial.read();
      if ((inChar != '\n'  && inChar != '\r' && inChar != '_')
      && !(stringComplete)){
        recibo += inChar;
      }
      
      if (inChar == '_') {
        Serial.print("string que viene:");
        Serial.println(recibo);
        stringComplete = true;
      }
    }
    if (stringComplete){
      temperatura = getValue(recibo, ':', 0).toFloat();
      isOnOff = getValue(recibo, ':',1).toInt();
      isIntruso = getValue(recibo, ':',2).toInt();
      isEmergencia = getValue(recibo, ':',3).toInt();
      Serial.print("temperatura: ");
      Serial.println(temperatura);
      Serial.print("estado: ");
      Serial.println(isOnOff);
      Serial.print("intruso: ");
      Serial.println(isIntruso);
      Serial.print("emerg: ");
      Serial.println(isEmergencia);
      stringComplete = false;
      recibo = "";
    }
  }
}

void loop(){
  uno_enviar();
  uno_recibir();
  delay(1000);
}
