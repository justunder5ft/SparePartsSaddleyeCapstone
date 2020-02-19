#!/usr/bin/env python2
# MIT License
# Copyright (c) 2019 JetsonHacks
# See license
# Using a CSI camera (such as the Raspberry Pi Version 2) connected to a 
# NVIDIA Jetson Nano Developer Kit using OpenCV
# Drivers for the camera and OpenCV are included in the base image


import os
import sys, getopt, smbus, math, importlib
import time
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/Jetson/GPIO/")
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/Jetson/")
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/RPi/GPIO/")
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/RPi/")
sys.path.append("/opt/nvidia/jetson-gpio/lib/python/")
import gpio as GPIO # Need all the sys.path stuff to make the GPIO work
import VideoGather

but_red = 21  # Board pin 21 (20 i a ground)
but_blk = 22  # Board pin 22
led_5 = 18
#cameraRunning = False # is our camera on?

def blk_action():
    print("do stuff here")
    

def red_action():
    VideoGather.show_camera()
    GPIO.output(led_5, GPIO.HIGH)

def control():
    GPIO.setmode(GPIO.BOARD)  # BOARD pin-numbering scheme
    GPIO.setup(but_red, GPIO.IN)  # button pin set as input
    #GPIO.setup(but_blk, GPIO.IN)  # button pin set as input
    GPIO.add_event_detect(but_red, GPIO.RISING)
    #GPIO.add_event_detect(but_blk, GPIO.RISING)
    GPIO.setup(led_5, GPIO.OUT)
    GPIO.output(led_5, GPIO.HIGH)
    i = 0
    while True: # wait for any input
        # event received when red button pressed
        if GPIO.event_detected(but_red):
            time.sleep(0.05) # to avoid switch bouncing delay half sec
            # this is here to make sure only one press is recived
            #GPIO.wait_for_edge(but_red,GPIO.FALLING) 
            print("Button Red Pressed!   " + str(i))
            i+=1
            GPIO.output(led_5, GPIO.LOW)
            red_action()


if __name__ == '__main__':
    control()
