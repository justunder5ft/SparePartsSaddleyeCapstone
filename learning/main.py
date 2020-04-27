"""
This is a docstring

Got a lot of this from:
https://gist.github.com/fchollet/0830affa1f7f19fd47b06d4cf89ed44d
as well as from my collaboratory doc

=== CLASSES ===
Asphalt
Sidewalk
Gravel
Off-road

loss function - categorical crossentropy


With the current amount of data that we have to train on this classifier is simply not good.
To compensate for this, we switched to the binary classifier found in binary.py and saved as
binary-classifier-720.h5.
Fixing this requires some more sosphicated design of course, but most of all: DATA. Going off of the '# of features'
rule of thumb for training instances - a 1280*720 RGB image has 1280*720*3 == 2764800 training instances (roughly)
Obviously convolutional neural nets are better at feature extraction than it would be if the image was flattened outright.

Brice - 2020-04-26
"""
from PIL import ImageFile
from keras.models import Sequential  # this is the model
from keras.layers import Conv2D, MaxPool2D  # for the convolution layer and pooling layer
from keras.layers import Flatten, Dense, Dropout, Activation  # some neat layers that help us out 
from keras import backend as K  # channels
from keras.preprocessing.image import ImageDataGenerator  # for loading the images into the appropriate format
import numpy as np
import os  # setting the log level

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '1'
ImageFile.LOAD_TRUNCATED_IMAGES = True

# Image format
IMG_HEIGHT = 720
IMG_WIDTH = 1280
DATA_PATH = 'D:/Training Data/partitioned_data'

TRAINING_DATA_DIR, TRAINING_DATA_SIZE = 'D:/Training Data/partitioned_data/train', 73871
VALIDATION_DATA_DIR, VALIDATION_DATA_SIZE = 'D:/Training Data/partitioned_data/validate', 9406
TEST_DATA_DIR, TEST_DATA_SIZE = 'D:/Training Data/partitioned_data/test', 9405

EPOCHS = 5  # Change these as we need to
BATCH_SIZE = 10  # This doesn't really matter right now since we have like no photos...

if K.image_data_format() == 'channels_first':  # i think channels first is the default so should be fine
    input_shape = (3, IMG_WIDTH, IMG_HEIGHT)
else:
    input_shape = (IMG_WIDTH, IMG_HEIGHT, 3)

# ===BUILDING THE MODEL===
model = Sequential()
model.add(Conv2D(32, (3, 3), input_shape=input_shape))
model.add(Activation('relu'))
model.add(MaxPool2D(pool_size=(2, 2)))

model.add(Conv2D(32, (3, 3)))
model.add(Activation('relu'))
model.add(MaxPool2D(pool_size=(2, 2)))

model.add(Conv2D(64, (3, 3)))
model.add(Activation('relu'))
model.add(MaxPool2D(pool_size=(2, 2)))

model.add(Flatten())
model.add(Dense(64))
model.add(Activation('relu'))
model.add(Dropout(0.5))
model.add(Dense(4))
model.add(Activation('softmax'))

model.compile(loss='categorical_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

# +++GETTING THE DATA+++
# this is the augmentation configuration we will use for training
train_datagen = ImageDataGenerator(
    rescale=1. / 255,
    shear_range=0.2,
    zoom_range=0.2,
    horizontal_flip=True)

# this is the augmentation configuration we will use for testing:
# only rescaling
test_datagen = ImageDataGenerator(rescale=1. / 255)  # get the RGB values in between 0 and 1

train_generator = train_datagen.flow_from_directory(  # generate data for the training set
    TRAINING_DATA_DIR,
    target_size=(IMG_WIDTH, IMG_HEIGHT),
    batch_size=BATCH_SIZE,
    class_mode='categorical'
)

validation_generator = test_datagen.flow_from_directory(  # generate data for the validation set
    VALIDATION_DATA_DIR,
    target_size=(IMG_WIDTH, IMG_HEIGHT),
    batch_size=BATCH_SIZE,
    class_mode='categorical'
)

test_generator = test_datagen.flow_from_directory(  # generate data for the test set
    TEST_DATA_DIR,
    target_size=(IMG_WIDTH, IMG_HEIGHT),
    batch_size=BATCH_SIZE,
    class_mode='categorical'
)

model.fit_generator(  # training the model
    train_generator,
    steps_per_epoch=TRAINING_DATA_SIZE // BATCH_SIZE,
    epochs=EPOCHS,
    validation_data=validation_generator,
    validation_steps=VALIDATION_DATA_SIZE // BATCH_SIZE,
    shuffle=False
)

model.evaluate_generator(  # testing the model against our test data
    test_generator,
    steps=TEST_DATA_SIZE // BATCH_SIZE,
    workers=1
)

model.save_weights('first_try.h5')  # don't really need to save models rn
