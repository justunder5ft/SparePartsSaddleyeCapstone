""" This script partitions data for a given directory into the appropriate sized sets
    The input is given via the command line as a string that is the path to the directory that has the data that needs
    to be partitioned.

    It will look at the total number of photos that we have and partition them based on a fixed percentage (perhaps
    unless one is given via the command line).
"""
import glob
import random as rand
import shutil

# incomplete - implement de_extract
# incomplete - add command line parsing/options
#   select the partition sizes
#   give source
#   give destination
#   option to "de-extract"

def extract(path: str, new_path: str):
    """ This takes the folder given in path and splits into 3 different sets of data """
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
            shutil.copy(file, new_path + '/Train/')
        elif train_count <= count < validate_count + train_count:
            shutil.copy(file, new_path + '/Test/')
        else:  # validate_count + train_count <= count:
            shutil.copy(file, new_path + '/Validation/')
        count = count + 1


def de_extract(path: str):
    """ Takes a path to the classification that we want to 'de-extract' back into one directory """
    pass


if __name__ == "__main__":
    extract('./data/Off-road/', './partitioned_data/Off-road/')
