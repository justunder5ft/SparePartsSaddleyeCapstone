""" This file takes a directory that is full of png images as input and converts them all to jpg images as that
    is the data type that we have decided to go with

    NOTE: this file might actually be unneccesary - it looks like keras will resize the image for you
    based off of what input size you give...
"""

import PIL
from PIL import Image
import sys
import os
import glob

def change_to_1280_720(pathname: str = ''):
    for file in glob.glob(pathname + '*.jpg'):
        try:
            img = Image.open(file)
            img = img.resize((1280, 720), PIL.Image.ANTIALIAS)
            img.save(file)
        except:
            print(file)  # if a file fails just show the path to it so we can fix/delete it


if __name__ == "__main__":
    # change_to_1280_720('D:/Training Data/partitioned_data/test/Gravel/')
    # change_to_1280_720('D:/Training Data/partitioned_data/train/Gravel/')
    # change_to_1280_720('D:/Training Data/partitioned_data/validate/Gravel/')
    change_to_1280_720('D:/Training Data/luke-sidewalk/')
