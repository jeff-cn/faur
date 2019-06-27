/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of a2x, a C video game framework.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "graphics/a_pixels.p.h"

typedef struct APixels APixels;

#include "general/a_main.v.h"
#include "graphics/a_color.v.h"
#include "math/a_fix.v.h"
#include "platform/a_platform.v.h"

typedef enum {
    A_PIXELS__ALLOC = A_FLAG_BIT(0),
    A_PIXELS__SCREEN = A_FLAG_BIT(1),
} APixelsFlags;

struct APixels {
    APixel* buffer;
    APlatformTexture* texture;
    int w, h;
    unsigned bufferSize;
    APixelsFlags flags;
};

extern APixels* a_pixels__new(int W, int H, APixelsFlags Flags);
extern APixels* a_pixels__sub(APixels* Source, int X, int Y, int Width, int Height);
extern APixels* a_pixels__dup(const APixels* Pixels);
extern void a_pixels__free(APixels* Pixels);

extern void a_pixels__bufferSet(APixels* Pixels, APixel* Buffer, int W, int H);
extern void a_pixels__commit(APixels* Pixels);

extern AVectorInt a_pixels__boundsFind(const APixels* Pixels, int X, int Y);

static inline APixel* a_pixels__bufferGetFrom(const APixels* Pixels, int X, int Y)
{
    return Pixels->buffer + Y * Pixels->w + X;
}

static inline APixel a_pixels__bufferGetAt(const APixels* Pixels, int X, int Y)
{
    return *(Pixels->buffer + Y * Pixels->w + X);
}

static inline void a_pixels__clear(const APixels* Pixels)
{
    memset(Pixels->buffer, 0, Pixels->bufferSize);
}

static inline void a_pixels__fill(const APixels* Pixels, APixel Value)
{
    APixel* buffer = Pixels->buffer;

    for(int i = Pixels->w * Pixels->h; i--; ) {
        *buffer++ = Value;
    }
}

static inline void a_pixels__copy(const APixels* Dst, const APixels* Src)
{
    #if A_CONFIG_BUILD_DEBUG
        if(Src->w != Dst->w || Src->h != Dst->h
            || Src->bufferSize > Dst->bufferSize) {

            A__FATAL("a_pixels__copy(%dx%d, %dx%d): Different sizes",
                     Dst->w,
                     Dst->h,
                     Src->w,
                     Src->h);
        }
    #endif

    memcpy(Dst->buffer, Src->buffer, Src->bufferSize);
}

static inline void a_pixels__copyToBuffer(const APixels* Pixels, APixel* Buffer)
{
    memcpy(Buffer, Pixels->buffer, Pixels->bufferSize);
}
