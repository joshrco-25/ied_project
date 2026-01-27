/*The program show how to program the motor to turn left*/

#define motor_board_input_pin_IN2 6      //Connect to Arduino pin 6
#define motor_board_input_pin_IN1 9          //Connect to Arduino pin 9
#define motor_board_input_pin_IN4 3      //Connect to Arduino pin 3
#define motor_board_input_pin_IN3 5          //Connect to Arduino pin 5

#define TrigPin1 11   // U/S1 Trig connected to pin 11
#define EchoPin1 10   // U/S1 Echo connected to pin 10

int dist;

void setup() 
{
  Serial.begin(9600);
  pinMode(TrigPin1, OUTPUT);  //Arduino's output, ranger's input
  pinMode(EchoPin1, INPUT);   //Arduino's input, ranger's output

  pinMode(motor_board_input_pin_IN2, OUTPUT); 
  pinMode(motor_board_input_pin_IN1, OUTPUT); 
  pinMode(motor_board_input_pin_IN4, OUTPUT); 
  pinMode(motor_board_input_pin_IN3, OUTPUT); 
}

void loop() {
  us();
  forward();
}

void us(){
    long pulseDuration; //variable needed by the ultrasound sensor code
    int Distance;       // Ultrasound distance in cm

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
    delay(20);
}

void forward()
{
  if(dist > 40){
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 175);
    
    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 175);
  } 
  else{
    digitalWrite(motor_board_input_pin_IN2, HIGH);
    analogWrite(motor_board_input_pin_IN1, 255);

    digitalWrite(motor_board_input_pin_IN3, HIGH);
    analogWrite(motor_board_input_pin_IN4, 255);
  }
}
