/*
    Copyright 2010, 2016, 2017 Alex Margarit

    This file is part of a2x-framework.

    a2x-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    a2x-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with a2x-framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "a2x_pack_sdl_video.v.h"

#include <SDL.h>

#if A_CONFIG_LIB_SDL == 1
    static SDL_Surface* g_sdlScreen = NULL;
#elif A_CONFIG_LIB_SDL == 2
    static SDL_Window* g_sdlWindow = NULL;
    static SDL_Renderer* g_sdlRenderer = NULL;

    #if A_CONFIG_RENDER_SOFTWARE
        static SDL_Texture* g_sdlTexture = NULL;
    #endif

    static uint8_t g_clearR, g_clearG, g_clearB;
#endif

#if A_CONFIG_LIB_SDL == 2
static void clearScreen(void)
{
    if(SDL_SetRenderDrawColor(g_sdlRenderer,
                              g_clearR,
                              g_clearG,
                              g_clearB,
                              SDL_ALPHA_OPAQUE) < 0) {

        a_out__error("SDL_SetRenderDrawColor failed: %s", SDL_GetError());
    }

    if(SDL_RenderClear(g_sdlRenderer) < 0) {
        a_out__error("SDL_RenderClear failed: %s", SDL_GetError());
    }
}
#endif

void a_sdl_video__init(void)
{
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        a_out__fatal("SDL_InitSubSystem: %s", SDL_GetError());
    }

    #if A_PLATFORM_PANDORA
        putenv("SDL_VIDEODRIVER=omapdss");
        putenv("SDL_OMAP_LAYER_SIZE=pixelperfect");
    #endif
}

void a_sdl_video__uninit(void)
{
    #if A_CONFIG_LIB_SDL == 1
        if(!a_settings_getBool("video.doubleBuffer")) {
            if(SDL_MUSTLOCK(g_sdlScreen)) {
                SDL_UnlockSurface(g_sdlScreen);
            }
        }
    #elif A_CONFIG_LIB_SDL == 2
        #if A_CONFIG_RENDER_SOFTWARE
            SDL_DestroyTexture(g_sdlTexture);
        #endif
        SDL_DestroyRenderer(g_sdlRenderer);
        SDL_DestroyWindow(g_sdlWindow);
    #endif

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void a_sdl_screen__set(void)
{
    #if A_CONFIG_LIB_SDL == 1
        int bpp = 0;
        uint32_t videoFlags = SDL_SWSURFACE;

        if(a_settings_getBool("video.fullscreen")) {
            videoFlags |= SDL_FULLSCREEN;
        }

        bpp = SDL_VideoModeOK(a__screen.width,
                              a__screen.height,
                              A_PIXEL_BPP,
                              videoFlags);
        if(bpp == 0) {
            a_out__fatal("SDL: %dx%d:%d video not available",
                         a__screen.width,
                         a__screen.height,
                         A_PIXEL_BPP);
        }

        g_sdlScreen = SDL_SetVideoMode(a__screen.width,
                                       a__screen.height,
                                       A_PIXEL_BPP,
                                       videoFlags);
        if(g_sdlScreen == NULL) {
            a_out__fatal("SDL: %s", SDL_GetError());
        }

        SDL_SetClipRect(g_sdlScreen, NULL);

        if(!a_settings_getBool("video.doubleBuffer")) {
            if(SDL_MUSTLOCK(g_sdlScreen)) {
                if(SDL_LockSurface(g_sdlScreen) < 0) {
                    a_out__fatal("SDL_LockSurface failed: %s", SDL_GetError());
                }
            }
        }

        a_screen__setPixelBuffer(g_sdlScreen->pixels);
    #elif A_CONFIG_LIB_SDL == 2
        int ret;
        uint32_t windowFlags = SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE;

        if(a_settings_getBool("video.fullscreen")) {
            windowFlags |= SDL_WINDOW_FULLSCREEN;
        }

        g_sdlWindow = SDL_CreateWindow("",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       a__screen.width,
                                       a__screen.height,
                                       windowFlags);
        if(g_sdlWindow == NULL) {
            a_out__fatal("SDL_CreateWindow failed: %s", SDL_GetError());
        }

        g_sdlRenderer = SDL_CreateRenderer(g_sdlWindow,
                                           -1,
                                           0);
        if(g_sdlRenderer == NULL) {
            a_out__fatal("SDL_CreateRenderer failed: %s", SDL_GetError());
        }

        ret = SDL_RenderSetLogicalSize(g_sdlRenderer,
                                       a__screen.width,
                                       a__screen.height);
        if(ret < 0) {
            a_out__fatal("SDL_RenderSetLogicalSize failed: %s", SDL_GetError());
        }

        #if A_CONFIG_RENDER_SOFTWARE
            g_sdlTexture = SDL_CreateTexture(g_sdlRenderer,
                                             #if A_PIXEL_BPP == 16
                                                 SDL_PIXELFORMAT_RGB565,
                                             #elif A_PIXEL_BPP == 32
                                                 SDL_PIXELFORMAT_RGBX8888,
                                             #endif
                                             SDL_TEXTUREACCESS_STREAMING,
                                             a__screen.width,
                                             a__screen.height);
            if(g_sdlTexture == NULL) {
                a_out__fatal("SDL_CreateTexture failed: %s", SDL_GetError());
            }
        #endif

        SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY,
                                "nearest",
                                SDL_HINT_OVERRIDE);

        char* end;
        const char* color = a_settings_getString("video.borderColor");
        g_clearR = (uint8_t)strtol(color, &end, 0);
        g_clearG = (uint8_t)strtol(end, &end, 0);
        g_clearB = (uint8_t)strtol(end, NULL, 0);

        clearScreen();
    #endif

    #if A_PLATFORM_LINUXPC
        char caption[64];
        snprintf(caption, 64, "%s %s",
            a_settings_getString("app.title"),
            a_settings_getString("app.version"));

        #if A_CONFIG_LIB_SDL == 1
            SDL_WM_SetCaption(caption, NULL);
        #elif A_CONFIG_LIB_SDL == 2
            SDL_SetWindowTitle(g_sdlWindow, caption);
        #endif
    #else
        SDL_ShowCursor(SDL_DISABLE);
    #endif
}

void a_sdl_screen__show(void)
{
    #if A_CONFIG_LIB_SDL == 1
        #if A_PLATFORM_WIZ
            if(a_settings_getBool("video.fixWizTearing")) { // also video.doubleBuffer
                #define A_WIDTH 320
                #define A_HEIGHT 240

                if(SDL_MUSTLOCK(g_sdlScreen)) {
                    if(SDL_LockSurface(g_sdlScreen) < 0) {
                        a_out__fatal("SDL_LockSurface failed: %s",
                                     SDL_GetError());
                    }
                }

                APixel* dst = (APixel*)g_sdlScreen->pixels + A_WIDTH * A_HEIGHT;
                const APixel* src = a__screen.pixels;

                for(int i = A_HEIGHT; i--; dst += A_WIDTH * A_HEIGHT + 1) {
                    for(int j = A_WIDTH; j--; ) {
                        dst -= A_HEIGHT;
                        *dst = *src++;
                    }
                }

                if(SDL_MUSTLOCK(g_sdlScreen)) {
                    SDL_UnlockSurface(g_sdlScreen);
                }

                SDL_Flip(g_sdlScreen);
                return;
            }
        #endif

        if(a_settings_getBool("video.doubleBuffer")) {
            if(SDL_MUSTLOCK(g_sdlScreen)) {
                if(SDL_LockSurface(g_sdlScreen) < 0) {
                    a_out__fatal("SDL_LockSurface failed: %s",
                                 SDL_GetError());
                }
            }

            memcpy(g_sdlScreen->pixels,
                   a__screen.pixels,
                   a__screen.width * a__screen.height * sizeof(APixel));

            if(SDL_MUSTLOCK(g_sdlScreen)) {
                SDL_UnlockSurface(g_sdlScreen);
            }

            SDL_Flip(g_sdlScreen);
        } else {
            if(SDL_MUSTLOCK(g_sdlScreen)) {
                SDL_UnlockSurface(g_sdlScreen);
            }

            SDL_Flip(g_sdlScreen);

            if(SDL_MUSTLOCK(g_sdlScreen)) {
                if(SDL_LockSurface(g_sdlScreen) < 0) {
                    a_out__fatal("SDL_LockSurface failed: %s",
                                 SDL_GetError());
                }
            }

            a_screen__setPixelBuffer(g_sdlScreen->pixels);
        }
    #elif A_CONFIG_LIB_SDL == 2
        #if A_CONFIG_RENDER_SOFTWARE
            if(SDL_UpdateTexture(g_sdlTexture,
                                 NULL,
                                 a__screen.pixels,
                                 a__screen.width * (int)sizeof(APixel)) < 0) {

                a_out__fatal("SDL_UpdateTexture failed: %s", SDL_GetError());
            }

            if(SDL_RenderCopy(g_sdlRenderer, g_sdlTexture, NULL, NULL) < 0) {
                a_out__fatal("SDL_RenderCopy failed: %s", SDL_GetError());
            }
        #endif

        SDL_RenderPresent(g_sdlRenderer);
        clearScreen();
    #endif
}

void a_sdl_screen__setFullScreen(bool FullScreen)
{
    #if A_CONFIG_LIB_SDL == 1
        FullScreen = FullScreen;
    #elif A_CONFIG_LIB_SDL == 2
        uint32_t flag = FullScreen ? SDL_WINDOW_FULLSCREEN : 0;
        int ret = SDL_SetWindowFullscreen(g_sdlWindow, flag);

        if(ret < 0) {
            a_out__error("SDL_SetWindowFullscreen failed: %s", SDL_GetError());
        }
    #endif
}

#if A_CONFIG_RENDER_SDL2
void a_sdl_render__setDrawColor(void)
{
    if(SDL_SetRenderDrawColor(g_sdlRenderer,
                              (uint8_t)a_pixel__state.red,
                              (uint8_t)a_pixel__state.green,
                              (uint8_t)a_pixel__state.blue,
                              (uint8_t)a_pixel__state.alpha) < 0) {

        a_out__error("SDL_SetRenderDrawColor failed: %s", SDL_GetError());
    }
}

void a_sdl_render__setBlendMode(void)
{
    SDL_BlendMode mode = SDL_BLENDMODE_NONE;

    if(a_pixel__state.blend >= A_PIXEL_BLEND_RGBA
        && a_pixel__state.blend <= A_PIXEL_BLEND_RGB75) {

        mode = SDL_BLENDMODE_BLEND;
    }

    if(SDL_SetRenderDrawBlendMode(g_sdlRenderer, mode) < 0) {
        a_out__error("SDL_SetRenderDrawBlendMode failed: %s", SDL_GetError());
    }
}

void a_sdl_render__fillRect(int X, int Y, int Width, int Height)
{
    SDL_Rect area = {X, Y, Width, Height};

    if(SDL_RenderFillRect(g_sdlRenderer, &area) < 0) {
        a_out__error("SDL_RenderFillRect failed: %s", SDL_GetError());
    }
}

void* a_sdl_render__makeTexture(APixel* Pixels, int Width, int Height)
{
    SDL_Surface* s = SDL_CreateRGBSurfaceFrom(Pixels,
                                              Width,
                                              Height,
                                              A_PIXEL_BPP,
                                              Width * (int)sizeof(APixel),
                                              (APixel)A_PIXEL_RED_MASK << A_PIXEL_RED_SHIFT,
                                              (APixel)A_PIXEL_GREEN_MASK << A_PIXEL_GREEN_SHIFT,
                                              (APixel)A_PIXEL_BLUE_MASK << A_PIXEL_BLUE_SHIFT,
                                              0);
    if(s == NULL) {
        a_out__fatal("SDL_CreateRGBSurfaceFrom failed: %s", SDL_GetError());
    }

    SDL_Texture* t = SDL_CreateTextureFromSurface(g_sdlRenderer, s);
    if(t == NULL) {
        a_out__fatal("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
    }

    SDL_FreeSurface(s);

    return t;
}

void a_sdl_render__freeTexture(void* Texture)
{
    SDL_DestroyTexture(Texture);
}

void a_sdl_render__blitTexture(void* Texture, int X, int Y, int Width, int Height)
{
    SDL_Texture* t = Texture;
    SDL_Rect dest = {X, Y, Width, Height};
    SDL_BlendMode mode = SDL_BLENDMODE_NONE;

    if(a_pixel__state.blend >= A_PIXEL_BLEND_RGBA
        && a_pixel__state.blend <= A_PIXEL_BLEND_RGB75) {

        mode = SDL_BLENDMODE_BLEND;
    }

    if(SDL_SetTextureBlendMode(t, mode) < 0) {
        a_out__error("SDL_SetTextureBlendMode failed: %s", SDL_GetError());
    }

    SDL_RenderCopy(g_sdlRenderer, t, NULL, &dest);
}
#endif
