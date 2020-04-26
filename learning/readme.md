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
a neural net from TODO: INSERT RESEARCH PAPER that had much better results. 

To improve on these results, obviously, we need what everybody wants. Data. 
The data gathering/storage needs to get MUCH more sophisticated than it currently. That is, 
data gathering, storing, labeling, and pre-processing is a whole beast on its own. The model that
trains off of that data is a separate one. 

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