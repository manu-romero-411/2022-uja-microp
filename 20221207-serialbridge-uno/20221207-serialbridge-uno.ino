#include <SoftwareSerial.h>

// VARIABLES COMUNES ENTRE UNO Y WEMOS
int hora;
int minuto;
float temperatura;
int isOnOff;
int isIntruso;

int minutoIntruso;
int horaIntruso;
int umbralHoraAlto;
int umbralHoraBajo;

int isEmergencia;

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

  // PONER EN MARCHA TOGGLE INVIERNO-VERANO

  // PONER EN MARCHA TEMPERATURA

  // PONER EN MARCHA DETECTAINTRUSOS
  
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
      Serial.print("Son las ");
      Serial.print(hora);
      Serial.print(":");
      Serial.println(minuto);
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

void medirTemperatura(){
  /*ESTO HAY QUE CAMBIARLO PARA AMOLDARLO AL SENSOR*/
  temperatura = 14.56;
  
  if(temperatura <= 5){ // UMBRAL DE FRÍO MODERADO
    isEmergencia = 1; 
  } else {
    if(temperatura <= 0){ // UMBRAL DE FRÍO EXTREMO
      isEmergencia = 2;
    } else {
      if(temperatura >= 20){ // UMBRAL DE CALOR MODERADO
         isEmergencia = 1; 
      } else {
        if(temperatura > 25){ // UMBRAL DE CALOR EXTREM0
          isEmergencia = 2;
        } else {
          isEmergencia = 0; // UMBRAL DE TEMPERATURA NORMAL
        }
      }
    }
  }
}

void detectarIntrusos(){
  /*ESTO HAY QUE CAMBIARLO PARA AMOLDARLO AL SENSOR*/
  int antes = isIntruso;
  if (true){ /*ULTRASONIDOS DETECTAN INTRUSO*/
    isIntruso = 1;
  }

  if(antes < isIntruso){ //SI ANTES NO HABÍA INTRUSO PERO AHORA SÍ
    minutoIntruso = minuto + 10;
    horaIntruso = hora;
  
    // CORREGIR HORA PARA CUANDO TENGA QUE SONAR EL BUZZER
    if(minutoIntruso > 60){
      minutoIntruso -= 60;
      horaIntruso += 1;
      if(horaIntruso > 23){
        horaIntruso = 0;
      }
    }
  
    Serial.print("Intruso a las ");
    Serial.print(horaIntruso);
    Serial.print(':');
    Serial.println(minutoIntruso);
  
    // AJUSTAR MODO DE EMERGENCIA EN FUNCIÓN DE UMBRALES DE HORA
    if(umbralHoraBajo > umbralHoraAlto){
      if((horaIntruso > umbralHoraBajo) || (horaIntruso < umbralHoraAlto)){
        isEmergencia = 1; 
      }
    } else {
      if(horaIntruso < umbralHoraAlto){
        isEmergencia = 1;
      }
    }
  } else {
    if(antes > isIntruso){ // SI ANTES HABÍA ALGUIEN Y AHORA NO
      isEmergencia = 0;
    }
  }
}

// LANZAR EMERGENCIA GRAVE SI EL INTRUSO SIGUE
void emergenciaIntrusos(){
  if((horaIntruso == hora) && (minutoIntruso == minuto)){
    isEmergencia = 2;
  }
}

void loop(){
  medirTemperatura();
  detectarIntrusos();
  wemos_enviar();
  wemos_recibir();
  delay(1000);
}
