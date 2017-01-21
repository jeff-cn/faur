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

#pragma once

#include "a2x_system_includes.h"

extern void a_fps_frame(void);
extern bool a_fps_notSkipped(void);

extern unsigned a_fps_getFps(void);
extern unsigned a_fps_getMaxFps(void);
extern unsigned a_fps_getFrameSkip(void);
extern unsigned a_fps_getCounter(void);
extern bool a_fps_nthFrame(unsigned N);
