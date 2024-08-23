/*
    Copyright 2016 Alex Margarit <alex@alxm.org>
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

#include "f_ecs.v.h"
#include <faur.v.h>

static void f_ecs__init(void)
{
    if(f_component__num == 0 || f_system__num == 0) {
        return;
    }

    f_out__info("%u components, %u systems", f_component__num, f_system__num);

    f_component__init();
    f_system__init();
    f_entity__init();
}

static void f_ecs__uninit(void)
{
    if(f_ecs__isInit()) {
        f_entity__uninit();
        f_system__uninit();
        f_component__uninit();
    }
}

const FPack f_pack__ecs = {
    "ECS",
    f_ecs__init,
    f_ecs__uninit,
};
