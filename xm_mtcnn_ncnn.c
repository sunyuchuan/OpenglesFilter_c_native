#include <android/log.h>
#include "xm_mtcnn_ncnn.h"
//#if defined(__ANDROID__)
#include "libyuv.h"
//#endif
#include "xmsdl_mtcnn.h"
#include "ijksdl/ijksdl_misc.h"

#define TAG "mtcnn_ncnn_thread"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

#define RGBA_MAX_W_H 480
#define FACE_INFO_LEN (1 + 14 * 100)

static void notify(Thread *thread)
{
    if(!thread)
        return;

    pthread_mutex_lock(&thread->mLock);
    pthread_cond_signal(&thread->mCondition);
    pthread_mutex_unlock(&thread->mLock);
}

static void waitOnNotify(Thread *thread)
{
    if(!thread)
        return;

    pthread_mutex_lock(&thread->mLock);
    pthread_cond_wait(&thread->mCondition, &thread->mLock);
    pthread_mutex_unlock(&thread->mLock);
}

static bool getRunningStatus(Thread *thread)
{
    bool status = false;
    if(!thread)
        return status;

    pthread_mutex_lock(&thread->mLock);
    status = thread->mRunning;
    pthread_mutex_unlock(&thread->mLock);
    return status;
}

static bool getDetectStatus(Thread *thread)
{
    bool status = true;
    if(!thread)
        return status;

    pthread_mutex_lock(&thread->mLock);
    status = thread->mDetectCompleted;
    pthread_mutex_unlock(&thread->mLock);
    return status;
}

static void set_running_status(Thread *thread, bool status)
{
    if(!thread)
        return;

    pthread_mutex_lock(&thread->mLock);
    thread->mRunning = status;
    pthread_mutex_unlock(&thread->mLock);
}

static void set_detect_status(Thread *thread, bool status)
{
    if(!thread)
        return;

    pthread_mutex_lock(&thread->mLock);
    thread->mDetectCompleted = status;
    pthread_mutex_unlock(&thread->mLock);
}

static void fillRectangleCoordinates(int *srcBuffer, int *dstBuffer, Thread *thread, int buffer_len)
{
    if(!srcBuffer || !dstBuffer || !thread)
        return;

    pthread_mutex_lock(&thread->mLock);
    for(int i = 0; i < buffer_len; i++)
    {
        dstBuffer[i] = srcBuffer[i];
    }
    pthread_mutex_unlock(&thread->mLock);
}

static void pixelCoordinates2PlaneCoordinates_rotate(int rectangleCoordinates[4], int angle, int origin[2], int h)
{
    float pi = 3.1415926;
    int point[4] = {0};
    float cos_r = cos(pi / 180.0 * angle);
    float sin_r = sin(pi / 180.0 * angle);
    int x2 = origin[0];
    int y2 = h -origin[1];

    for (int i = 0; i < 4; i += 2) {
        int x1 = rectangleCoordinates[i];
        int y1 = h -rectangleCoordinates[i+1];
        point[i] = (x1 - x2) * cos_r - (y1 - y2) * sin_r + x2;
        point[i+1] = (x1 - x2) * sin_r + (y1 - y2) * cos_r + y2;
        point[i+1] = h -point[i+1];
    }

    for(int i = 0; i < 2; i++) {
        if(point[i] < point[i+2]) {
            rectangleCoordinates[i] = point[i];
            rectangleCoordinates[i+2] = point[i+2];
        } else {
            rectangleCoordinates[i] = point[i+2];
            rectangleCoordinates[i+2] = point[i];
        }
    }
}

static void I420toRGBA(
        const unsigned char *y, const unsigned char *u, const unsigned char *v,
        int stride_y, int stride_u, int stride_v,
        int src_w, int src_h,
        unsigned char *dst, int dst_w, int dst_h)
{
    if(!y || !u || !v || !dst)
        return;

    unsigned char *temp = (unsigned char *)malloc(sizeof(char)*src_w*src_h*4);

    I420ToABGR(y, stride_y,
        u, stride_u,
        v, stride_v,
        temp, src_w*4,
        src_w, src_h);

    ARGBScale(temp, src_w*4,
               src_w, src_h,
               dst, dst_w*4,
               dst_w, dst_h,
               kFilterNone);
    free(temp);
}

