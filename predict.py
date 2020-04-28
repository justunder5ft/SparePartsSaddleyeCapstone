""" This file will be used to test the model in action mostly for the purpose
of seeing how much power it will consume while on the trail.

The script captures a photo every 5 seconds, runs that photo through the trained model, and returns a
trail type that it suspects it is on. while doing it will measure the power consumption on the battery.

We should get the power consumption on a 'per picture' basis as well as how much it takes over 15 minutes, 30 minutes
and so on. We should also do this in many different environments if possible: hot, cold, day, night (though i suspect
that will not have a HUGE effect).

MIT License
Copyright (c) 2019 JetsonHacks
See license
Using a CSI camera (such as the Raspberry Pi Version 2) connected to a
NVIDIA Jetson Nano Developer Kit using OpenCV
Drivers for the camera and OpenCV are included in the base image
"""
import tensorflow as tf
from PIL import ImageFile
from keras.models import Sequential  # this is the model
from keras.layers import Conv2D, MaxPool2D  # for the convolution layer and pooling layer
from keras.layers import Flatten, Dense, Dropout, Activation  # some neat layers that help us out
from keras import backend as K  # channels
import cv2
import numpy as np


IMG_HEIGHT = 720
IMG_WIDTH = 1280

ImageFile.LOAD_TRUNCATED_IMAGES = True
if K.image_data_format() == 'channels_first':  # i think channels first is the default so should be fine
    input_shape = (3, IMG_WIDTH, IMG_HEIGHT)
else:
    input_shape = (IMG_WIDTH, IMG_HEIGHT, 3)

model = Sequential()
model.add(Conv2D(10, (3, 3), input_shape=input_shape))
model.add(Activation('relu'))
model.add(MaxPool2D(pool_size=(2, 2)))

model.add(Conv2D(20, (3, 3)))
model.add(Activation('relu'))
model.add(MaxPool2D(pool_size=(2, 2)))

model.add(Conv2D(30, (3, 3)))
model.add(Activation('relu'))
model.add(MaxPool2D(pool_size=(2, 2)))

model.add(Conv2D(40, (3, 3)))
model.add(Activation('relu'))
model.add(MaxPool2D(pool_size=(2, 2)))

model.add(Flatten())
model.add(Dense(120))
model.add(Activation('relu'))
model.add(Dropout(0.5))
model.add(Dense(60))
model.add(Activation('relu'))
model.add(Dropout(0.5))
model.add(Dense(1))
model.add(Activation('softmax'))

model.load_weights('learning/binary-classifier-720.h5')

model.compile(loss='binary_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

# img = cv2.imread('test-asphalt-brice-4-0.jpg')
img = cv2.imread('test-sidewalk-brice-3-1915.jpg')
img = cv2.resize(img, (IMG_WIDTH, IMG_HEIGHT))
img = np.reshape(img, [1, IMG_WIDTH, IMG_HEIGHT, 3])

with tf.device('/cpu:0'):
    print(model.predict(img))

print("( - done - )")


