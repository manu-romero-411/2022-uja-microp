const int ledPin=3;
const int sensor=A0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int valor = analogRead(sensor);
  float miliVoltios = (valor / 1023.0) * 5000;
  float grados = miliVoltios / 10; 
  Serial.print(grados);
  Serial.println(" C");
  delay(1000);
}
