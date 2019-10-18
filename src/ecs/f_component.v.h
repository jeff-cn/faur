/*
    Copyright 2016-2019 Alex Margarit <alex@alxm.org>
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

#ifndef A_INC_ECS_COMPONENT_V_H
#define A_INC_ECS_COMPONENT_V_H

#include "ecs/f_component.p.h"

typedef struct AComponent AComponent;

#include "memory/f_mem.v.h"

typedef struct {
    const AComponent* component; // shared data for all components of same type
    AEntity* entity; // entity this component belongs to
    AMaxMemAlignType buffer[];
} AComponentInstance;

extern void f_component__init(void);
extern void f_component__uninit(void);

extern int f_component__stringToIndex(const char* StringId);
extern const AComponent* f_component__get(int Index);

extern const char* f_component__stringGet(const AComponent* Component);

extern void* f_component__templateInit(const AComponent* Component, const ABlock* Block);
extern void f_component__templateFree(const AComponent* Component, void* Buffer);

extern AComponentInstance* f_component__instanceNew(const AComponent* Component, AEntity* Entity, const void* TemplateData);
extern void f_component__instanceFree(AComponentInstance* Instance);

#endif // A_INC_ECS_COMPONENT_V_H
