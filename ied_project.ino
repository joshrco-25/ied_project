#include <Servo.h>    // Import the Servo library to control the servo
#define speakerPin 4
#define TrigPin1 11   // U/S1 Trig connected to pin 11
#define EchoPin1 10   // U/S1 Echo connected to pin 10

#define IR1Pin  A0   // A0 input pin for IR sensor signal
int IR1_Val = 0;     // to store IR sensor 1 signal value

Servo servo;          // Initialise the servo object to control the servo

int pos = 10;         // variable for the position of the servo

int dist;

void setup() {
  Serial.begin(9600);
  pinMode(TrigPin1, OUTPUT);  //Arduino's output, ranger's input
  pinMode(EchoPin1, INPUT);   //Arduino's input, ranger's output

  pinMode(speakerPin, OUTPUT);

  servo.attach(A1);    // Tell the servo object that we've connected to pin 8
  servo.write(0);     // Helps to adjust the initial position
  delay(40);  
}

void loop() {
  turn();
}

void turn(){
  // Make the pos variabele go from 0 to 180
  for (pos = 0; pos <= 180; pos++) {
    servo.write(pos);  // Set the position of the servo
    us();
    pathCheck();
    delay(5);         // Wait for 10ms for the servo to process the command
  }
  // // Make the pos variabele go from 180 to 0
  for (pos = 180; pos >= 0; pos--) {
    servo.write(pos);  // Set the position of the servo
    us();
    pathCheck();
    delay(5);         // Wait for 10ms for the servo to process the command
  }

  return 0;
}

//ultrasound
void us(){
  long pulseDuration; //variable needed by the ultrasound sensor code
  int Distance;       // Ultrasound distance in cm

  // this main code runs repeatedly:
  // 1. Produce a 10us (micro-second) HIGH pulse in Trig to trigger the sensor...
  digitalWrite(TrigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin1, LOW);

  // 2. Use pulseIn() function to measure the duration of the HIGH pulse in Echo 
  // every 58 us is an obstacle distance of 1 cm

  pulseDuration = pulseIn(EchoPin1, HIGH);
  Distance = pulseDuration / 58;  //  Convert to cm

  dist = Distance;

  // 3. display the obstacle distance in serial monitor 
  Serial.print("Distance = ");
  Serial.print(Distance);
  Serial.println(" cm");
  if(Distance < 50){
    Serial.println("Too close");
  }
  delay(5);

  us_buzz();
}

//detects obstacles with buzzer
void us_buzz(){
  if(dist < 10){
    for(int freq = 500; freq <= 1000; freq += 10){
      tone(speakerPin, freq);
      delay(5);
    }
  }
  else{
    noTone(speakerPin);
  }
}


//check for path / infrared
void pathCheck(){
    IR1_Val = analogRead(IR1Pin); // Reading and storing IR sensor 1 signal value
    Serial.print("Input Value:");
    Serial.print(IR1_Val);       // Printing IR sensor 1 signal value
    Serial.print("\n");            // moving to new line
    delay(5);
    ir_buzz();     
}


//will make sound if no path detected
void ir_buzz(){
  if(IR1_Val < 40){
    for(int freq = 1000; freq <= 2000; freq += 10){
      tone(speakerPin, freq);
      delay(2);
    }
    for(int freq = 2000; freq >= 1000; freq -= 10){
      tone(speakerPin, freq);
      delay(2);
    }
  }
  else{
    noTone(speakerPin);
  }
}






