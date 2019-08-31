#pragma once
#ifndef TEXTURE_BASE_H
#define TEXTURE_BASE_H

#include <Graphic/GraphicCommon.h>

namespace env
{
    struct TextureRect
    {
        TextureRect()
            : left(0.0f),
            top(0.0f),
            right(1.0f),
            bottom(1.0f) {}

        TextureRect(float left, float top, float right, float bottom)
            : left(left), top(top), right(right), bottom(bottom) {}

        float left, top, right, bottom;
    };
} // env
#endif TEXTURE_BASE_H