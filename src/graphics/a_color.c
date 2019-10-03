/*
    Copyright 2010, 2016-2019 Alex Margarit <alex@alxm.org>
    This file is part of a2x, a C video game framework.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "a_color.v.h"
#include <a2x.v.h>

APixelState a__color;
static AList* g_stateStack;

APixel a_color__key;
APixel a_color__limit;

static void a_color__init(void)
{
    g_stateStack = a_list_new();
    a_color_reset();

    a_color__key = a_pixel_fromHex(A_CONFIG_COLOR_SPRITE_KEY);
    a_color__limit = a_pixel_fromHex(A_CONFIG_COLOR_SPRITE_BORDER);
}

static void a_color__uninit(void)
{
    a_list_freeEx(g_stateStack, a_mem_free);
}

const APack a_pack__color = {
    "Color",
    {
        [0] = a_color__init,
    },
    {
        [0] = a_color__uninit,
    },
};

void a_color_push(void)
{
    a_list_push(g_stateStack, a_mem_dup(&a__color, sizeof(APixelState)));
}

void a_color_pop(void)
{
    APixelState* state = a_list_pop(g_stateStack);

    #if A_CONFIG_BUILD_DEBUG
        if(state == NULL) {
            A__FATAL("a_color_pop: Stack is empty");
        }
    #endif

    a__color = *state;
    a_mem_free(state);

    a_color_blendSet(a__color.blend);
    a_color_baseSetRgba(
        a__color.rgb.r, a__color.rgb.g, a__color.rgb.b, a__color.alpha);
}

void a_color_reset(void)
{
    a_color_blendSet(A_COLOR_BLEND_PLAIN);
    a_color_baseSetRgba(0, 0, 0, A_COLOR_ALPHA_MAX);
    a_color_fillBlitSet(false);
    a_color_fillDrawSet(true);
}

#if A_CONFIG_LIB_RENDER_SOFTWARE
static void optimizeAlphaBlending(void)
{
    if(a__color.canonicalBlend == A_COLOR_BLEND_RGBA) {
        AColorBlend fastestBlend = A_COLOR_BLEND_RGBA;

        switch(a__color.alpha) {
            case A_COLOR_ALPHA_MAX / 4: {
                fastestBlend = A_COLOR_BLEND_RGB25;
            } break;

            case A_COLOR_ALPHA_MAX / 2: {
                fastestBlend = A_COLOR_BLEND_RGB50;
            } break;

            case A_COLOR_ALPHA_MAX * 3 / 4: {
                fastestBlend = A_COLOR_BLEND_RGB75;
            } break;

            case A_COLOR_ALPHA_MAX: {
                fastestBlend = A_COLOR_BLEND_PLAIN;
            } break;
        }

        if(a__color.blend != fastestBlend) {
            a__color.blend = fastestBlend;
        }
    }
}
#endif

void a_color_blendSet(AColorBlend Blend)
{
    a__color.blend = Blend;
    a__color.canonicalBlend = Blend;

    #if A_CONFIG_LIB_RENDER_SOFTWARE
        optimizeAlphaBlending();
    #else
        if(Blend == A_COLOR_BLEND_RGB25) {
            a_color_alphaSet(A_COLOR_ALPHA_MAX / 4);
        } else if(Blend == A_COLOR_BLEND_RGB50) {
            a_color_alphaSet(A_COLOR_ALPHA_MAX / 2);
        } else if(Blend == A_COLOR_BLEND_RGB75) {
            a_color_alphaSet(A_COLOR_ALPHA_MAX * 3 / 4);
        }

        a_platform_api__renderSetBlendMode();
    #endif
}

void a_color_alphaSet(int Alpha)
{
    a__color.alpha = a_math_clamp(Alpha, 0, A_COLOR_ALPHA_MAX);

    #if A_CONFIG_LIB_RENDER_SOFTWARE
        optimizeAlphaBlending();
    #else
        a_platform_api__renderSetDrawColor();
    #endif
}

static void setRgb(int Red, int Green, int Blue)
{
    a__color.rgb.r = (unsigned)Red & 0xff;
    a__color.rgb.g = (unsigned)Green & 0xff;
    a__color.rgb.b = (unsigned)Blue & 0xff;

    a__color.pixel = a_pixel_fromRgb(Red, Green, Blue);
}

void a_color_baseSetRgb(int Red, int Green, int Blue)
{
    setRgb(Red, Green, Blue);

    #if !A_CONFIG_LIB_RENDER_SOFTWARE
        a_platform_api__renderSetDrawColor();
    #endif
}

void a_color_baseSetRgba(int Red, int Green, int Blue, int Alpha)
{
    setRgb(Red, Green, Blue);
    a__color.alpha = a_math_clamp(Alpha, 0, A_COLOR_ALPHA_MAX);

    #if A_CONFIG_LIB_RENDER_SOFTWARE
        optimizeAlphaBlending();
    #else
        a_platform_api__renderSetDrawColor();
    #endif
}

void a_color_baseSetHex(uint32_t Hexcode)
{
    a__color.rgb.r = (Hexcode >> 16) & 0xff;
    a__color.rgb.g = (Hexcode >> 8)  & 0xff;
    a__color.rgb.b = (Hexcode)       & 0xff;
    a__color.pixel = a_pixel_fromHex(Hexcode);

    #if !A_CONFIG_LIB_RENDER_SOFTWARE
        a_platform_api__renderSetDrawColor();
    #endif
}

void a_color_baseSetPixel(APixel Pixel)
{
    a__color.rgb = a_pixel_toRgb(Pixel);
    a__color.pixel = Pixel;

    #if !A_CONFIG_LIB_RENDER_SOFTWARE
        a_platform_api__renderSetDrawColor();
    #endif
}

void a_color_fillBlitSet(bool Fill)
{
    a__color.fillBlit = Fill;
}

void a_color_fillDrawSet(bool Fill)
{
    a__color.fillDraw = Fill;
}
