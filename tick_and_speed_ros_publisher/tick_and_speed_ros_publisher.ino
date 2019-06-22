/* 
 * rosserial::std_msgs::Float64 Test
 * Receives a Float64 input, subtracts 1.0, and publishes it
 */

#include <ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/UInt64.h>

ros::NodeHandle nh;

float x; 
uint64_t tick_counter;
float speed_kmh;

uint8_t hall_a_motor_1 = 2;
uint8_t hall_a_last_val_m1 = 0;

unsigned long timer_start = 0;
unsigned long time_passed = 0;


std_msgs::UInt64 tick_msg;
std_msgs::Float64 speed_msg;


ros::Publisher TickPublisher("left_wheel/motor_tick",&tick_msg);
ros::Publisher SpeedPublisher("left_wheel/speed",&speed_msg);

void setup()
{
  Serial.begin(9600);
  pinMode(hall_a_motor_1, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(2), speedControl, CHANGE);
  
  hall_a_last_val_m1 = digitalRead(hall_a_motor_1);
  
  nh.initNode();
  nh.advertise(TickPublisher);
  nh.advertise(SpeedPublisher);

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
  tick_counter++;
  time_passed = temp_time_passed;
  timer_start = temp_timer_start;
  return;
}

void loop()
{
  speed_kmh =  1000000.0 / 24.0 / 2.0 * 16.0 * 2.54 * 3.141592 / 100.0 / 1000.0 * 3600.0 / time_passed;
  speed_msg.data = speed_kmh;

  tick_msg.data = tick_counter;

  TickPublisher.publish(&tick_msg);

  SpeedPublisher.publish(&speed_msg);
 
  nh.spinOnce();
  delay(50);
}
