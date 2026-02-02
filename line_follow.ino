/*The program show how to program the motor to turn left*/

#define motor_board_input_pin_IN2 6     //RIGHT BLACK
#define motor_board_input_pin_IN1 9     //RIGHT RED
#define motor_board_input_pin_IN4 3     //LEFT RED
#define motor_board_input_pin_IN3 5     //LEFT BLACK

#define leftIR A0
#define rightIR A1

int left_val = 0;
int right_val = 0;

void setup() 
{
  Serial.begin(9600);

  pinMode(motor_board_input_pin_IN2, OUTPUT); 
  pinMode(motor_board_input_pin_IN1, OUTPUT); 
  pinMode(motor_board_input_pin_IN4, OUTPUT); 
  pinMode(motor_board_input_pin_IN3, OUTPUT);
}

void loop() {
  pathCheck();
  movement();
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
    analogWrite(motor_board_input_pin_IN4, 120);
  } 

  //right
  if(left_val > 800 && right_val < 40){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 120);
    
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 255);
  } 

  //stop
  if(left_val > 800 && right_val > 800){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 255);
    
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 255);
  } 
}





















