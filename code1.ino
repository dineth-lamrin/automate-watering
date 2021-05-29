#include <Servo.h>

#define MOTOR_PIN   3
#define READ_PIN   A0
#define ANODE_PIN   4
#define VOLT_DROP   790       // change this value as suitalbe
#define SLEEP_FOR   8         //in hours
#define MILSECONDS_IN_MIN 60000
#define CHECKS_IN_A_CYCLE 3
#define MOTOR_SPEED 2

Servo m1;
int need_water();
void run_motor();
int check(); 

void setup(){
  pinMode(ANODE_PIN, OUTPUT);
  pinMode(READ_PIN, INPUT);
//  m1.attach(MOTOR_PIN);
  Serial.begin(9600);
  m1.write(0);

}

void loop(){
  check();
}

void run_motor(){
//  int delay_val = (11 - MOTOR_SPEED)*10;
  m1.attach(MOTOR_PIN);
  for (int i = 0; i < 90; i++){
    m1.write(i);
    delay(50);
  }
  delay(1000);
  for (int i = 90; i != 0; i--){
    m1.write(i);
    delay(50);
  }
  m1.detach();
  delay(2000);
  Serial.println("------ Motor ran successfully ------");
}

int need_water(){
  digitalWrite(ANODE_PIN, HIGH);   // turn on moisture sensor
  long avg = 0;
  int  val = 0;
  for (int i = 0; i < 10; i++) {
    // to get few readings before starts calculations
    val = analogRead(READ_PIN);
    Serial.println(val);
    delay(500);
  }
  for (int i = 0; i < 60; i++) {
    val = analogRead(READ_PIN);
    Serial.println(val);
    if (val > 1023 || val < 0){
      val = 0;
      i--;
    }
    avg += val;
    delay(1000);
  }
  avg = avg/60;
  Serial.print("avg: ");
  Serial.println(avg);
  if (avg > VOLT_DROP) {
    Serial.println("NEED WATER");
    return 1;
  }
  Serial.println("I'M FINE");
  return 0;
}

int check(){
  for (int i = 0; i < CHECKS_IN_A_CYCLE; i++){
    Serial.print("check ");
    Serial.println(i+1);
    if (need_water()){
      run_motor();
    }
  }
  digitalWrite(ANODE_PIN, LOW); //to turn off the  sensor
  for (int i = 0; i < SLEEP_FOR * 60; i++) delay(MILSECONDS_IN_MIN);
}
