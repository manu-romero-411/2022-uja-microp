/************************
 * Created: 21-May-2019 *
 * Author:  J.F.C.D.    *
 ************************/

#ifndef ALARM_H
#define ALARM_H

#include <SoftwareSerial.h>
#include "Config.h"

class sensor{
public:
    int output;
    sensor(int pin);
};

class alarm {
public:
  bool state;
  bool monitoring;
  unsigned long triggerTime;
  SoftwareSerial *SIM800L;

  alarm();
  void countdown();
  bool detect(sensor s1,sensor s2);
  void activate_ALERT();
  void RESET();
  void switch_ON();
  void switch_OFF();  
  void alertTimeOnCheck();
  void beep(int msON, int msOFF, int rep);
  void GSM_init();
  void updateSerial();
  void sendSMS();
  void userSMSCheck();
  bool offSMS();
};

#endif
