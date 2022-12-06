/************************
 * Created: 21-May-2019 *
 * Author:  J.F.C.D.    *
 ************************/
 
#include "Alarm.h"

alarm ALM;
sensor PIR = sensor(A1);
sensor MW = sensor(A3);

void setup() {
  ALM.countdown(); 
  ALM.GSM_init();
}

void loop() {
  if(ALM.state == ON){
    if(ALM.monitoring == ON){
      if(ALM.detect(PIR,MW)){
        ALM.activate_ALERT();
      }
    } 
    else {
      ALM.alertTimeOnCheck();
    }
  }
  ALM.userSMSCheck();
}
