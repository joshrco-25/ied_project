//ULTRASOUND

#define TrigPin1 11   // U/S1 Trig connected to pin 11
#define EchoPin1 10   // U/S1 Echo connected to pin 10
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
                   // addr,en,rw,rs,d4,d5,d6,d7,bl,blpol
                   // addr can be 0x3F or 0x27

int counterA = 0;   //inialized CounterA as 0
int dist;

void setup() {
  // this setup code runs once only:
  Serial.begin(9600);
  pinMode(TrigPin1, OUTPUT);  //Arduino's output, ranger's input
  pinMode(EchoPin1, INPUT);   //Arduino's input, ranger's output

  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
}

void loop() {
  light();
  us();
  check();
}

void light(){
  lcd.setCursor(0, 0);    //set cursor to left top corner
  lcd.print("A score is:");
  lcd.print(counterA);
  if (counterA==100)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("A is winner");
    lcd.setCursor(0, 1);
    lcd.print("Reset to restart");
  }
  while((!digitalRead(4)))
  delay(500);
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
    delay(500);

    return Distance;
}

void check(){
  if(dist <= 50){
    counterA++;
  }
}


  
