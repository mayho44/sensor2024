const int trigPin = 5;
const int echoPin = 6;
long duration;
float distance1 = 0.0, distance2 = 0.0, velocity = 0.0;
unsigned long time1 = 0, time2 = 0, previousMillis = 0;
const long interval = 50; 
int water;
#define ldr A0
#define led 2
#define irSensor 7 
#define buzzer 8 
#define redLight 12
#define yellowLight 10
#define greenLight 11

unsigned long lightPreviousMillis = 0;
int lightState = 0; // red=0,green=1,yellow=2
const long redDuration = 5000;
const long greenDuration = 5000;
const long yellowDuration = 2000;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
pinMode(3,OUTPUT);//relay output
pinMode(4,INPUT); //soil sensor
  pinMode(led, OUTPUT);
  pinMode(irSensor, INPUT);
  pinMode(buzzer, OUTPUT);
  
 
  pinMode(redLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(greenLight, OUTPUT);
}

void loop() {
 
   water = digitalRead(4);   
  if(water == HIGH) 
  {
  digitalWrite(3,LOW); 
  }
  else
  {
  digitalWrite(3,HIGH); 
  }
  calculateVelocity();

  // LDR and LED logic
  checkLDR();

  // Traffic light sequence
  updateTrafficLights();

  // Check IR sensor and control buzzer
  checkIRSensor();
}

void calculateVelocity() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Measure distance and time
    distance1 = distance2;
    distance2 = measureDistance();
    time1 = time2;
    time2 = currentMillis;

    // Calculate the change in distance and time
    unsigned long deltaTime = time2 - time1; // Time in milliseconds
    float deltaDistance = distance2 - distance1; // Distance in cm

    // Calculate velocity (cm/ms converted to cm/s)
    velocity = (deltaDistance / deltaTime) * 1000; // cm/s

    Serial.print("Velocity: ");
    Serial.print(abs(velocity));
    Serial.println(" cm/s");
    if (abs(velocity) > 40) {
      Serial.println("Speed limit exceeded");
    }
  }
}

float measureDistance() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  float distance = duration * 0.034 / 2;
  
  return distance;
}

void checkLDR() {
  if (analogRead(ldr) < 500) {
    digitalWrite(led, LOW);
  } else {
    digitalWrite(led, HIGH);
  }
}

void checkIRSensor() {
  if (lightState == 0 && digitalRead(irSensor) == LOW) { // If the red light is on and an obstacle is detected
    digitalWrite(buzzer, HIGH);
    delay(100); // Turn on the buzzer for a short time
    digitalWrite(buzzer, LOW);
  } else {
    digitalWrite(buzzer, LOW); // Ensure buzzer is off
  }
}

void updateTrafficLights() {
  unsigned long currentMillis = millis();

  if (lightState == 0 && currentMillis - lightPreviousMillis >= redDuration) {
    // Switch to Green light
    lightState = 1;
    lightPreviousMillis = currentMillis;
    setTrafficLights(LOW, LOW, HIGH);
  } else if (lightState == 1 && currentMillis - lightPreviousMillis >= greenDuration) {
    // Switch to Yellow light
    lightState = 2;
    lightPreviousMillis = currentMillis;
    setTrafficLights(LOW, HIGH, LOW);
  } else if (lightState == 2 && currentMillis - lightPreviousMillis >= yellowDuration) {
    // Switch to Red light
    lightState = 0;
    lightPreviousMillis = currentMillis;
    setTrafficLights(HIGH, LOW, LOW);
  }
}

void setTrafficLights(int redState, int yellowState, int greenState) {
  digitalWrite(redLight, redState);
  digitalWrite(yellowLight, yellowState);
  digitalWrite(greenLight, greenState);
}
