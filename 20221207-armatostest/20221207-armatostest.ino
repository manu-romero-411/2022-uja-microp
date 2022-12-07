void setup(){
  Serial.begin(9600);
  pinMode(3,OUTPUT);
    pinMode(6,OUTPUT);
        pinMode(9,OUTPUT);


}

void bucleLed(){
  digitalWrite(3,HIGH);
  delay(200);
  digitalWrite(3,LOW);

    digitalWrite(6,HIGH);
  delay(200);
  digitalWrite(6,LOW);

    digitalWrite(9,HIGH);
  delay(200);
  digitalWrite(9,LOW);
}

void loop(){
bucleLed();
}
