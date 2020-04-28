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