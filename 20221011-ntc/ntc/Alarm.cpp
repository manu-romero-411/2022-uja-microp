/************************
 * Created: 21-May-2019 *
 * Author:  J.F.C.D.    *
 ************************/

#include "Alarm.h"

alarm::alarm() {
  state = ON;
  monitoring = ON;
  SIM800L = new SoftwareSerial(RX_pin, TX_pin);
  pinMode(BUZZER, OUTPUT);
  pinMode(ALERT_SIGNAL, OUTPUT);    
}

void alarm::countdown(){
  for(int i=ACTIVATION_DELAY;i>1;i--){
    if(i>10){
      beep(50,50,1);
      delay(100);
    }else{
      beep(50,50,2);
    }
    delay(900);
  }
  beep(50, 50, 10);
}

bool alarm::detect(sensor s1,sensor s2){
  bool detection = 0;
  if (digitalRead(s1.output) == HIGH){
    if (digitalRead(s2.output) == HIGH) {
      detection = 1;
    }
  }
  return detection;
}

void alarm::activate_ALERT() {
  delay(ALERT_DELAY*1000);
  digitalWrite(ALERT_SIGNAL, HIGH);
  triggerTime = millis();
  monitoring = OFF;
  delay(SMS_DELAY*1000);
  sendSMS();
}

void alarm::switch_ON(){
  state = ON;
  monitoring = ON;
}

void alarm::switch_OFF(){
  digitalWrite(ALERT_SIGNAL, LOW);
  state = OFF;
}

void alarm::RESET(){
  digitalWrite(ALERT_SIGNAL, LOW);
  monitoring = ON;
}

void alarm::alertTimeOnCheck(){
  unsigned long duration = ALERT_DURATION;
  unsigned long currentTime = millis();
  if (currentTime > (triggerTime + duration*1000)){
    RESET();
  }
  delay(3000);
}

void alarm::beep(int msON, int msOFF, int rep) {
  for (int i = 0; i < rep; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(msON);//wait for X ms
    digitalWrite(BUZZER, LOW);
    delay(msOFF);
  }
}

void alarm::GSM_init(){
  Serial.begin(SERIAL_BAUDRATE);
  SIM800L->begin(GSM_BAUDRATE);
  SIM800L->println("AT");
  updateSerial();
  SIM800L->println("AT+CMGF=1");
  delay(100);
  SIM800L->println("AT+CNMI=1,2,0,0,0");
  SIM800L->println("AT+CSQ");
  updateSerial();
  SIM800L->println("AT+CCID");
  updateSerial();
  SIM800L->println("AT+CREG?");
  updateSerial();
}

void alarm::updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    SIM800L->write(Serial.read());
  }
  if (SIM800L->available()){
    while(SIM800L->available())
    {
      Serial.write(SIM800L->read());
    } 
  }
}

void alarm::sendSMS(){
  SIM800L->println("AT");
  updateSerial();
  delay(500);
  /* NOTA: +PPxxxxxxxxx = Número de telefono del usuario con prefijo de país (+34 en España) (ejemplo: +34600700800) */
  SIM800L->println("AT+CMGS=\"+PPxxxxxxxxx\"");
  updateSerial();
  SIM800L->print("SE HA ACTIVADO LA ALARMA\n\"Reset\"-> Reiniciar alarma\n\"Off\"-> Apagar alarma\n\"On\"-> Encender alarma");
  SIM800L->write(26);
  updateSerial();
}

void alarm::userSMSCheck(){
  String incomingSMS;
  if(SIM800L->available()){
    incomingSMS = SIM800L->readString();
}
  if(incomingSMS.indexOf("Reset")>=0){
    RESET();
  }  
  if(incomingSMS.indexOf("Off")>=0){
    switch_OFF();
  }  
  if(incomingSMS.indexOf("On")>=0){
    switch_ON();
  }  

}

sensor::sensor(int pin){
    pinMode(pin, INPUT);
    output = pin;
}
