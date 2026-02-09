#include <Servo.h> 

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define motor_board_input_pin_IN2 6     //RIGHT BLACK
#define motor_board_input_pin_IN1 9     //RIGHT RED
#define motor_board_input_pin_IN4 3     //LEFT RED
#define motor_board_input_pin_IN3 5     //LEFT BLACK

#define leftIR A0
#define rightIR A1

#define speakerPin 4

#define TrigPin1 11   // U/S1 Trig connected to pin 11
#define EchoPin1 10   // U/S1 Echo connected to pin 10

#define TrigPin2 8   // U/S2 Trig connected to pin ___
#define EchoPin2 7   // U/S2 Echo connected to pin ___

Servo servo;

int left_val = 0;
int right_val = 0;

int dist;

int pos = 10;

bool itemDetected = false;

void setup() 
{
  Serial.begin(9600);

  //motor
  pinMode(motor_board_input_pin_IN2, OUTPUT); 
  pinMode(motor_board_input_pin_IN1, OUTPUT); 
  pinMode(motor_board_input_pin_IN4, OUTPUT); 
  pinMode(motor_board_input_pin_IN3, OUTPUT);

  //ultrasound sensor
  pinMode(TrigPin1, OUTPUT);  //Arduino's output, ranger's input
  pinMode(EchoPin1, INPUT);   //Arduino's input, ranger's output

  pinMode(TrigPin2, OUTPUT);  //Arduino's output, ranger's input
  pinMode(EchoPin2, INPUT);   //Arduino's input, ranger's output

  //buzzer
  pinMode(speakerPin, OUTPUT);

  //lcd
  pinMode(4, INPUT_PULLUP);
  PCICR |= B00000100; // Enable interrupts on PD port
  PCMSK2 |= B00010000; // Trigger interrupts on pins D4 (Hit a)
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();

  //servo
  servo.attach(2);    // Tell the servo object that we've connected to pin 8
  servo.write(0);     // Helps to adjust the initial position
  delay(200);
}

void loop() {
  turn();
}

//servo turn
void turn(){
  for (pos = 0; pos <= 180; pos += 10) {
    servo.write(pos);  // Set the position of the servo
    us();
    pathCheck();
    screen();
    delay(5); 
   
    if(dist < 10 || left_val > 800 && right_val > 800){
      servo.detach();
    } 
    else{
      servo.attach(2);
    }
  }

  // Make the pos variabele go from 180 to 0
  for (pos = 180; pos >= 0; pos -= 10) {
    servo.write(pos);  // Set the position of the servo
    us();
    pathCheck();
    screen();
    delay(5);    
    
    if(dist < 10 || left_val > 800 && right_val > 800){
      servo.detach();
    } 
    else{
      servo.attach(2);
    }
  }
  return 0;
}

void pathCheck(){
  left_val = analogRead(leftIR); // Reading and storing IR sensor 1 signal value
  // Serial.print("Left Input Value:");
  // Serial.print(left_val);       // Printing IR sensor 1 signal value
  // Serial.print("\n");            // moving to new line

  right_val = analogRead(rightIR); // Reading and storing IR sensor 1 signal value
  // Serial.print("Right Input Value:");
  // Serial.print(right_val);       // Printing IR sensor 1 signal value
  // Serial.print("\n");            // moving to new line

  delay(10);     

  movement();
}

void movement()
{
  //forward
  if(left_val < 40 && right_val < 40){
    analogWrite(motor_board_input_pin_IN2, 75);
    digitalWrite(motor_board_input_pin_IN1, LOW);
    
    digitalWrite(motor_board_input_pin_IN4, LOW);
    analogWrite(motor_board_input_pin_IN3, 75);

    Serial.print("Forward");
    Serial.println();
  } 

  //left
  if(left_val < 40 && right_val > 800){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 255);
    
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 140);

    Serial.print("Left");
    Serial.println();
  } 

  //right
  if(left_val > 800 && right_val < 40){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 140);
    
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 255);

    Serial.print("Right");
    Serial.println();
  }

  //backwards
  if(left_val > 800 && right_val > 800){
    digitalWrite(motor_board_input_pin_IN2, LOW);
    analogWrite(motor_board_input_pin_IN1, 160);

    digitalWrite(motor_board_input_pin_IN3, LOW);
    analogWrite(motor_board_input_pin_IN4, 70);

    Serial.print("Backward");
    Serial.println();

    delay(500);
  }

  detectObs(); 
}

//ultrasound sensing code
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
  // Serial.print("Distance = ");
  // Serial.print(Distance);
  // Serial.println(" cm");
  if(Distance < 10){
    Serial.println("Too close");
  }
  delay(5);
}


//detects obstacles
void detectObs(){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 255);
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 255);
    // if(dist < 10){
    //   for(int freq = 500; freq <= 1000; freq += 10){
    //   tone(speakerPin, freq);
    //   delay(5);
    // }
    // }
    // else{
    //   noTone(speakerPin);
    // }

}

//lcd
void screen(){
    itemDetect();
    lcd.setCursor(0, 0);
    if(itemDetected == true){
      lcd.clear();
      lcd.print("Item Detected");
    }
    else{
      lcd.clear();
      lcd.print("Nothing here");
      itemDetected = false;
    }
}

//detects if item is in box
void itemDetect(){
  long pulseDuration; //variable needed by the ultrasound sensor code
  int Distance;       // Ultrasound distance in cm

  digitalWrite(TrigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin2, LOW);

  pulseDuration = pulseIn(EchoPin2, HIGH);
  Distance = pulseDuration / 58;  //  Convert to cm

  Serial.print("Distance = ");
  Serial.print(Distance);
  Serial.println(" cm");

  if(Distance < 10){
    itemDetected = true;
  }
  else{
    itemDetected = false;
  }

  delay(50);
}

















