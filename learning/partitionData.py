""" This script partitions data for a given directory into the appropriate sized sets
    The input is given via the command line as a string that is the path to the directory that has the data that needs
    to be partitioned.

    It will look at the total number of photos that we have and partition them based on a fixed percentage (perhaps
    unless one is given via the command line).
"""
import glob
import random as rand
import shutil
import time

CLASSES = {'Asphalt', 'Sidewalk', 'Off-road', 'Gravel'}


def extract(path: str, new_path: str, classification: str):
    """ This takes the folder given in path and splits into 3 different sets of data

        currently uses an 80, 10, 10 split for the train, test, validate respectively
    """
    if classification not in CLASSES:
        raise ValueError("Classification not in classes")

    files = glob.glob(path + '*')
    number_of_files = len(glob.glob(path + '*'))
    train_count = number_of_files * .8
    test_count = number_of_files * .1
    validate_count = number_of_files * .1

    # iterate through the files in a random order
    count = 0
    rand.shuffle(files)
    for file in files:
        if count < train_count:
            shutil.copy(file, new_path + 'train/' + classification + '/')
        elif train_count <= count < validate_count + train_count:
            shutil.copy(file, new_path + 'test/' + classification + '/')
        else:  # validate_count + train_count <= count:
            shutil.copy(file, new_path + 'validate/' + classification + '/')
        count = count + 1


if __name__ == "__main__":
    # Keep all of this as a sort of log of what you have partitioned
    #
    # extract('D:/Training Data/data/Off-road/', 'D:/Training Data/partitioned_data/', 'Off-road')
    # extract('D:/Training Data/data/Asphalt/', 'D:/Training Data/partitioned_data/', 'Asphalt')
    # extract('D:/Training Data/data/Sidewalk/', 'D:/Training Data/partitioned_data/', 'Sidewalk')
    # extract('D:/Training Data/data/Gravel/', 'D:/Training Data/partitioned_data/', 'Gravel')
    # extract('D:/Training Data/04132020/', 'D:/Training Data/partitioned_data/', 'Gravel')
    # extract('D:/Training Data/luke-sidewalk/', 'D:/Training Data/partitioned_data/', 'Sidewalk')
    # extract('D:/Training Data/cam-off-road/', 'D:/Training Data/partitioned_data/', 'Off-road')
    # extract('D:/Training Data/cameron/asphalt_cameron/', 'D:/Training Data/partitioned_data/', 'Asphalt')
    # extract('D:/Training Data/cameron/sidewalk_cameron/', 'D:/Training Data/partitioned_data/', 'Sidewalk')

    print(time.time()) 
