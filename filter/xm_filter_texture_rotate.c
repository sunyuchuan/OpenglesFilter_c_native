/*
* Created by sunyc on 2018-8-22.
* Rotate Texture coordinates.
*/
#include "xm_filter_texture_rotate.h"

static void texture_rotated_90(float texcoords[8])
{
    float temp[2] = {0.0f, 0.0f};
    if(!texcoords)
        return;

    temp[0] = texcoords[6];
    temp[1] = texcoords[7];
    for(int i = 5; i >= 0; i--)
    {
        texcoords[i + 2] = texcoords[i];
    }
    texcoords[0] = temp[0];
    texcoords[1] = temp[1];
}

static void texture_rotated_180(float texcoords[8])
{
    float temp = 0.0f;
    if(!texcoords)
        return;

    for(int i = 0; i < 4; i++)
    {
        temp = texcoords[i];
        texcoords[i] = texcoords[i + 4];
        texcoords[i + 4] = temp;
    }
}

static void texture_rotated_270(float texcoords[8])
{
    float temp[2] = {0.0f, 0.0f};
    if(!texcoords)
        return;

    temp[0] = texcoords[0];
    temp[1] = texcoords[1];
    for(int i = 2; i < 8; i++)
    {
        texcoords[i - 2] = texcoords[i];
    }
    texcoords[6] = temp[0];
    texcoords[7] = temp[1];
}

static float flip(float i)
{
    if (i > -0.000001f && i < 0.000001f) {
        return 1.0f;
    }

    return 0.0f;
}

void XM_Filter_TexCoords_Rotation(float texcoords[8], enum XM_Rotation rotate,
                                                      bool flipHorizontal, bool flipVertical)
{
    if(!texcoords)
        return;

    switch (rotate) {
        case ROTATION_90:
            texture_rotated_90(texcoords);
            break;
        case ROTATION_180:
            texture_rotated_180(texcoords);
            break;
        case ROTATION_270:
            texture_rotated_270(texcoords);
            break;
        case NORMAL:
        default:
            break;
    }

    if(flipHorizontal) {
        for(int i = 0; i < 8; i += 2) {
            texcoords[i] = flip(texcoords[i]);
        }
    }

    if(flipVertical) {
        for(int i = 1; i < 8; i += 2) {
            texcoords[i] = flip(texcoords[i]);
        }
    }
}

