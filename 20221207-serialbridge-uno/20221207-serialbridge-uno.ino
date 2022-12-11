#include <SoftwareSerial.h>

// VARIABLES COMUNES ENTRE UNO Y WEMOS
int hora;
int minuto;
float temperatura;
int isOnOff;
int isIntruso;
int isEmergencia;

int led = 13; //Led connected to pin 13
int data = 0; //Serial Data
int data2 = 0; //RX TX Data
String recibo;
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

void wemos_recibir(){
  if (mySerial.available() > 0) {
    while (!stringComplete){
      char inChar = mySerial.read();
      if ((inChar != '\n'  && inChar != '\r' && inChar != '_')
      && !(stringComplete)){
        recibo += inChar;
      }
      
      if (inChar == '_') {
        stringComplete = true;
      }
    }
    if (stringComplete){
      //hora = getValue(recibo, ':', 0).toInt();
      //minuto = getValue(recibo, ':',1).toInt();
      Serial.print("hora: ");
      Serial.println(hora);
      Serial.print("minuto: ");
      Serial.println(minuto);
      stringComplete = false;
      recibo = "";
    }
  } else {
    Serial.println("[ERROR] Dispositivo serie del WEMOS no disponible");
  }
}

void wemos_enviar(){
  if (mySerial.available() > 0) {
    mySerial.write("14.5:1");
    mySerial.write('_');
  } else {
    Serial.println("[ERROR] Dispositivo serie del WEMOS no disponible");
  }
}

void loop(){
  //wemos_recibir();
  wemos_enviar();
  delay(1000);
}
