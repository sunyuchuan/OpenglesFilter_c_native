/*
* Created by sunyc on 2018-8-14.
* Abstraction and packaging about gles base effects.
*/
#include "base_filter.h"
#include "base_filter/base_renderer.h"

int XM_BFilter_getBitmapNumber(int index)
{
    int ret = 0;
    switch(index)
    {
        case V_BASE_EFFECTS_TYPE_RETRO:
            ret = 0;
            break;
        case V_BASE_EFFECTS_TYPE_FAIRYTALE:
            ret = 1;
            break;
        case V_BASE_EFFECTS_TYPE_SUNRISE:
            ret = 3;
            break;
        case V_BASE_EFFECTS_TYPE_SUNSET:
            ret = 2;
            break;
        case V_BASE_EFFECTS_TYPE_WHITECAT:
            ret = 0;
            break;
        case V_BASE_EFFECTS_TYPE_BLACKCAT:
            ret = 0;
            break;
        case V_BASE_EFFECTS_TYPE_SKINWHITEN:
            ret = 0;
            break;
        case V_BASE_EFFECTS_TYPE_SWEETS:
            ret = 1;
            break;
        case V_BASE_EFFECTS_TYPE_ROMANCE:
            break;
        case V_BASE_EFFECTS_TYPE_SAKURA:
            break;
        case V_BASE_EFFECTS_TYPE_WARM:
            ret = 2;
            break;
        case V_BASE_EFFECTS_TYPE_ANTIQUE:
            break;
        case V_BASE_EFFECTS_TYPE_NOSTALGIA:
            break;
        case V_BASE_EFFECTS_TYPE_CALM:
            ret = 2;
            break;
        case V_BASE_EFFECTS_TYPE_LATTE:
            break;
        case V_BASE_EFFECTS_TYPE_TENDER:
            ret = 1;
            break;
        case V_BASE_EFFECTS_TYPE_COOL:
            break;
        case V_BASE_EFFECTS_TYPE_EMERALD:
            break;
        case V_BASE_EFFECTS_TYPE_EVERGREEN:
            break;
        case V_BASE_EFFECTS_TYPE_CRAYON:
            break;
        case V_BASE_EFFECTS_TYPE_SKETCH:
            break;
        case V_BASE_EFFECTS_TYPE_AMARO:
            ret = 3;
            break;
        case V_BASE_EFFECTS_TYPE_BRANNAN:
            ret = 5;
            break;
        case V_BASE_EFFECTS_TYPE_BROOKLYN:
            ret = 3;
            break;
        case V_BASE_EFFECTS_TYPE_EARLYBIRD:
            ret = 5;
            break;
        case V_BASE_EFFECTS_TYPE_FREUD:
            ret = 1;
            break;
        case V_BASE_EFFECTS_TYPE_HEFE:
            ret = 4;
            break;
        case V_BASE_EFFECTS_TYPE_HUDSON:
            ret = 3;
            break;
        case V_BASE_EFFECTS_TYPE_INKWELL:
            ret = 1;
            break;
        case V_BASE_EFFECTS_TYPE_KEVIN:
        case V_BASE_EFFECTS_TYPE_LOMO:
        case V_BASE_EFFECTS_TYPE_N1977:
        case V_BASE_EFFECTS_TYPE_NASHVILLE:
        case V_BASE_EFFECTS_TYPE_PIXAR:
        case V_BASE_EFFECTS_TYPE_RISE:
        case V_BASE_EFFECTS_TYPE_SIERRA:
        case V_BASE_EFFECTS_TYPE_SUTRO:
        case V_BASE_EFFECTS_TYPE_TOASTER2:
        case V_BASE_EFFECTS_TYPE_VALENCIA:
        case V_BASE_EFFECTS_TYPE_WALDEN:
        case V_BASE_EFFECTS_TYPE_XPROII:
        case V_BASE_EFFECTS_TYPE_HEALTHY:
            break;
        case V_BASE_EFFECTS_TYPE_SWIRL:
            ret= 0;
            break;
        case V_BASE_EFFECTS_TYPE_EDGE_DETECTION:
            ret= 0;
            break;
        case V_BASE_EFFECTS_TYPE_GLASS_SPHERE:
            ret= 0;
            break;
        case V_BASE_EFFECTS_TYPE_CROSS_HATCH:
        case V_BASE_EFFECTS_TYPE_LAPLACIAN:
        case V_BASE_EFFECTS_TYPE_VIVGNETTE:
            break;
        case V_BASE_EFFECTS_TYPE_FACE_STICKER:
            ret= 1;
            break;
        default:
            ret = 0;
            break;
    }
    return ret;
}

