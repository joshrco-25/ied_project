#define motor_board_input_pin_IN2 6     //RIGHT BLACK
#define motor_board_input_pin_IN1 9     //RIGHT RED
#define motor_board_input_pin_IN4 3     //LEFT RED
#define motor_board_input_pin_IN3 5     //LEFT BLACK

#define leftIR A0
#define rightIR A1

#define speakerPin 4
#define TrigPin1 11   // U/S1 Trig connected to pin 11
#define EchoPin1 10   // U/S1 Echo connected to pin 10

int left_val = 0;
int right_val = 0;

int dist;

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

  //buzzer
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  pathCheck();
  movement();
  us();
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

  delay(20);     
}

void movement()
{
  //forward
  if(left_val < 40 && right_val < 40){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 175);
    
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 175);
  } 

  //left
  if(left_val < 40 && right_val > 800){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 255);
    
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 140);
  } 

  //right
  if(left_val > 800 && right_val < 40){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 140);
    
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 255);
  } 

  //backwards
  if(left_val > 800 && right_val > 800){
    digitalWrite(motor_board_input_pin_IN2, LOW);
    analogWrite(motor_board_input_pin_IN1, 150);
    digitalWrite(motor_board_input_pin_IN3, LOW);
    analogWrite(motor_board_input_pin_IN4, 70);

    delay(200);
  } 
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
  if(Distance < 50){
    Serial.println("Too close");
  }
  delay(5);

  detectObs();
}


//detects obstacles
void detectObs(){
  if(dist < 10){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 255);
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 255);

    for(int freq = 500; freq <= 1000; freq += 10){
      tone(speakerPin, freq);
      delay(5);
    }
  }
  else{
    noTone(speakerPin);
  }
}





















