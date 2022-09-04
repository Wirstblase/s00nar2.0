#include <Servo.h>
//trig-3 echo-2
Servo x_servo, y_servo;

int distanceFilter = 40; //how much to filter/normalize the distance value measured, used to reduce noise (min: 1, max: int max lol)

int echoPin = 2, trigPin = 3;

int rangeMinX = 45, rangeMaxX = 135; //the horizontal resolution of the "image" 
int rangeMinY = 60, rangeMaxY = 135; //the vertical resolution of the "image"

int calibX = 14, calibY = -11; // WARNING! Those values are UNIT SPECIFIC and UNIQUE
//offset for calibrating servos

float getDistance() {
  long duration;
  float distance = 0;

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;

  //returns the distance
  return distance;
}

int getDistanceAverage(int x) {
  float sum = 0, average = 0;
  float val[x];

  for (int i = 0; i < x; i++) {
    val[i] = getDistance();
  }
  for (int i = 0; i < x; i++) {
    sum = sum + val[i];
  }
  average = sum / x;
  return average;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

  x_servo.attach(10);
  y_servo.attach(11);

  x_servo.write(90 + calibX);
  y_servo.write(90 + calibY);
}

char recv = 0;
void loop() {

  if (Serial.available()) {
    recv = Serial.read();
    //Serial.print("received command: ");
    //Serial.println(recv);

    if (recv == 's') {
      //s = command for s c a n
      //Serial.println("begin_scan");
      for (int yPos = rangeMinY; yPos <= rangeMaxY; yPos++) {
        for (int xPos = rangeMinX; xPos <= rangeMaxX; xPos++) {
          x_servo.write(xPos+calibX);
          y_servo.write(yPos+calibY);
          float distance = getDistanceAverage(distanceFilter);
          Serial.print(xPos);
          Serial.print(":");
          Serial.print(yPos);
          Serial.print(":");
          Serial.println(distance);
        }
        delay(100);
      }
      //Serial.println("end_scan");
      
    }

  }

}