static void fill_rgba_image(XMYUV420Image *image, unsigned char *dst, int dst_w, int dst_h)
{
    if(!image || !dst)
        return;

    I420toRGBA(image->pixels[0], image->pixels[1], image->pixels[2],
            image->pitches[0], image->pitches[1], image->pitches[2],
            image->w, image->h, dst, dst_w, dst_h);
}

static int detect_start(void *arg)
{
    if(!arg)
        return -1;

    XMMtcnnNcnn *mn = arg;
    Thread *thread = mn->thread;
    XMYUV420Image *image = NULL;
    int rgba_image_w = 0;
    int rgba_image_h = 0;
    int rgba_image_h_last = 0;
    int left_last = 0;
    int top_last = 0;
    int right_last = 0;
    int bottom_last = 0;
    unsigned char *rgba_image = NULL;

    set_running_status(thread, true);

    while(!thread->mAbortRequest)
    {
        waitOnNotify(thread);
        image = mn->image;
        set_detect_status(thread, false);

        if(image->w > image->h)
        {
            rgba_image_w = RGBA_MAX_W_H;
            rgba_image_h = IJKALIGN((int)(rgba_image_w * ((double)image->h / (double)image->w)), 4);
        } else {
            rgba_image_h = RGBA_MAX_W_H;
            rgba_image_w = IJKALIGN((int)(rgba_image_h * ((double)image->w / (double)image->h)), 4);
        }
        if(rgba_image == NULL ||  rgba_image_h_last != rgba_image_h)
        {
            free(rgba_image);
            rgba_image_h_last = rgba_image_h;
            LOGD("src_w %d src_h %d, dst_w %d, dst_h %d", image->w, image->h, rgba_image_w, rgba_image_h);
            rgba_image = (unsigned char *)malloc(4 * rgba_image_w * rgba_image_h);
        }

        fill_rgba_image(image, rgba_image, rgba_image_w, rgba_image_h);
        Mtcnn_SetMinFaceSize(80);
        Mtcnn_SetTimeCount(1);
        Mtcnn_SetThreadsNumber(4);
        int faceInfo[FACE_INFO_LEN] = {0};
        bool ret = Mtcnn_MaxFaceDetect(rgba_image, rgba_image_w, rgba_image_h, 4, faceInfo, FACE_INFO_LEN);
        if(ret && faceInfo[0] > 0)
        {
            float scale = 0.35;
            int left = (int) ((float)(faceInfo[1]) - (faceInfo[3] - faceInfo[1])*scale);
            int top = (int) ((float)faceInfo[2] - (faceInfo[4] - faceInfo[2])*scale);
            int right = (int) ((float)(faceInfo[3]) + (faceInfo[3] - faceInfo[1])*scale);
            int bottom = (int) ((float)faceInfo[4] + (faceInfo[4] - faceInfo[2])*scale);
            //pixelCoordinates2PlaneCoordinates_rotate(rectangleCoordinates, angle, origin, rgba_image_h);
            int rectangle[6] = {left, top, right, bottom, rgba_image_w, rgba_image_h};
            /*Rectangle Coordinates jumping when off-screen rendering*/
            if(left_last == 0 && top_last == 0 && right_last == 0 && bottom_last == 0)
            {
                left_last = left;
                top_last = top;
                right_last = right;
                bottom_last = bottom;
            }
            if(!(abs(left -left_last) > rgba_image_w/3
                ||abs(top -top_last) > rgba_image_h/3
                ||abs(right -right_last) > rgba_image_w/3
                ||abs(bottom -bottom_last) > rgba_image_h/3))
            /*Rectangle Coordinates jumping when off-screen rendering*/
            {
                left_last = left;
                top_last = top;
                right_last = right;
                bottom_last = bottom;
                fillRectangleCoordinates(rectangle, mn->rectangle_coordinates, thread, FACE_DETECT_OUTPUT_LEN);
            } else {
                LOGD("rectangle coordinates abnormal and discarded\n");
            }
        }
        set_detect_status(thread, true);
    }

end:
    free(rgba_image);
    set_running_status(thread, false);
    Mtcnn_FaceDetectionModelUnInit();
    return 0;
}

