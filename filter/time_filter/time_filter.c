/*
* Created by sunyc on 2018-8-14.
* Abstraction and packaging about gles effects.
*/

#include "time_filter.h"
#include "gles2_xm/internal.h"

GLboolean XM_GLES2_TimeFilter_isValid(XM_GLES2_Renderer *renderer, SDL_VoutOverlay *overlay)
{
    return (renderer->v_effects.timeType_show == overlay->v_effects.timeType_show)
                && (renderer->v_effects.timeType_index == overlay->v_effects.timeType_index);
}

int XM_TFilter_yuv420p_getPlanesTexturesNum(SDL_VideoEffects *veffects)
{
    int textures_num = 3;
    if(veffects->timeType_show) {
        switch (veffects->timeType_index) {
            case V_TIME_EFFECTS_TYPE_GHOST:
                textures_num = 6;
                break;
            case V_TIME_EFFECTS_TYPE_RETRO:
            case V_TIME_EFFECTS_TYPE_MAGNIFYING_GLASS:
            case V_TIME_EFFECTS_TYPE_ROTATE:
            case V_TIME_EFFECTS_TYPE_SPIRE:
            case V_TIME_EFFECTS_TYPE_SPHERE:
                textures_num = 3;
                break;
            case V_TIME_EFFECTS_TYPE_FACE_STICKER:
                textures_num = 4;
                break;
            default:
                textures_num = 3;
        }
    }

    return textures_num;
}

int XM_TFilter_getBitmapNumber(int index)
{
    int ret = 0;
    switch(index)
    {
        case V_TIME_EFFECTS_TYPE_GHOST:
        case V_TIME_EFFECTS_TYPE_RETRO:
        case V_TIME_EFFECTS_TYPE_MAGNIFYING_GLASS:
        case V_TIME_EFFECTS_TYPE_ROTATE:
        case V_TIME_EFFECTS_TYPE_FISSION:
        case V_TIME_EFFECTS_TYPE_SPIRE:
        case V_TIME_EFFECTS_TYPE_SPHERE:
        case V_TIME_EFFECTS_TYPE_CONTRAST:
        case V_TIME_EFFECTS_TYPE_BRIGHTNESS:
        case V_TIME_EFFECTS_TYPE_EXPOSURE:
        case V_TIME_EFFECTS_TYPE_HUE:
        case V_TIME_EFFECTS_TYPE_SATURATION:
        case V_TIME_EFFECTS_TYPE_SHARPEN:
        case V_TIME_EFFECTS_TYPE_IMAGE_ADJUST:
        case V_TIME_EFFECTS_TYPE_INVERT:
                break;
        case V_TIME_EFFECTS_TYPE_FACE_STICKER:
                ret = 1;
                break;
        default:
            ret = 0;
            break;
    }
    return ret;
}

