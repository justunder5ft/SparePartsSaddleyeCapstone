#ifndef COMMON_H
#define COMMON_H
#include "framethreader.h"

extern FrameThreader* global_processing_thread; //Thread that processed frames
extern int global_total_enqueued_frames; //Total number of frames pushed into the queue (This will bbe EVERY frame in the video)
extern int global_total_processed_frames; //All frames that were actually saved to the local directories (Based on skip amount)

#endif // COMMON_H
