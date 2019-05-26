/* This code reads the speed of BLDC motor and publishes it. 
 * Arduino Uno has 2 interrupt pins. 
 * Each pin is connected to one motor. 
 * 
 */

uint8_t hall_a_motor_1 = 2;
uint8_t hall_a_motor_2 = 3;


uint8_t hall_a_last_val_m1 = 0;
uint8_t hall_a_last_val_m2 = 0;


unsigned long odometry_m1 = 0;
unsigned long odometry_m2 = 0;

unsigned long timer_start = 0;
unsigned long time_passed = 0;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  pinMode(hall_a_motor_1, INPUT);
  pinMode(hall_a_motor_2, INPUT);
 
  attachInterrupt(digitalPinToInterrupt(2), speedControl, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), odometryMotor2, CHANGE);
  hall_a_last_val_m1 = digitalRead(hall_a_motor_1);
  hall_a_last_val_m2 = digitalRead(hall_a_motor_2);
}

void speedControl(){

  unsigned long temp_time_passed = micros()-timer_start;
  unsigned long temp_timer_start = micros();

  uint8_t hall_a_val_m1;
  for (uint8_t i = 0; i < 100; i++){
    hall_a_val_m1 = digitalRead(hall_a_motor_1);
    if (hall_a_val_m1 == hall_a_last_val_m1) return;
  }
  hall_a_last_val_m1 = hall_a_val_m1;
  odometry_m1++;
  time_passed = temp_time_passed;
  timer_start = temp_timer_start;
  return;
}

void odometryMotor2(){

  uint8_t hall_a_val_m2;
  for (uint8_t i = 0; i < 100; i++){
    hall_a_val_m2 = digitalRead(hall_a_motor_2);
    if (hall_a_val_m2 == hall_a_last_val_m2) return;
  }
  odometry_m2++;
  hall_a_last_val_m2 = hall_a_val_m2;
  return;
}


void loop() {
  Serial.print("1M");
  Serial.print(odometry_m1);
  Serial.println("");
  
  Serial.print("2M");
  Serial.print(odometry_m2);
  Serial.println("");
  
  //Serial.println( 1000000.0 / 24.0 / 2.0 * 16.0 * 2.54 * 3.141592 / 100.0 / 1000.0 * 3600.0 / time_passed);
}