static void YUV420ImageFreep(XMYUV420Image **image)
{
    if(!image || !*image)
        return;

    for(int i = 0; i < 3; i++)
    {
        free((*image)->pixels[i]);
    }
    free(*image);
    *image = NULL;
}

static void threadFreep(Thread **thread)
{
    if(!thread || !*thread)
        return;

    pthread_mutex_destroy(&(*thread)->mLock);
    pthread_cond_destroy(&(*thread)->mCondition);
    free(*thread);
    *thread = NULL;
}

static bool fillYUV420Image(XMMtcnnNcnn *mn, SDL_VoutOverlay *overlay)
{
    if(!mn || !mn->thread || !mn->image || !overlay)
        return false;

    XMYUV420Image *image = mn->image;
    Thread *thread = mn->thread;
    int planes[3] = { 0, 1, 2 };
    const int widths[3]    = { overlay->pitches[0], overlay->pitches[1], overlay->pitches[2] };
    const int heights[3]   = { overlay->h,          overlay->h / 2,      overlay->h / 2 };
    const unsigned char *pixels[3]   = { overlay->pixels[0],  overlay->pixels[1],  overlay->pixels[2] };

    switch (overlay->format) {
	case SDL_FCC_I420:
		break;
	case SDL_FCC_YV12:
		planes[1] = 2;
		planes[2] = 1;
		break;
	default:
		LOGD("unexpected format %x\n", overlay->format);
		return false;
    }

    image->w = overlay->w;
    image->h = overlay->h;
    image->format = overlay->format;
    for(int i = 0; i < 3; i++)
    {
        image->pitches[i] = widths[planes[i]];
        pthread_mutex_lock(&thread->mLock);
        memcpy(image->pixels[i], pixels[planes[i]], widths[planes[i]] * heights[planes[i]]);
        pthread_mutex_unlock(&thread->mLock);
    }
    return true;
}

static XMYUV420Image *YUV420ImageCreate(int w, int h)
{
    XMYUV420Image *image = (XMYUV420Image *)malloc(sizeof(XMYUV420Image));
    if (!image) {
        LOGD("Struct XMYUV420Image malloc error!!!\n");
        return NULL;
    }
    memset(image, 0, sizeof(XMYUV420Image));

    image->pixels_len[0] = IJKALIGN((w * h), 4);
    image->pixels_len[1] = IJKALIGN(((w * h)/4), 4);
    image->pixels_len[2] = IJKALIGN(((w * h)/4), 4);
    for(int i = 0; i < 3; i++)
    {
        image->pixels[i] = (Uint8 *)malloc(image->pixels_len[i]);
    }

    return image;
}

static Thread *threadCreate()
{
    Thread *thread = (Thread *)malloc(sizeof(Thread));
    if (!thread) {
        LOGD("Struct Thread malloc error!!!\n");
        return NULL;
    }
    memset(thread, 0, sizeof(Thread));
    thread->mDetectCompleted = true;
    pthread_mutex_init(&thread->mLock, NULL);
    pthread_cond_init(&thread->mCondition, NULL);
    return thread;
}

static int startAsync(XMMtcnnNcnn *mn)
{
    if(NULL == mn) {
        LOGD("xm_mtcnn_ncnn_startAsync() failed, XMMtcnnNcnn object invalid");
        return -1;
    }

    Thread *thread = threadCreate();
    if(!thread) {
        LOGD("xm_mtcnn_ncnn_threadCreate() failed");
        return -1;
    }
    pthread_mutex_lock(&thread->mLock);
    mn->thread = thread;
    pthread_mutex_unlock(&thread->mLock);
    set_running_status(mn->thread, true);

    char *sdPath = "/sdcard/mtcnn/";
    Mtcnn_FaceDetectionModelInit(sdPath);

    thread->tid = SDL_CreateThreadEx(&thread->_tid, detect_start, mn, "mtcnn_ncnn thread");
    if (!thread->tid) {
        av_log(NULL, AV_LOG_FATAL, "SDL_CreateThreadEx() failed");
        xm_mtcnn_ncnn_free(&mn);
        return -1;
    }

    return 0;
}

