/*
* Created by sunyc on 2018-8-21.
* Abstraction and packaging about filters factory.
*/
#include "xm_filter_factory.h"

XM_Filter *XM_Filter_Factory_createBaseFilters(SDL_VoutOverlay *overlay)
{
    XM_Filter *filter = NULL;

    if(overlay->v_effects.baseType_show) {
        switch (overlay->v_effects.baseType_index) {
            case V_BASE_EFFECTS_TYPE_RETRO:
                filter = XM_BFilter_Retro_create();
                break;
            case V_BASE_EFFECTS_TYPE_FAIRYTALE:
                filter = XM_BFilter_Fairytale_create();
                break;
            case V_BASE_EFFECTS_TYPE_BLACKCAT:
                filter = XM_BFilter_Blackcat_create();
                break;
            case V_BASE_EFFECTS_TYPE_SKINWHITEN:
                filter = XM_BFilter_SkinWhiten_create();
                break;
            case V_BASE_EFFECTS_TYPE_SUNRISE:
                filter = XM_BFilter_SunRise_create();
                break;
            case V_BASE_EFFECTS_TYPE_SUNSET:
                filter = XM_BFilter_SunSet_create();
                break;
            case V_BASE_EFFECTS_TYPE_SWEETS:
                filter = XM_BFilter_Sweets_create();
                break;
            case V_BASE_EFFECTS_TYPE_WHITECAT:
                filter = XM_BFilter_WhiteCat_create();
                break;
            case V_BASE_EFFECTS_TYPE_ANTIQUE:
                filter = XM_BFilter_Antique_create();
                break;
            case V_BASE_EFFECTS_TYPE_ROMANCE:
                filter = XM_BFilter_Romance_create();
                break;
            case V_BASE_EFFECTS_TYPE_SAKURA:
                filter = XM_BFilter_Sakura_create();
                break;
            case V_BASE_EFFECTS_TYPE_WARM:
                filter = XM_BFilter_Warm_create();
                break;
            case V_BASE_EFFECTS_TYPE_NOSTALGIA:
                filter = XM_BFilter_Nostalgia_create();
                break;
            case V_BASE_EFFECTS_TYPE_CALM:
                filter = XM_BFilter_Calm_create();
                break;
            case V_BASE_EFFECTS_TYPE_LATTE:
                filter = XM_BFilter_Latte_create();
                break;
            case V_BASE_EFFECTS_TYPE_TENDER:
                filter = XM_BFilter_Tender_create();
                break;
            case V_BASE_EFFECTS_TYPE_COOL:
                filter = XM_BFilter_Cool_create();
                break;
            case V_BASE_EFFECTS_TYPE_EMERALD:
                filter = XM_BFilter_Emerald_create();
                break;
            case V_BASE_EFFECTS_TYPE_EVERGREEN:
                filter = XM_BFilter_Evergreen_create();
                break;
            case V_BASE_EFFECTS_TYPE_CRAYON:
                filter = XM_BFilter_Crayon_create();
                break;
            case V_BASE_EFFECTS_TYPE_SKETCH:
                filter = XM_BFilter_Sketch_create();
                break;
            case V_BASE_EFFECTS_TYPE_AMARO:
                filter = XM_BFilter_Amaro_create();
                break;
            case V_BASE_EFFECTS_TYPE_BRANNAN:
                filter = XM_BFilter_Brannan_create();
                break;
            case V_BASE_EFFECTS_TYPE_BROOKLYN:
                filter = XM_BFilter_Brooklyn_create();
                break;
            case V_BASE_EFFECTS_TYPE_EARLYBIRD:
                filter = XM_BFilter_EarlyBird_create();
                break;
            case V_BASE_EFFECTS_TYPE_FREUD:
                filter = XM_BFilter_Freud_create();
                break;
            case V_BASE_EFFECTS_TYPE_HEFE:
                filter = XM_BFilter_Hefe_create();
                break;
            case V_BASE_EFFECTS_TYPE_HUDSON:
                filter = XM_BFilter_Hudson_create();
                break;
            case V_BASE_EFFECTS_TYPE_INKWELL:
                filter = XM_BFilter_Inkwell_create();
                break;
            case V_BASE_EFFECTS_TYPE_SWIRL:
                filter = XM_BFilter_Swirl_create();
                break;
            case V_BASE_EFFECTS_TYPE_EDGE_DETECTION:
                filter = XM_BFilter_EdgeDetection_create();
                break;
            case V_BASE_EFFECTS_TYPE_GLASS_SPHERE:
                filter = XM_BFilter_GlassSphere_create();
                break;
            case V_BASE_EFFECTS_TYPE_CROSS_HATCH:
                filter = XM_BFilter_CrossHatch_create();
                break;
            case V_BASE_EFFECTS_TYPE_LAPLACIAN:
                filter = XM_BFilter_Laplacian_create();
                break;
            case V_BASE_EFFECTS_TYPE_VIVGNETTE:
                filter = XM_BFilter_Vivgnette_create();
                break;
            case V_BASE_EFFECTS_TYPE_FACE_STICKER:
                filter = XM_BFilter_FaceSticker_create();
                break;
            default:
                ALOGE("unexpected v_effects baseType_index %d\n", overlay->v_effects.baseType_index);
                break;
        }
    }

    if(filter)
    {
        filter->v_effects.baseType_show = overlay->v_effects.baseType_show;
        filter->v_effects.baseType_index = overlay->v_effects.baseType_index;
    }
    return filter;
}

XM_Filter *XM_Filter_Factory_createTimeFilters(SDL_VoutOverlay *overlay)
{
    XM_Filter *filter = NULL;

    if(overlay->v_effects.timeType_show) {
        switch (overlay->v_effects.timeType_index) {
            case V_TIME_EFFECTS_TYPE_GHOST:
                filter = XM_TFilter_Ghost_create();
                break;
            case V_TIME_EFFECTS_TYPE_RETRO:
                filter = XM_TFilter_Retro_create();
                break;
            case V_TIME_EFFECTS_TYPE_MAGNIFYING_GLASS:
                filter = XM_TFilter_Zoom_create();
                break;
            case V_TIME_EFFECTS_TYPE_ROTATE:
                filter = XM_TFilter_Rotate_create();
                break;
            case V_TIME_EFFECTS_TYPE_FISSION:
                filter = XM_TFilter_Fission_create();
                break;
            case V_TIME_EFFECTS_TYPE_SPIRE:
                filter = XM_TFilter_Spire_create();
                break;
            case V_TIME_EFFECTS_TYPE_SPHERE:
                filter = XM_TFilter_Sphere_create();
                break;
            case V_TIME_EFFECTS_TYPE_INVERT:
                filter = XM_TFilter_Invert_create();
                break;
            case V_TIME_EFFECTS_TYPE_HUE:
                filter = XM_TFilter_Hue_create();
                break;
            case V_TIME_EFFECTS_TYPE_FACE_STICKER:
                filter = XM_TFilter_FaceSticker_create();
                break;
            default:
                ALOGE("unexpected v_effects timeType_index %d\n", overlay->v_effects.timeType_index);
                break;
        }
    }

    if(filter)
    {
        filter->v_effects.timeType_show = overlay->v_effects.timeType_show;
        filter->v_effects.timeType_index = overlay->v_effects.timeType_index;
    }
    return filter;
}

