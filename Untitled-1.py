#!/usr/bin/env python3

import rospy
from std_msgs.msg import String

import cv2

import numpy as np
cap = cv2.VideoCapture(0)       # 카메라 모듈 사용.


def cvpub():
    ret, img_color = cap.read()#   카메라 모듈 연속프레임 읽기
    height, width = img_color.shape[:2]
    img_color_blue = cv2.resize(img_color, (width, height), interpolation=cv2.INTER_AREA)

    hsv = cv2.cvtColor(img_color, cv2.COLOR_BGR2HSV)    # BGR을 HSV로 변환해줌

    # define range of blue color in HSV
    lower_green = np.array([100,100,120])          # 파랑색 범위
    upper_green = np.array([150,255,255])
    img_mask = cv2.inRange(hsv, lower_green, upper_green)     # 110<->150 Hue(색상) 영역을 지정.
    res = cv2.bitwise_and(img_color, img_color, mask=img_mask)      # 흰색 영역에 파랑색 마스크를 씌워줌.

    lower_red = np.array([100,100,120])          # 파랑색 범위
    upper_red = np.array([150,255,255])
    img_mask2 = cv2.inRange(hsv, lower_red, upper_red)     # 110<->150 Hue(색상) 영역을 지정.
    res = cv2.bitwise_and(img_color, img_color, mask=img_mask2)      # 흰색 영역에 파랑색 마스크를 씌워줌.
    
    if img_mask.any()== True:
    	print("hello")
    if else:
         print("world")
    

    if img_mask2.any()== True:
    	print("hello")
    else:
        print("no")
   
    
    # Bitwise-AND mask and original image
    pub = rospy.Publisher('cv_detection', String, queue_size=10)
    cv2.imshow('frame',img_color)       # 원본 영상을 보여줌
    cv2.imshow('Blue', res)           # 마스크 위에 파랑색을 씌운 것을 보여줌.
    if img_mask.any()== True:
        print("hello")
        pub.publish("Blue")

    else:
    	print("world")

if __name__=='__main__':
    
    rospy.init_node('study_cv')
    rate = rospy.Rate(30) #30hz
    while not rospy.is_shutdown():
        try:
            cvpub()
        except rospy.ROSInitException:
            pass
        rate.sleep()
        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break



















