/*
    Copyright 2017 Alex Margarit

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

#pragma once

#include "a2x_pack_platform.p.h"

#include "a2x_pack_platform_gp2x.v.h"
#include "a2x_pack_platform_pandora.v.h"
#include "a2x_pack_platform_sdl.v.h"
#include "a2x_pack_platform_wiz.v.h"

extern void a_platform__init(void);
extern void a_platform__uninit(void);

extern uint32_t a_platform__getMs(void);

extern void a_platform__setScreen(int Width, int Height, bool FullScreen);
extern void a_platform__showScreen(void);
extern void a_platform__setFullScreen(bool FullScreen);
extern void a_platform__getNativeResolution(int* Width, int* Height);

extern void a_platform__renderClear(void);
extern void a_platform__renderSetDrawColor(void);
extern void a_platform__renderSetBlendMode(void);

extern void a_platform__drawPixel(int X, int Y);
extern void a_platform__drawLine(int X1, int Y1, int X2, int Y2);
extern void a_platform__drawRectangleFilled(int X, int Y, int Width, int Height);
extern void a_platform__drawRectangleOutline(int X, int Y, int Width, int Height);
extern void a_platform__drawCircleOutline(int X, int Y, int Radius);
extern void a_platform__drawCircleFilled(int X, int Y, int Radius);
