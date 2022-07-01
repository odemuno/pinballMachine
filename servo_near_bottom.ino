 #include <Servo.h>

// Create a new servo object:
Servo myservo;
#define servoPin 9
int angle = 0;
//sensor
#define PROXIMITY_CUTOFF 15
#define proximityPin_reset A1


void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin);
  pinMode(proximityPin_reset, INPUT);
}

// Pulse Sequence
void loop() {
 // PROXIMITY SENSOR
 Serial.println(analogRead(proximityPin_reset));
    if(analogRead(proximityPin_reset) >= PROXIMITY_CUTOFF){

      myservo.write(90);
      // And back from 180 to 0 degrees:
      //for (angle = 180; angle >= 0; angle -= 1) {
        //myservo.write(angle);
        //delay(30); // adjust speed of servo arm
      //}
      delay(1500);
      myservo.write(180);
      // Sweep from 0 to 180 degrees:
      //for (angle = 0; angle <= 180; angle += 1) {
        //myservo.write(angle);
        //delay(15);
      //}
       
      delay(1000);
    }
    
}
