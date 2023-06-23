/**The DESTROYER by.Ben */
#include <Servo.h>

#define echoPin 2
#define trigPin 3
#define servoPin 6
#define ledPin 13
#define buzzerPin 11
#define buttonPin 9

Servo trigger;

long duration;
int distance;
int firstDistnace;
int currentDistance;
int stoper;
int shotCount = 0;
int delayBetweenShots = 15;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  trigger.detach();
}

void loop() {
  getDistanceThreshold();  //get the distance threshold
  mainAction();            //the main action
  alarm();                 //turning the light on and making alarm sound
  wait();                  //waiting 15 seconds for reloading the gun
  readySound();            //playing sound that symbols readyness
}

// all the methods

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 29 / 2;
  return distance;
  delay(30);
}

void getDistanceThreshold() {
  if (stoper == 0) {
    analogWrite(ledPin, 0);
    for (int i = 0; i < 10; i++) {
      firstDistnace = getDistance();
      delay(200);
    }
    readySound();
    Serial.println(firstDistnace);
    stoper++;
  }
}

void mainAction() {
  while (shotCount == 0) {
    analogWrite(ledPin, 0);
    currentDistance = getDistance();
    Serial.println(distance);

    if ((currentDistance < (firstDistnace - 30) && currentDistance > 30 && currentDistance < 400)) {
      trigger.attach(servoPin);
      shot();
      shotCount++;
    }

    if (digitalRead(buttonPin) == 1) {
      trigger.attach(servoPin);
      shot();
      shotCount++;
    }
  }
}

void shot() {
  trigger.write(15);
  delay(500);
  trigger.write(0);
}

void alarm() {
  analogWrite(ledPin, 255);
  for (int i = 0; i < 10000; i++) {
    analogWrite(buzzerPin, 255);
    delayMicroseconds(100);
    analogWrite(buzzerPin, 0);
    delayMicroseconds(100);
  }
}

void wait() {
  trigger.detach();
  for (int i = 0; i < delayBetweenShots; i++) {
    delay(500);
    analogWrite(ledPin, 0);
    delay(500);
    analogWrite(buzzerPin, 255);
    delayMicroseconds(75);
    analogWrite(buzzerPin, 0);
    analogWrite(ledPin, 255);
    Serial.print("Time Left ");
    Serial.println(delayBetweenShots - i);
  }
}

void readySound() {
  for (int i = 0; i < 5000; i++) {
    analogWrite(buzzerPin, 255);
    delayMicroseconds(75);
    analogWrite(buzzerPin, 0);
    delayMicroseconds(75);
  }
  shotCount = 0;
}