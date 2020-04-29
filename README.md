# SparePartsSaddleyeCapstone
CSCE 4901.001 - Capstone 2020  
## Developers  
* Anthony Carr
* Brice Brosig
* Cameron Donner
* Ephraim Jackson
* Alexandra Triampol

## Overview
This repo contains all the code/scripts necesarry to label data, 
pre-process, partition, train, and test for the machine learning
model that classifies trails into "trail types".  
There are three pieces of this project: the labeler, the learning, and the control scripts that run on the bean device.  
This readme outlines the different components, dependencies, and instrunctions for each of these pieces.

## File structure
* labeler -> This contains the contents necessary to edit the data labeler, this is a QT project.
* learning -> This contains the contents of the model, and its training scripts, along with some scripts used to sort, convert, and separate the data.
* ControlScript.py -> Used to gather data on the prototype, this operates the videoGather.py script.
* VideoGather.py -> Used to gather data on the prototype, this takes video at 10fps and stores it on the device.
* Predict.py -> This starts the model with tensorflow backend, allows for image prediction by using function “predictImage(img).”
* ImagePredict.py -> Used to predict images from the camera, takes a picture every 5 seconds and prints a prediction.

# Learning

## Results
The results of the 4-class classifier were decidedly not great... We suspect that there
was simply not enough data to support that sort of classification. To compensate, however, we 
developed a second model in binary.py that only classifies between sidewalk and asphalt as these
are the areas where the system is likely to be on and doing important work - looking for danger.
When on a trail or off-road, the system can loosen its "alertness" so-to-speak as there are likely
no cars and few pedestrians.

For that binary classifier, we sit around 60% (as of writing this). That is with a 4 layer convolutional 
neural net that has two fully connected layers at the end with two dropout layer. This neural net was based off
a neural net from A Machine Learning Approach to Visual Perception of Forest Trails for Mobile Robots
that had much better results. 

To improve on these results, obviously, we need what everybody wants. Data. 
The data gathering/storage needs to get MUCH more sophisticated than it currently. That is, 
data gathering, storing, labeling, and pre-processing is a whole beast on its own. The model that
trains off of that data is a separate one. 

## Usage
### retraining
You can run either main.py or binary.py to retrain either classifiers.
* binary - classifies between two different classification
* main - classifies between four

To retrain you will need to alter to the paths slightly as they are based off of
the directories on my system. in addition to new training data, you will have to change
the number of training instances that are in the folders.

### the scripts
Run any of the scripts with either the command line or some ide like pycharm. A lot of the scripts depend on paths
that are relative to your local machine/wherever you are running so be weary.

## Dependencies
* python3.8 was used for development and testing
    * python2 will absolutely not work
    * python3.7 should be fine though it may present different dependencies with the tensorflow
    version and cuda versions
* keras
* tensorflow
    * to use gpu you need to set up CUDA on your machine
* opencv2
    * this includes python modules as well as binaries
* numpy
* PIL (python image library)
    * This should automatically get resolved when you install keras/tensorflow
    but it is a dependency
* to learn more about keras installation - https://keras.io/
* to learn more about tensorflow installation - https://keras.io/
* to learn more about tensorflow gpu support - https://www.tensorflow.org/install/gpu
* to learn more about cudaNN and other cuda dependencies - https://docs.nvidia.com/deeplearning/sdk/cudnn-install/

## Scripts
### convert
This is a very small script that takes a directory as input and converts any image in that
directory to a file size specified in the script itself. This can be useful if a decision is made
that the a new standard image size is now being trained on.

### extraction
Another small, but powerful, script that converts an mp4 video into a directory full of images
that can actually be trained on. The option to skip certain numbers of frames is given via a definition
at the top of the file.

### partitionData
Part of the training process is the partitioning of data into three VERY distict sets: train, test, validation.
These need to not contain duplicates inside themselves or between otherwise the model is cheating, overfitting, and 
giving results that are not genuine. This script allows for the partitioning of a directory of data into the
three different data sets of sizes determined by definitions at the top of the file.

# Control Scripts
These are meant to be run on the bean device. They handle taking photos, running
a trained model and getting analytics off the device.

## Dependencies
* opencv2
* numpy
* keras
* tensorflow
* pillow (PIL)
* python3

# Labeler
## quick start
To use the labeler you will need to have to K-Lite codec installed on your machine, as the libraries we used within Qt are dependent on this codec to decode the videos for playback. The codec can be found here: https://codecguide.com/download_kl.htm

## step by step
* Click on the Load Media button in the bottom right of the program, and select the video you would like you process. (All formats supported by the K-Lite codec will function properly.)  
* Click the Select Data Folder button and select the file location where the frame folders will be saved. (This step is optional, if nothing is selected then folders will be created in the same location as the executable.)  
* Click on the Process media button to load the video file into the program.  
* Click the Play button to begin playback and the individual video frames will be saved to the specified folders based on the checkboxes that are selected.  
  
Note 1: When paused, the video will stop playing and no more frames will be pushed for processing. HOWEVER, all frames that are currently enqueued will continue to be processed until the queue is empty.  
Note 2: Closing the program early or loading in a new video to be processed will halt frame processing and empty the queue entirely.  

## Detailed Guide  
The labeling tool is designed to help quickly collect and sort training data to be used with the trail classification model. By taking a video of a trail ride with any camera (Go Pros for example) and loading this video into the program, you can retrieve each frame within the video and save them in folders specified within the program. The folders that will be written to can be specified by selecting the corresponding checkbox and playing the video. While the video is playing, each frame will be written to each folder that is checked AT THAT POINT in the video, meaning checkboxes can be changed dynamically if there are multiple trail types in a video.  
The frame skip amount in the bottom left represents the frequency at which you would like to save frames. By default the value is set to 1, which means that every single frame in the video will be saved. Changing this value to 2 for example will only save every other frame, starting at the first frame, and so on.
The process of loading in and processing video can be found in the quick start guide above. An important thing to remember is that each frame is pushed into a frame processing queue which is used per each video loaded in. So if frames are still being processed but you want to begin loading another video, you must wait until all frames are processed to continue, and the same is true when exiting the program.   
The backend of the program is fairly simple. For each frame being processed while the video is playing, the states of the checkboxes are checked and stored along with the data for the given frame, and all of this information is bundled together and stored within a queue. This queue is checked constantly by a frame processing thread, which will convert each frame into jpeg image format and save the frames based on the checkbox state at the time the frame was pushed to the queue. All frames will be saved as the name of the original video with the frame number appended at the end. The controls for playback of the video itself as well as access to individual frames of the video is provided by built in classes to Qt, so within the code we simply call the member functions for these classes.

## dependencies
* Qt Version 5.14.1
* Min Gw 64-bit compiler
