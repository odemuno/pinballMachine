/* This code controls the LED, buzzer, score board, sensors, and spinning obstacles
// Module 4 ECE 115 Spring 2022
// authors: Odemuno Ogelohwohor, Nicholas Rowlett, Sasha Kubichka
// Some references
// https://forum.arduino.cc/t/two-leds-using-millis/901634/
// https://create.arduino.cc/projecthub/SURYATEJA/use-a-buzzer-module-piezo-speaker-using-arduino-uno-89df45
*/

// switch to power most of the electronic circuits
#define swt 13

// motor pins for the spinning obstacles
#define motorPin1 10
#define motorPin2 11

// maximum score: the counter loops back to zero at this (so the score range is [0, MAX_SCORE-1] )
#define MAX_SCORE 100

// the analog reading above which a signal will be interpreted as "high" (score), 
// and below which it will be interpreted as "low" (no score).
#define PIEZO_THRESHOLD 100 
#define PROXIMITY_CUTOFF 200

// for the LED to display
#define DELAY 10

// mappings from digits to the corresponding binary code for the 7-segment display
// each bit refers to whether a particular segment is on or off
// if you want a visual mapping of the segment locations, look it up
#define   ZERO  B11111100
#define   ONE   B01100000
#define   TWO   B11011010
#define   THREE B11110010
#define   FOUR  B01100110
#define   FIVE  B10110110
#define   SIX   B10111110
#define   SEVEN B11100000
#define   EIGHT B11111110
#define   NINE  B11100110
#define   OFF   B00000000 // to reset when board is off

// the digits in an array, to allow them to be accessed
// so if a variable holds the number I want to print, I can do numCodes[var] to get the correct bit mapping
int numCodes[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

// input pin numbers
#define SER_L 22
#define RCLK_L 23
#define SRCLK_L 24

#define SER_R 25
#define RCLK_R 26
#define SRCLK_R 27

#define piezoPin1 A3 
#define piezoPin2 A4 
#define piezoPin3 A5 

#define proximityPin A0
#define proximityPin_reset A1

// output pin
#define ledoutput 8  // digital pin connected to LED
#define buzzoutput 9  // auditory feedback

//variable to keep track of the score
int counter;

// led pins
int led1 = 2;
int led2 = 3;
int led3 = 4;
int led4 = 5;
int led5 = 6;

// led state
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 60;                // interval at which to blink (milliseconds)
char ledState = 'A';                     // ledState used to set the LED

/* SET UP STAGE */
void setup() {
  
  //begin Serial (the debug text output)
  Serial.begin(9600);

  // initialize arduino pins
  pinMode(SER_L, OUTPUT);
  pinMode(RCLK_L, OUTPUT);
  pinMode(SRCLK_L, OUTPUT);
  pinMode(SER_R, OUTPUT);
  pinMode(RCLK_R, OUTPUT);
  pinMode(SRCLK_R, OUTPUT);  
  pinMode(ledoutput, OUTPUT);  
  pinMode(buzzoutput, OUTPUT); 
  pinMode(motorPin1, OUTPUT);   
  pinMode(motorPin2, OUTPUT);   
  pinMode(piezoPin1, INPUT);
  pinMode(piezoPin2, INPUT);
  pinMode(piezoPin3, INPUT);
  pinMode(proximityPin, INPUT);
  pinMode(proximityPin_reset, INPUT);
  pinMode(swt, INPUT);
  pinMode(led1, OUTPUT);   
  pinMode(led2, OUTPUT);  
  pinMode(led3, OUTPUT);    
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  
  //score starts at 0
  counter = 0;

}

/* FUNCTION DEFINITIONS */
/* This function blinks the 5 sets of LEDs without delay */
void blink_leds() {
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // if the LED A is on turn it off and vice-versa:
    if (ledState == 'A') {
      ledState = 'B';
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
    } else if (ledState == 'B') {
      ledState = 'C';
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
    } else if (ledState == 'C') {
      ledState = 'D';
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
    }else if (ledState == 'D') {
      ledState = 'E';
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, HIGH);
      digitalWrite(led5, LOW);
    }else if (ledState == 'E') {
      ledState = 'A';
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, HIGH);
  }
 }
}

/* This function turns the motors on */
void motors_on(){
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, HIGH);
}

/* This function counts the scores for when the piezo is hit by the ball, 
turns the green LED on and buzzes a sound */
void piezo_scoring(){
  
  counter += 5;                       // when the ball passes, it is +5 points   

  // LED
  digitalWrite(ledoutput, HIGH);
  delay(50);                          // to make the LED visible
 
  // BUZZER
  tone(buzzoutput, 2000);            // Send 1KHz sound signal
  delay(50);                         // for 1 sec
  noTone(buzzoutput);                // Stop sound
  delay(50);                         // for 1 sec
      
  //check and loop around to 0
  if(counter >= MAX_SCORE){
    counter = counter % MAX_SCORE;
  }
      
  //delay (milliseconds) to prevent seeing the same score several times
  delay(1);
}

