
#include <SoftwareSerial.h>

// VARIABLES COMUNES ENTRE UNO Y WEMOS
int hora;
int minuto;
float temperatura;
int isOnOff;
int isIntruso;
int isEmergencia;
int emergenciaPrevia;

/* OTRAS VARIABLES AUXILIARES */
// DETECCIÓN DE INTRUSOS SEGÚN HORA
int minutoIntruso = 61;
int umbralHoraAlto = 20;
int umbralHoraBajo = 18;
int espera = 1;

// SENSOR DE ULTRASONIDOS
long duration, distance; // Duration used to calculate distance
int maximumRange = 25; // Maximum range needed
int minimumRange = 1; // Minimum range needed

// SENSOR DE TEMPERATURA
int tempMin = 21;
int tempMinMin = 20;
int tempMax = 28;
int tempMaxMax = 31;

// INTERRUPTOR
/* DEFINICIÓN DE PUERTOS */
int ultraEcho = 8;
int ultraTrig = 9;
int buzzer = 12;
int lm35pin = A0;
int interruptor = 4;
int luzCasa = 5;
int led = 13; //Led connected to pin 13

String recibo;
bool stringComplete;


SoftwareSerial mySerial(6, 7); // RX, TX

void setup(){
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  Serial.begin(9600);
  mySerial.begin(9600);
  
  pinMode(led, OUTPUT);

  // PONER EN MARCHA BUZZER
  pinMode(buzzer, OUTPUT);


  // PONER EN MARCHA INTERRUPTOR
  pinMode(interruptor, INPUT);
  pinMode(luzCasa, OUTPUT);

  // PONER EN MARCHA TEMPERATURA

  // PONER EN MARCHA DETECTAINTRUSOS
  pinMode(ultraTrig, OUTPUT);
  pinMode(ultraEcho, INPUT);
}

// DIVIDIR STRINGS PARA TRANSFORMARLAS EN VARIABLES
// FUENTE: https://arduino.stackexchange.com/a/1237
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

// FLOTANTES A ARRAYS
// FUENTE: https://forum.arduino.cc/t/ftoa/63464
char *ftoa(double d, char *buffer, int precision) {
  long wholePart = (long) d;

  // Deposit the whole part of the number.

  itoa(wholePart,buffer,10);

  // Now work on the faction if we need one.

  if (precision > 0) {

    // We do, so locate the end of the string and insert
    // a decimal point.

    char *endOfString = buffer;
    while (*endOfString != '\0') endOfString++;
    *endOfString++ = '.';

    // Now work on the fraction, be sure to turn any negative
    // values positive.

    if (d < 0) {
      d *= -1;
      wholePart *= -1;
    }
    
    double fraction = d - wholePart;
    while (precision > 0) {

      // Multipleby ten and pull out the digit.

      fraction *= 10;
      wholePart = (long) fraction;
      *endOfString++ = '0' + wholePart;

      // Update the fraction and move on to the
      // next digit.

      fraction -= wholePart;
      precision--;
    }
    // Terminate the string.
    *endOfString = '\0';
  }
  return buffer;
}

void wemos_recibir(){
  if(mySerial.available() > 0){
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
      hora = getValue(recibo, ':', 0).toInt();
      minuto = getValue(recibo, ':',1).toInt();
      isOnOff = getValue(recibo, ':', 2).toInt();
      Serial.print("Son las ");
      Serial.print(isOnOff);
      //Serial.print(":");
      //Serial.println(minuto);
      stringComplete = false;
      recibo = "";
    }
  }
}

void wemos_enviar(){
  // HACER LA CONVERSIÓN A ARRAY
  char tempe[7];
  char onoff[7];
  char intru[7];
  char emerg[7];

  ftoa(temperatura,tempe,10);
  itoa(isOnOff,onoff,10);
  itoa(isIntruso,intru,10);
  itoa(isEmergencia,emerg,10);

  
  // ENVIARLO POR PUERTO SERIE VIRTUAL AL WEMOS
  mySerial.write(tempe);
  mySerial.write(':');
  mySerial.write(onoff);
  mySerial.write(':');
  mySerial.write(intru);
  mySerial.write(':');
  mySerial.write(emerg);
  mySerial.write('_');
}

