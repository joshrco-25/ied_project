/*Testing_IR1.ino
Analog IR Sensor. This program is used to read and adjust IR sensor signal values*/

#define IR1Pin A0  // A0 input pin for IR sensor signal
int IR1_Val = 0;     // to store IR sensor 1 signal value

#define speakerPin A1

void setup() {
  Serial.begin(9600);   // Setup Serial Communication.               
  Serial.print("Analog IR Sensor 1.\n");  

  pinMode(speakerPin, OUTPUT);
}

void loop(){
  pathCheck();
}

void pathCheck(){
    IR1_Val = analogRead(IR1Pin); // Reading and storing IR sensor 1 signal value
    Serial.print("Input Value:");
    Serial.print(IR1_Val);       // Printing IR sensor 1 signal value
    Serial.print("\n");            // moving to new line
    delayMicroseconds(50);     
    buzzer();
}

void buzzer(){
  if(IR1_Val < 40){
    for(int freq = 1000; freq <= 2000; freq += 10){
      tone(speakerPin, freq);
      delay(1);
    }
    for(int freq = 2000; freq >= 1000; freq -= 10){
      tone(speakerPin, freq);
      delay(1);
    }
  }
  else{
    noTone(speakerPin);
  }
}








