/*
    Copyright 2010 Alex Margarit

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

#ifndef A2X_PACK_SPRITE_PH
#define A2X_PACK_SPRITE_PH

#include "a2x_app_includes.h"

typedef struct Sprite Sprite;

#include "a2x_pack_pixel.p.h"

extern Sprite* a_sprite_fromFile(const char* path);
extern Sprite* a_sprite_fromData(const uint8_t* data);

extern Sprite* a_sprite_new(const Sprite* sheet, int x, int y, int w, int h);
extern Sprite* a_sprite_zoomed(const Sprite* sheet, int x, int y, int w, int h, int zoom);
extern Sprite* a_sprite_blank(int w, int h);

extern int a_sprite_w(const Sprite* s);
extern int a_sprite_h(const Sprite* s);
extern Pixel* a_sprite_data(Sprite* s);

extern uint8_t a_sprite_getAlpha(const Sprite* s);
extern void a_sprite_setAlpha(Sprite* s, uint8_t a);

extern Pixel a_sprite_getPixel(const Sprite* s, int x, int y);

extern void a_sprite_refresh(Sprite* s);
extern Sprite* a_sprite_clone(const Sprite* src);

#endif // A2X_PACK_SPRITE_PH
