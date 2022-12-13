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
const String ssid     = "vodafone1B20";
const String password = "NP7UTSVGTW5858";
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
WiFiServer server(80);

void setup(){
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  Serial.begin(9600);
  mySerial.begin(9600);
  WiFi.begin(ssid, password);
  
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    //Serial.print ( "." );
  }
  server.begin();
  //Serial.println(F("Server started"));

  // Print the IP address
  //Serial.println(WiFi.localIP());
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
  //Serial.print("cadena: ");
  //Serial.println(cadena);
  //Serial.print("trozo: ");
  //Serial.println(getValue(cadena, ';', 1));
  String temp = getValue(cadena, ';', pos);
  char temp2[8];
  temp.toCharArray(temp2,temp.length() + 1);
  return atof(temp2);
}

void actualizarHora(){
  timeClient.update();
  hora = timeClient.getHours();
  minuto = timeClient.getMinutes();
}

void uno_enviar(){

  // CODIFICACIÓN DE LA HORA
  
  char horaf[7];
  char minu[7];
  char onoff[7];
  
  itoa(hora,horaf,10);
  itoa(minuto,minu,10); 
  itoa(isOnOff,onoff,10);
  
  mySerial.write(horaf);
  mySerial.write(':');
  mySerial.write(minu);
  mySerial.write(':');
  mySerial.write(onoff);

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
        //Serial.print("string que viene:");
        //Serial.println(recibo);
        stringComplete = true;
      }
    }
    if (stringComplete){
      temperatura = getValue(recibo, ':', 0).toFloat();
      isOnOff = getValue(recibo, ':',1).toInt();
      isIntruso = getValue(recibo, ':',2).toInt();
      isEmergencia = getValue(recibo, ':',3).toInt();
      //Serial.print("temperatura: ");
      //Serial.println(temperatura);
      //Serial.print("estado: ");
      //Serial.println(isOnOff);
      //Serial.print("intruso: ");
      //Serial.println(isIntruso);
      //Serial.print("emerg: ");
      //Serial.println(isEmergencia);
      stringComplete = false;
      recibo = "";
    }
  }
}

void web() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  //Serial.println(F("new client"));

  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  //Serial.println(F("request: "));
  //Serial.println(req);

  // Match the request
  int val;
  if (req.indexOf(F("/gpio/0")) != -1) {
    val = 0;
  } else if (req.indexOf(F("/gpio/1")) != -1) {
    val = 1;
  } else {
    //Serial.println(F("invalid request"));
    val = digitalRead(LED_BUILTIN);
  }

  // Set LED according to the request
  digitalWrite(LED_BUILTIN, val);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }
  
  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n"));
  client.print(F("<style> h1 { color:  #4f0ad5;  font-size: 1.5em; font-family: Arial, Helvetica, sans-serif; }  h2{ color:   #f44996   !important;  font-size: 1.4em !important; font-family: Arial, Helvetica, sans-serif; } p { font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif; } button { border: none; border-radius: 200px; background-color:  #0262cf; color: #fbfbfb; transition: none !important; margin: 5 5px !important; padding: 5px;  transform: none !important; box-shadow: 1px 2px 4px 1px rgba(0, 0, 0, 0.3) !important;  } button:active { border: none; border-radius: 200px; transition: none !important; margin: 5 5px !important; padding: 5px;  box-shadow: none !important; transform: translate3d(1px, 1px, 1px) !important; }  .verde { background-color: #0b800b !important;} .amarillo { background-color: #c59100 !important;} .rojo { background-color: #e70f0f !important;} </style>\r\n"));
  client.print(F("<head>\r\n"));
  client.print(F("<meta http-equiv=\"refresh\" content=\"5\">\r\n"));
  client.print(F("</head>\r\n"));\
  client.print(F("<h1>WEMOS DEMO - "));
  client.print(hora);
  client.print(F(":"));
  client.print(minuto);
  client.print(F("</h1>\r\n"));
  client.print(F("<p>Temperatura ahora mismo: "));
  client.print(temperatura); 
  client.print(F(" C</p>\r\n"));
  client.print(F("<h2>Ha entrado alguien en la habitacion supersecreta? "));
  if(isIntruso == 0){
    client.print(F("NO"));
  } else {
    client.print(F("SI"));
  }
  client.print(F("</h2>\r\n"));  
  if(isEmergencia == 0){
    client.print(F("<button class=\"verde\">Toggle modo emergencia (ahora mismo: tranquilo"));
  } else {
    if(isEmergencia == 1){
      client.print(F("<button class=\"amarillo\">Toggle modo emergencia (ahora mismo: moderado"));
    } else {
      client.print(F("<button class=\"rojo\">Toggle modo emergencia (ahora mismo: EMERGENCIA"));
    }
  }
  client.print(F(")</button>\r\n"));
  client.print(F("<button>Apagar/encender todo por completo</button>\r\n"));
  client.print(F("</html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  //Serial.println(F("Disconnecting from client"));
}

void loop(){
  actualizarHora();
  web();
  uno_enviar();
  uno_recibir();
  delay(1000);
}
