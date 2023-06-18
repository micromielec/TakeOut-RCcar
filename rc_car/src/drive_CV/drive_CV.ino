#include "Arduino.h"
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle nh;
std_msgs::String msg;

String state;

int left_1 =2;  //왼쪽 모터 2번핀 연결
int left_2 =3;  //왼쪽 모터 3번핀 연결

int right_1 =6; //오른쪽 모터 6번핀 연결
int right_2 =7; //오른쪽 모터 7번핀 연결

int speed_motor_left=10;    //왼쪽 모터 pwm제어핀 10번
int speed_motor_right=11;    //오른쪽 모터 pwm제어핀 11번

void callback(const std_msgs::String& sub_cv){ //콜백 함수 생성
  
  state=sub_cv.data;   //카메라로부터 얻은 데이터 변수 state로 저장
  
  
  if (state == "RED"){ //만약 카메라로부터 red신호를 받을 경우 -> stop
    Serial.println("stop");
    
    digitalWrite(left_1,LOW);  //모터 정지
    digitalWrite(left_2,LOW);
    
    digitalWrite(right_1,LOW);
    digitalWrite(right_2,LOW);
    
    analogWrite(speed_motor_left,0);
    analogWrite(speed_motor_right,0);
    
    delay(100);
  }
  else if (state == "GREEN") { //만약 카메라로부터 green 신호를 받을 경우 -> go
    Serial.println("go");

    digitalWrite(left_1,LOW);   //모터 정방향 회전
    digitalWrite(left_2,HIGH);
    
    digitalWrite(right_1,LOW);
    digitalWrite(right_2,HIGH);
    
    analogWrite(speed_motor_left,255); //모터 속도를 최대로 설정
    analogWrite(speed_motor_right,255); //pwm제어는 min 0~ max 255 까지 가능
   
    delay(100);
  
  }
  
}

ros::Subscriber <std_msgs::String> sub("/cv_detection", callback);  //cv_detection 토픽 구독

void setup() {
  
  Serial.begin(57600); 
  nh.initNode();
  nh.subscribe(sub);
  
  pinMode(left_1,OUTPUT);   //출력모드 설정
  pinMode(left_2,OUTPUT);   //출력모드 설정
  pinMode(right_1,OUTPUT);   //출력모드 설정
  pinMode(right_2,OUTPUT);   //출력모드 설정
  pinMode(speed_motor_left,OUTPUT);   //출력모드 설정
  pinMode(speed_motor_right,OUTPUT);   //출력모드 설정

}

void loop() {
  nh.spinOnce();
  delay(10);
}