/* This function counts the scores for when the proximity sensor is passed by the ball, 
turns the green LED on and buzzes a sound */
void proximity_scoring(){
  
  counter += 2; // when the ball passes these sensors, it is +2 points
  
  // LED
  digitalWrite(ledoutput, HIGH);
  delay(50);                          // to make the LED visible
 
  // BUZZER
  tone(buzzoutput, 2000);            // Send 1KHz sound signal
  delay(50);                         // for 1 sec
  noTone(buzzoutput);                // Stop sound
  delay(50);                         // for 1 sec
      
  //check and loop around to 0
  if(counter >= MAX_SCORE){
    counter = counter % MAX_SCORE;
  }
      
  //delay (milliseconds) to prevent seeing the same score several times
  delay(1);
}


/* This function displays the score from the counter on the 7-segment display*/
void display_score(){
  
  // output to the 7-segment display
  digitalWrite(RCLK_R, LOW);    // Force register clock to 0 
  shiftOut(SER_R, SRCLK_R, LSBFIRST, numCodes[counter%10]);   // Internally load serial data 
  digitalWrite(RCLK_R, HIGH);   // Rising edge to update outputs 
    
  digitalWrite(RCLK_L, LOW);    // Force register clock to 0 
  shiftOut(SER_L, SRCLK_L, LSBFIRST, numCodes[counter/10]);   // Internally load serial data 
  digitalWrite(RCLK_L, HIGH);   // Rising edge to update outputs 
    
  delay(DELAY);
}

/* This function turns off the LED, motors, resets counter and clears 7-segment display */
void turn_off() {

  // turn leds off
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);

  // turns motors off
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);

  // reset counter
  counter = 0;
  
  // clear the 7-segment display 
  digitalWrite(RCLK_R, LOW);    // Force register clock to 0 
  shiftOut(SER_R, SRCLK_R, LSBFIRST, OFF);   // Internally load serial data 
  digitalWrite(RCLK_R, HIGH);   // Rising edge to update outputs 
  
  digitalWrite(RCLK_L, LOW);    // Force register clock to 0 
  shiftOut(SER_L, SRCLK_L, LSBFIRST, OFF);   // Internally load serial data 
  digitalWrite(RCLK_L, HIGH);   // Rising edge to update outputs 
}

/* This function counts resets board when the proximity sensor is passed by the ball, 
turns the green LED on and buzzes a sound */
void reset(){

  // LED
  digitalWrite(ledoutput, HIGH);
  delay(50);                          // to make the LED visible
 
  // BUZZER
  tone(buzzoutput, 2000);            // Send 1KHz sound signal
  delay(50);                         // for 1 sec
  noTone(buzzoutput);                // Stop sound
  delay(50);                         // for 1 sec
      
  turn_off();
  
}


/* MAIN LOOP */
void loop() {

  // SWITCH IS ON
  if(digitalRead(swt) == 1) {
    
    blink_leds();

    motors_on();

    Serial.print("P1: ");
    Serial.println(piezoPin1);
    Serial.print("P2: ");
    Serial.println(piezoPin2);
    Serial.print("P3: ");
    Serial.println(piezoPin3);
    
    // PIEZOELECTRIC SENSOR
    if (analogRead(piezoPin1) >= PIEZO_THRESHOLD){
      piezo_scoring();
    } else{
      digitalWrite(ledoutput, LOW);
    }
  
    // PIEZO 2
    if (analogRead(piezoPin2) >= PIEZO_THRESHOLD){
      piezo_scoring();
    } else{
      digitalWrite(ledoutput, LOW);
    }
  
    //  PIEZO 3
    if (analogRead(piezoPin3) >= PIEZO_THRESHOLD){
      piezo_scoring();
    } else{
      digitalWrite(ledoutput, LOW);
    }
    
    // PROXIMITY SENSOR - middle of the board
    //if(analogRead(proximityPin) >= PROXIMITY_CUTOFF){
    //  proximity_scoring();
    //} else{
    //  digitalWrite(ledoutput, LOW);
    //}

    // PROXIMITY SENSOR - reset
    //if(analogRead(proximityPin_reset) >= PROXIMITY_CUTOFF){
      //reset();
   // } else{
     // digitalWrite(ledoutput, LOW);
    //}
  
    display_score();

     
   }

  // SWITCH OFF
  else{
    turn_off();
  }
}
