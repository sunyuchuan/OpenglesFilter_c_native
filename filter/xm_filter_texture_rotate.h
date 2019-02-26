/*
* Created by sunyc on 2018-8-22.
* Rotate Texture coordinates.
*/
#ifndef XM_FILTER_TEXTURE_ROTATE_H
#define  XM_FILTER_TEXTURE_ROTATE_H
#include <stdbool.h>

enum XM_Rotation {
    NORMAL = 0,
    ROTATION_90,
    ROTATION_180,
    ROTATION_270,
};

void XM_Filter_TexCoords_Rotation(float texcoords[8], enum XM_Rotation rotate, bool flipHorizontal, bool flipVertical);

#endif
