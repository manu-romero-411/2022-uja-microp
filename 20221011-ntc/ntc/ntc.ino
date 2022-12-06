
void setup() {
  Serial.begin(9600);
}

void ntc1() {
  int Ro = 2.5;
  int B =  3977; 
  int Rseries = 3;
  float To = 298.15; 
  const int ntc = A0;
  float Vi = analogRead(ntc) * (5.0 / 1023.0);
  float R = (Vi * Rseries) / (5 - Vi);
  float T =  1 / ((1 / To) + ((log(R / Ro)) / B));
  float Tc = T - 273.15; 
  
  Serial.println((String)"NTC:  " + Tc + "°C");
  delay(1000);
}

void lm35(){
  const int sensor=A1;
  int valor = analogRead(sensor);
  float miliVoltios = (valor / 1023.0) * 5000;
  float grados = miliVoltios / 10; 
  Serial.println((String)"LM35: " + grados + "ºC");
  delay(1000);
}

void loop(){
  int cont = 0;
  Serial.println((String)"== MEDICIÓN " + cont + "==");
  ntc1();
  lm35();
  Serial.println();
  cont = cont + 1;
}
