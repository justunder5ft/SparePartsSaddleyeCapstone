#!/usr/bin/env python2
# MIT License
# Copyright (c) 2019 JetsonHacks
# See license
# Using a CSI camera (such as the Raspberry Pi Version 2) connected to a 
# NVIDIA Jetson Nano Developer Kit using OpenCV
# Drivers for the camera and OpenCV are included in the base image
#Dependencies:
# 		predict.py

import cv2
from predict import predictImage
from datetime import datetime
import time
import os
import sys, getopt, smbus, math, importlib
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/Jetson/GPIO/")
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/Jetson/")
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/RPi/GPIO/")
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/RPi/")
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/")
import gpio as GPIO

save_path = '/home/saddleye/Pictures/' # parent directory for images
list1 = os.listdir(save_path) # get all the files in save path
num_of_files = len(list1) #count the number of files in save path
#print ("Number of files : " + str(num_of_files))
save_dir = os.path.join(save_path, "Run_" + str(num_of_files)) # working directory for images
if not os.path.exists(save_dir): # if the folder does not exsist 
    os.mkdir(save_dir) # make it

# Pin Definitons, as wired on the board of the demo units
led_1 = 12
led_2 = 13
led_3 = 15
led_4 = 16
led_5 = 18
but_blk = 22  # Black button pin

# gstreamer_pipeline returns a GStreamer pipeline for capturing from the CSI camera
# Defaults to 1280x720 @ 60fps 
# Flip the image by setting the flip_method (most common values: 0 and 2)
# display_width and display_height determine the size of the window on the screen
cap_height = 720
cap_width = 1280
framerate = 21
display_width = 1280
display_height = 720
flip_method = 0
image_storing_interval = 5 # every 5 seconds (+ 1 sec for led output) take a picture

def gstreamer_pipeline () :   # need this function for camera to work
    return ('nvarguscamerasrc ! ' 
    'video/x-raw(memory:NVMM), '
    'width=(int)%d, height=(int)%d, '
    'format=(string)NV12, framerate=(fraction)%d/1 ! '
    'nvvidconv flip-method=%d ! '
    'video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! '
    'videoconvert ! '
    'video/x-raw, format=(string)BGR ! appsink'  % (cap_width,cap_height,framerate,flip_method,display_width,display_height))

def take_picture(img,i): # pass (the current caputer, the image number to be saved)
    now = datetime.now()# get the day and time
    current_datetime = now.strftime("%d-%m-%Y_%H-%M-%S") # create file string
    #cv2.imwrite(os.path.join(save_dir, "pic_" + str(i) + "_Run_" + str(num_of_files) + '.jpg'),img) #write the img

def show_camera():
    GPIO.setmode(GPIO.BOARD)  # BOARD pin-numbering scheme
    GPIO.setup(but_blk, GPIO.IN)  # button pin set as input
    GPIO.add_event_detect(but_blk, GPIO.RISING)
    GPIO.setup(led_1, GPIO.OUT)
    GPIO.output(led_1, GPIO.LOW)
    GPIO.setup(led_2, GPIO.OUT)
    GPIO.output(led_2, GPIO.LOW)
    GPIO.setup(led_3, GPIO.OUT)
    GPIO.output(led_3, GPIO.LOW)
    GPIO.setup(led_4, GPIO.OUT)
    GPIO.output(led_4, GPIO.LOW)
    GPIO.setup(led_5, GPIO.OUT)
    GPIO.output(led_5, GPIO.LOW)


    # To flip the image, modify the flip_method parameter (0 and 2 are the most common)
    #print gstreamer_pipeline()
    cap = cv2.VideoCapture(gstreamer_pipeline(), cv2.CAP_GSTREAMER) # start the capture
    if cap.isOpened():


        #window_handle = cv2.namedWindow('CSI Camera', cv2.WINDOW_AUTOSIZE)
        # Window 
        i = 0
        while True: #cv2.getWindowProperty('CSI Camera',0) >= 0:
            ret_val, img = cap.read(); # read the capture
            #cv2.imshow('CSI Camera',img) # for testing show on screen

            keyCode = cv2.waitKey(30) & 0xff
            # Stop the program on the ESC key
            if keyCode == 27: #ESC key
                break
            if GPIO.event_detected(but_blk): # exit if the black button is hit
                GPIO.output(led_1, GPIO.HIGH) # show user the process has ended
                GPIO.output(led_2, GPIO.HIGH)
                GPIO.output(led_4, GPIO.HIGH)
                GPIO.output(led_5, GPIO.HIGH)
                time.sleep(0.50)
                GPIO.output(led_1, GPIO.LOW)
                GPIO.output(led_2, GPIO.LOW)
                GPIO.output(led_4, GPIO.LOW)
                GPIO.output(led_5, GPIO.LOW)
                GPIO.remove_event_detect(but_blk)
                time.sleep(0.50)
                break
            GPIO.output(led_3, GPIO.HIGH)# heartbeat
            time.sleep(0.50)
            GPIO.output(led_3, GPIO.LOW)# heartbeat
            time.sleep(0.50)
            take_picture(img,i) # take the pic
            predictImage(img) # send the picture to the model and predict it
            i+=1
            time.sleep(image_storing_interval)# wait
        cap.release()
        cv2.destroyAllWindows()
    else:
        print('Unable to open camera')


if __name__ == '__main__':
    show_camera()
