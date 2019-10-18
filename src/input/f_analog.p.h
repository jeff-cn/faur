/*
    Copyright 2010, 2017-2019 Alex Margarit <alex@alxm.org>
    This file is part of Faur, a C video game framework.

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

#ifndef A_INC_INPUTS_ANALOG_P_H
#define A_INC_INPUTS_ANALOG_P_H

#include "general/f_system_includes.h"

typedef struct AAnalog AAnalog;

typedef enum {
    A_AXIS_INVALID = -1,
    A_AXIS_LEFTX,
    A_AXIS_LEFTY,
    A_AXIS_RIGHTX,
    A_AXIS_RIGHTY,
    A_AXIS_LEFTTRIGGER,
    A_AXIS_RIGHTTRIGGER,
    A_AXIS_NUM
} AAnalogId;

#include "input/f_controller.v.h"
#include "math/f_fix.p.h"

extern AAnalog* f_analog_new(void);
extern void f_analog_free(AAnalog* Analog);

extern void f_analog_bind(AAnalog* Analog, const AController* Controller, AAnalogId Id);

extern bool f_analog_isWorking(const AAnalog* Analog);
extern const char* f_analog_nameGet(const AAnalog* Analog);

extern AFix f_analog_valueGet(const AAnalog* Analog);

#endif // A_INC_INPUTS_ANALOG_P_H
