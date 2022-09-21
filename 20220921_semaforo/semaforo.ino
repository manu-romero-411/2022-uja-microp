

const int ledPin1 = 3;
const int ledPin2 = 6;
const int ledPin3 = 9;


void setup() {
    // initialize serial:
  Serial.begin(9600);
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
                      // you're connected now, so print out the status
}


void loop() {
  
  digitalWrite(ledPin1, HIGH);
  delay(1000);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin3, HIGH);
  delay(1000);

  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin2, HIGH);
  delay(1000);

  digitalWrite(ledPin2, LOW);
}