bool xm_mtcnn_ncnn_faceDetect(XMMtcnnNcnn *mn, SDL_VoutOverlay *overlay, int *face_rectangle_coordinates)
{
    bool ret = false;
    if(!mn || !overlay || !face_rectangle_coordinates)
        return ret;

    bool tf_detect_enable = false;
    if(overlay->v_effects.timeType_show) {
        switch (overlay->v_effects.timeType_index) {
            case V_TIME_EFFECTS_TYPE_FACE_STICKER:
                tf_detect_enable = true;
                break;
            default:
                tf_detect_enable = false;
                break;
        }
    }

    bool bf_detect_enable = false;
    if(overlay->v_effects.baseType_show) {
        switch (overlay->v_effects.baseType_index) {
            case V_BASE_EFFECTS_TYPE_FACE_STICKER:
                bf_detect_enable = true;
                break;
            default:
                bf_detect_enable = false;
                break;
        }
    }

    if(!tf_detect_enable && !bf_detect_enable)
    {
	//xm_mtcnn_ncnn_abort(mn->thread);
	//xm_mtcnn_ncnn_wait(mn->thread);
	//xm_mtcnn_ncnn_free(mn);
	ret = false;
    } else {
	bool running = getRunningStatus(mn->thread);
	if(!running)
	    startAsync(mn);

	bool detectCompleted = getDetectStatus(mn->thread);
	if(running && detectCompleted)
	{
	    pthread_mutex_lock(&mn->thread->mLock);
	    for(int i = 0; i < FACE_DETECT_OUTPUT_LEN; i++) {
	        face_rectangle_coordinates[i] = mn->rectangle_coordinates[i];
	    }
	    pthread_mutex_unlock(&mn->thread->mLock);

	    XMYUV420Image *image = mn->image;
	    if(NULL == image || (overlay->w != image->w || overlay->h != image->h))
	    {
	        YUV420ImageFreep(&mn->image);
	        mn->image = YUV420ImageCreate(overlay->w, overlay->h);
	    }
	    if(fillYUV420Image(mn, overlay))
	    {
	        notify(mn->thread);
	    }
	}
	ret = true;
    }

    return ret;
}

void xm_mtcnn_ncnn_free(XMMtcnnNcnn *mn)
{
    if(!mn)
        return;

    threadFreep(&mn->thread);
    YUV420ImageFreep(&mn->image);
}

void xm_mtcnn_ncnn_freep(XMMtcnnNcnn **mn)
{
    if(!mn)
        return;

    xm_mtcnn_ncnn_free(*mn);
    free(*mn);
    *mn = NULL;
}

XMMtcnnNcnn *xm_mtcnn_ncnn_create()
{
    XMMtcnnNcnn *mn = (XMMtcnnNcnn *)malloc(sizeof(XMMtcnnNcnn));
    if (!mn) {
        LOGD("Struct XMMtcnnNcnn malloc error!!!\n");
        return NULL;
    }
    memset(mn, 0, sizeof(XMMtcnnNcnn));
    return mn;
}

void xm_mtcnn_ncnn_abort(Thread *thread)
{
    if(!thread)
        return;

    pthread_mutex_lock(&thread->mLock);
    thread->mAbortRequest = true;
    pthread_cond_signal(&thread->mCondition);
    pthread_mutex_unlock(&thread->mLock);
}

int xm_mtcnn_ncnn_wait(Thread *thread)
{
    if(!thread)
        return -1;

    if(!thread->mRunning || !thread->tid)
    {
        return -1;
    }
    SDL_WaitThread(thread->tid, NULL);
    thread->mRunning = false;
    return 0;
}

