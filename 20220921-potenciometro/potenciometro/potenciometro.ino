

const int ledPin = 5;
const int analogPin = 0;
int val = 0;

void setup() {
    // initialize serial:
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
                      // you're connected now, so print out the status
}


void loop() {
  val = analogRead(analogPin);
  
  analogWrite(ledPin, val/4);

  Serial.println(val/4);
}
