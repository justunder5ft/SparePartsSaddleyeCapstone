# Program To Read video
# and Extract Frames 
import cv2


def FrameCapture(path, run, folder, classification):
    vidObj = cv2.VideoCapture(path)  # This is the video
    count = 0  # which frame we are on
    success = 1  # if it worked
    while success:
        success, image = vidObj.read()  # read frame from video file
        if count % 5 == 0:  # every 5 frames is written
            try:  # writing
                cv2.imwrite('D:/Training Data/luke-sidewalk-demo/luke-%s-%s-%s.jpg' % (classification, run, count), image)
            except:  # if it fails, keep going as the other frames might work
                print()

        count += 1


# Driver Code 
if __name__ == '__main__':
    # Calling the function
    # FrameCapture('D:/Training Data/sidewalk_luke_1.mp4', '15', 'none', 'sidewalk')
    # FrameCapture('D:/Training Data/sidewalk_luke_2.mp4', '16', 'none', 'sidewalk')
    # FrameCapture('D:/Training Data/sidewalk_luke_3.mp4', '17', 'none', 'sidewalk')
    # FrameCapture('D:/Training Data/sidewalk_luke_4.mp4', '18', 'none', 'sidewalk')
    # FrameCapture('D:/Training Data/sidewalk_luke_5.mp4', '19', 'none', 'sidewalk')
    # FrameCapture('D:/Training Data/sidewalk_luke_6.mp4', '20', 'none', 'sidewalk')
    # FrameCapture('D:/Training Data/sidewalk_luke_7.mp4', '21', 'none', 'sidewalk')
    FrameCapture('D:/Training Data/sidewalk_luke_8.mp4', '22', 'none', 'sidewalk')