void triggerEmergencia(){
  isEmergencia = emergenciaPrevia;
  emergenciaPrevia = 0;
  if(isEmergencia == 1){
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, LOW);
  } else {
    if (isEmergencia == 2){
      digitalWrite(led, HIGH);
      digitalWrite(buzzer, HIGH);
    } else {
      if (isEmergencia == 0){
        digitalWrite(led, LOW);
        digitalWrite(buzzer, LOW);
      }
    }
  }
}

bool luz(){
  if (digitalRead(interruptor) == HIGH){
    digitalWrite(luzCasa, HIGH);
    return true;
  } else {
    digitalWrite(luzCasa, LOW);
    return false;
  }
}

void detectarIntrusos(){
  if (luz() == 1){ /*ULTRASONIDOS DETECTAN INTRUSO*/
   isIntruso = 0;
   if(umbralHoraBajo > umbralHoraAlto){
      if((hora > umbralHoraBajo) || (hora < umbralHoraAlto)){
        Serial.println("Emergencia GRAVE por intruso en habitación encendida");
        isIntruso = 1;
        emergenciaPrevia = 2;
      }
    } else {
      if(hora < umbralHoraAlto){
        Serial.println("Emergencia GRAVE por intruso en habitación encendida");
        isIntruso = 1;
        emergenciaPrevia = 2;
      }
    }
  } else {
    Serial.println("Emergencia NULA por intruso en habitación encendida");
    if (emergenciaPrevia < 1) emergenciaPrevia = 0;
  }
}

void sensorMovimiento() {
  if (luz() == 0){
    isIntruso = 1;
    readDistance();
    if(distance>minimumRange && distance < maximumRange){
      Serial.println("Emergencia GRAVE por intruso");
      emergenciaPrevia = 2;
    } else {
      if (distance > maximumRange) {
        Serial.println("Emergencia NULA por intruso");
        isIntruso = 0;
        if (emergenciaPrevia < 1) emergenciaPrevia = 0;
      }
    }
    delay(50);
  }
}

int readDistance(){
  digitalWrite(ultraTrig, LOW);
  delayMicroseconds(2);
  
  digitalWrite(ultraTrig, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(ultraTrig, LOW);
  duration = pulseIn(ultraEcho, HIGH);
  
  distance = duration/58.2;
}

void sensorTemperatura(){ // CAMBIAR PARA QUE DEVUELVA BOOL O INT CON VARIOS ESTADOS
  int valor = analogRead(lm35pin);
  float miliVoltios = (valor / 1023.0) * 5000;
  temperatura = miliVoltios / 10;
  Serial.println();
  if(temperatura < tempMin || temperatura > tempMax){
    if(temperatura < tempMinMin || temperatura > tempMaxMax){
      Serial.println("Emergencia GRAVE por temperatura");
      emergenciaPrevia = 2;
    } else {
      Serial.println("Emergencia LEVE por temperatura");
      if (emergenciaPrevia < 2) emergenciaPrevia = 1;
    }
  } else {
    Serial.println("Emergencia NULA por temperatura");
    if (emergenciaPrevia < 1) emergenciaPrevia = 0;
  }
 
  //Serial.print(temperatura);
  //Serial.println(" C");
  delay(50);
}


void loop(){
  //if(isOnOff == 0){
    triggerEmergencia();
  //}
  wemos_enviar();
  wemos_recibir();
  //Serial.print(isOnOff);
  //if(isOnOff == 0){
    detectarIntrusos();
    sensorMovimiento();
    sensorTemperatura();
  //} else {
   // luz();
   // emergenciaPrevia = 0;
   // triggerEmergencia();
  //}
  delay(1000);
}
