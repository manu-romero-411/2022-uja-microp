float sp = 0;
float heart = 0;

void bucleLed(){
  digitalWrite(4,HIGH);
  delay(200);
  digitalWrite(4,LOW);

    digitalWrite(6,HIGH);
  delay(200);
  digitalWrite(6,LOW);

    digitalWrite(9,HIGH);
  delay(200);
  digitalWrite(9,LOW);
}

void setup() {
  Serial.begin(115200);
    pinMode(4,OUTPUT);
    pinMode(6,OUTPUT);
        pinMode(9,OUTPUT);
}
void loop()
{
  sp = 5 + random(5);
  heart = 100 + random(40);
  Serial.print(sp);
  Serial.print(",");
  Serial.println(heart);
  Serial.println("O B A M A");
  bucleLed();
  delay(400);
}
