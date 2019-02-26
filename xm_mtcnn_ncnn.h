#ifndef _XM_MTCNN_NCNN_H_
#define _XM_MTCNN_NCNN_H_
#include "ijksdl/ijksdl_thread.h"
#include "ijksdl/ijksdl_vout.h"
#include <pthread.h>

#define FACE_DETECT_OUTPUT_LEN 6

typedef struct Thread {
    SDL_Thread *tid;
    SDL_Thread _tid;
    pthread_mutex_t mLock;
    pthread_cond_t mCondition;
    bool mRunning;
    volatile bool mAbortRequest;
    volatile bool mDetectCompleted;
} Thread;

typedef struct XMYUV420Image {
    int w;
    int h;
    Uint32 format;
    Uint16 pitches[3];
    int pixels_len[3];
    Uint8 *pixels[3];
} XMYUV420Image;

typedef struct XMMtcnnNcnn {
    Thread *thread;
    XMYUV420Image *image;
    int rectangle_coordinates[FACE_DETECT_OUTPUT_LEN];
} XMMtcnnNcnn;

void xm_mtcnn_ncnn_free(XMMtcnnNcnn *mn);
void xm_mtcnn_ncnn_freep(XMMtcnnNcnn **mn);
XMMtcnnNcnn *xm_mtcnn_ncnn_create();
bool xm_mtcnn_ncnn_faceDetect(XMMtcnnNcnn *mn, SDL_VoutOverlay *overlay, int *face_rectangle_coordinates);
void xm_mtcnn_ncnn_abort(Thread *thread);
int xm_mtcnn_ncnn_wait(Thread *thread);

#endif
