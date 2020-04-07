# Program To Read video
# and Extract Frames 
import cv2


# Function to extract frames
def FrameCapture(path, run, folder, classification):
    # Path to video file 
    vidObj = cv2.VideoCapture(path)

    # Used as counter variable 
    count = 0

    # checks whether frames were extracted 
    success = 1

    while success:

        # vidObj object calls read 
        # function extract frames 
        success, image = vidObj.read()

        # Saves the frames with frame-count
        if count % 5 == 0:
            try:
                cv2.imwrite(
                    "C:/Users/bgb0074/Dev/tempVideos/%s/%s-brice-%d-%d.jpg" % (folder, classification, run, count),
                    image)
            except:
                print()

        count += 1


# Driver Code 
if __name__ == '__main__':
    # Calling the function 
    FrameCapture('C:/Users/bgb0074/Dev/tempVideos/GOPR0668.MP4', 8, '668', 'sidewalk')
    FrameCapture('C:/Users/bgb0074/Dev/tempVideos/GOPR0669.MP4', 9, '669', 'sidewalk')
    FrameCapture('C:/Users/bgb0074/Dev/tempVideos/GOPR0670.MP4', 10, '670', 'sidewalk')
    FrameCapture('C:/Users/bgb0074/Dev/tempVideos/GOPR0671.MP4', 11, '671', 'sidewalk')
    FrameCapture('C:/Users/bgb0074/Dev/tempVideos/GOPR0672.MP4', 12, '672', 'sidewalk')
    FrameCapture('C:/Users/bgb0074/Dev/tempVideos/GOPR0673.mp4', 13, '673', 'sidewalk')
    FrameCapture('C:/Users/bgb0074/Dev/tempVideos/GOPR0674.MP4', 14, '674', 'sidewalk')
