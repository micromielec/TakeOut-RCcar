3#include "Arduino.h"
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle nh;
std_msgs::String msg;

String state;

int left_1 =2;
int left_2 =3;

int right_1 =6;
int right_2 =7;

int speed_motor_left=10;
int speed_motor_right=11;

void callback(const std_msgs::String& sub_cv){
  
  state=sub_cv.data;
  
  
  if (state == "RED"){ //stop
    Serial.println("stop");
    
    digitalWrite(left_1,LOW);
    digitalWrite(left_2,LOW);
    
    digitalWrite(right_1,LOW);
    digitalWrite(right_2,LOW);
    
    analogWrite(speed_motor_left,0);
    analogWrite(speed_motor_right,0);
    
    delay(100);
  }
  else if (state == "GREEN") {//go
    Serial.println("go");

    digitalWrite(left_1,LOW);
    digitalWrite(left_2,HIGH);
    
    digitalWrite(right_1,LOW);
    digitalWrite(right_2,HIGH);
    
    analogWrite(speed_motor_left,255);
    analogWrite(speed_motor_right,255); //min 0~ max 255
   
    delay(100);
  
  }
  
}

ros::Subscriber <std_msgs::String> sub("/cv_detection", callback);

void setup() {
  
  Serial.begin(57600); 
  nh.initNode();
  nh.subscribe(sub);
  
  pinMode(left_1,OUTPUT);
  pinMode(left_2,OUTPUT);
  pinMode(right_1,OUTPUT);
  pinMode(right_2,OUTPUT);
  pinMode(speed_motor_left,OUTPUT);
  pinMode(speed_motor_right,OUTPUT);
  
  
}

void loop() {
  nh.spinOnce();
  delay(10);
}
