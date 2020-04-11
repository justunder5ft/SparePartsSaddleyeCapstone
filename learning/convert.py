""" This file takes a directory that is full of png images as input and converts them all to jpg images as that
    is the data type that we have decided to go with
"""

from PIL import Image
import sys
import os


def handle_cmd_line_args():
    if len(sys.argv) != 2:
        print("usage: python3 main.py <path>")
        exit(1)
    else:
        return str(sys.argv[1])


if __name__ == "__main__":
    i = 0
    for path in os.listdir(handle_cmd_line_args()):
        print(path)
        i = i + 1
        if i > 5:
            break
