#define echoPin 6 // Echo Pin
#define trigPin 7 // Trigger Pin
#define lm35pin A0
#define tempMin 24
#define tempMinMin 23
#define tempMax 26
#define tempMaxMax 27
#define led 9
#define buzz 5
// #define ledVerde 8


int maximumRange = 25; // Maximum range needed
int minimumRange = 1; // Minimum range needed
long duration, distance; // Duration used to calculate distance

void setup() {
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(buzz, OUTPUT);
 pinMode(led, OUTPUT);
 //Como encendemos nosotros el led verde?????
}

void loop(){
  luz();
  temperatura();
  sensorMovimiento();
}


void luz(){
  if (inicio >  00 && tTotal > 3){    //3 minutos
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led,LOW);
  }
}



void temperatura(){ // CAMBIAR PARA QUE DEVUELVA BOOL O INT CON VARIOS ESTADOS
  int valor = analogRead(lm35pin);
  float miliVoltios = (valor / 1023.0) * 5000;
  float grados = miliVoltios / 10;
  if(grados < tempMin || grados > tempMax){
    digitalWrite(led, HIGH);
    if(grados < tempMinMin || grados > tempMaxMax){
      digitalWrite(buzz, HIGH);
    } else {
      digitalWrite(buzz, LOW);
    }
  } else {
    digitalWrite(led, LOW);
    digitalWrite(buzz, LOW);
  }
  Serial.print(grados);
  Serial.println(" C");
  delay(50);
}



void sensorMovimiento() {
  readDistance();
  if(distance>minimumRange && distance < maximumRange){
    Serial.println(distance);
    digitalWrite(buzz, HIGH);
    digitalWrite(led, HIGH);
  } else {
    if (distance > maximumRange) {
      Serial.println("Out of range...");
      digitalWrite(buzz, LOW);
      digitalWrite(led, LOW);
    }
  }
  delay(50);
}



int readDistance(){
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 distance = duration/58.2;
}
