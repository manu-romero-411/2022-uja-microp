const int ledPin1 = 3;
const int ledPin2 = 6;
const int ledPin3 = 9;
const int pot = 11;
const int ldr = A0;
volatile int toggle = LOW;
volatile int interrup = LOW;

void setup() {
    // initialize serial:
  Serial.begin(9600);
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  pinMode(pot,INPUT);
  pinMode(ldr,INPUT);
  attachInterrupt(0, parate, CHANGE);
}

void coche(){
  if(analogRead(ldr) < 900){
    digitalWrite(ledPin3, HIGH);
    delay(5000);
  } else {
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(750);
    digitalWrite(ledPin2, LOW);
    delay(750);
  }
}

void peaton(){
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, LOW);
    delay(1000);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin1, HIGH);
    delay(5000);
    digitalWrite(ledPin1, LOW);
    interrup = !interrup;
}

void parate(){
    interrup = !interrup;
}

void loop() {
  Serial.print(analogRead(ldr));
  Serial.println();
  if (interrup)
    coche();
  else
    peaton();
}
