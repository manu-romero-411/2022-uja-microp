const int ledPin1 = 3;
const int ledPin2 = 9;
const int ledPin3 = 6;
const int pot = A3;
const int ldr = A5;


void setup() {
    // initialize serial:
  Serial.begin(9600);
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  pinMode(pot,INPUT);
  pinMode(ldr,INPUT);                      // you're connected now, so print out the status
}


void loop() {
  Serial.print(analogRead(ldr));
  Serial.println();
  Serial.print(analogRead(pot));
  Serial.println();
  if ((analogRead(ldr) < 800) && (analogRead(pot) < 250)){
    digitalWrite(ledPin1, HIGH);
    delay(1000);
  
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin3, HIGH);
    delay(1000);
  
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(1000);
  
    digitalWrite(ledPin2, LOW);
  } else{
      digitalWrite(ledPin3, HIGH);
      delay(1000);
      digitalWrite(ledPin3, LOW);
      delay(1000);
  }
}
