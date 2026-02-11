//TESTING

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);


#define motor_board_input_pin_IN2 6     //RIGHT BLACK
#define motor_board_input_pin_IN1 9     //RIGHT RED

#define motor_board_input_pin_IN4 3     //LEFT RED
#define motor_board_input_pin_IN3 5     //LEFT BLACK

#define leftIR A0
#define rightIR A1

#define speakerPin A3

#define TrigPin1 11   // U/S1 Trig connected to pin 11
#define EchoPin1 10   // U/S1 Echo connected to pin 10

#define TrigPin2 8  
#define EchoPin2 7  

int left_val = 0;
int right_val = 0;

int dist;

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

  pinMode(TrigPin2, OUTPUT); 
  pinMode(EchoPin2, INPUT);  

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

}

void loop() {
  pathCheck();
  detectObs();
  screen();
}

void pathCheck(){
  left_val = analogRead(leftIR); // Reading and storing IR sensor 1 signal value
  Serial.print("Left Input Value:");
  Serial.print(left_val);       // Printing IR sensor 1 signal value
  Serial.print("\n");            // moving to new line

  right_val = analogRead(rightIR); // Reading and storing IR sensor 1 signal value
  Serial.print("Right Input Value:");
  Serial.print(right_val);       // Printing IR sensor 1 signal value
  Serial.print("\n");            // moving to new line

  delay(10);     

  movement();
}

void movement()
{
  //forward
  if(left_val < 40 && right_val < 40){
    analogWrite(motor_board_input_pin_IN1, 150);
    digitalWrite(motor_board_input_pin_IN2, LOW);
    
    digitalWrite(motor_board_input_pin_IN4, LOW);
    analogWrite(motor_board_input_pin_IN3, 150);
  } 

  //left
  if(left_val < 40 && right_val > 800){
    digitalWrite(motor_board_input_pin_IN1, LOW);
    analogWrite(motor_board_input_pin_IN2, 50);
    
    digitalWrite(motor_board_input_pin_IN4, HIGH);
    analogWrite(motor_board_input_pin_IN3, HIGH);
  } 

  //right
  if(left_val > 800 && right_val < 40){
    digitalWrite(motor_board_input_pin_IN1, HIGH);
    analogWrite(motor_board_input_pin_IN2, HIGH);
    
    digitalWrite(motor_board_input_pin_IN4, LOW);
    analogWrite(motor_board_input_pin_IN3, 50);
  } 

  //backwards
  if(left_val > 800 && right_val > 800){
    digitalWrite(motor_board_input_pin_IN1, LOW);
    analogWrite(motor_board_input_pin_IN2, 150);

    analogWrite(motor_board_input_pin_IN4, 255);
    digitalWrite(motor_board_input_pin_IN3, LOW);
  } 

  if(itemDetected == false){
    digitalWrite(motor_board_input_pin_IN2, 1);
    digitalWrite(motor_board_input_pin_IN1, 1);

    digitalWrite(motor_board_input_pin_IN3, 1);
    digitalWrite(motor_board_input_pin_IN4, 1);

    Serial.print("Stopped");
  }

  us();
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
  Serial.print("Distance = ");
  Serial.print(Distance);
  Serial.println(" cm");
  delay(5);
}


//detects obstacles
void detectObs(){
  if(dist < 20){
    
    Serial.println("Obstacle detected");

    digitalWrite(motor_board_input_pin_IN2, 1);
    digitalWrite(motor_board_input_pin_IN1, 1);

    digitalWrite(motor_board_input_pin_IN3, 1);
    digitalWrite(motor_board_input_pin_IN4, 1);

    for(int freq = 500; freq <= 1000; freq += 4){
      tone(speakerPin, freq);
      delay(5);
    }
  }
  else{
    noTone(speakerPin);
  }
}


//lcd
void screen(){
    itemDetect();
    lcd.setCursor(0, 0);

    if(dist < 20){
      lcd.clear();
      lcd.print("Obstacle detected");
    }
    else{
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
}

//detects if item is in box
void itemDetect(){
  long pulseDuration; //variable needed by the ultrasound sensor code
  int Distance;       // Ultrasound distance in cm

  digitalWrite(TrigPin2, 0);
  delayMicroseconds(2);
  digitalWrite(TrigPin2, 1);
  delayMicroseconds(10);
  digitalWrite(TrigPin2, 0);

  pulseDuration = pulseIn(EchoPin2, HIGH);
  Distance = pulseDuration / 58;  //  Convert to cm

  // Serial.print("Distance = ");
  // Serial.print(Distance);
  // Serial.println(" cm");

  if(Distance < 10){
    itemDetected = true;
  }
  else{
    itemDetected = false;
  }
}




















