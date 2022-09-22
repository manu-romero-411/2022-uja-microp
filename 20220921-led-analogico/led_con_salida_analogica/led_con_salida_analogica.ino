

const int ledPin = 5;
int val = 0;

void setup() {
    // initialize serial:
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
                      // you're connected now, so print out the status
}


void loop() {
  if(val <= 255){
    analogWrite(ledPin, val);
  }
  val = val + 10;
  delay(1000);
  Serial.println(val);
}
