/*
    Copyright 2016-2018 Alex Margarit <alex@alxm.org>
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

#include "f_pandora.v.h"
#include <faur.v.h>

#if A_CONFIG_SYSTEM_PANDORA
static char* g_nubModes[2];

static void pandora_setNubModes(const char* Nub0, const char* Nub1)
{
    const char* cmd = f_str__fmt512(
                        "/usr/pandora/scripts/op_nubchange.sh %s %s",
                        Nub0,
                        Nub1);

    if(cmd == NULL) {
        return;
    }

    f_out__info("Set nub modes to '%s', '%s'", Nub0, Nub1);

    if(system(cmd) < 0) {
        f_out__error("%s: failed", cmd);
        return;
    }
}

static void pandora_setScreenFilter(const char* Value)
{
    const char* cmd = f_str__fmt512(
                        "sudo -n /usr/pandora/scripts/op_videofir.sh %s",
                        Value);

    if(cmd == NULL) {
        return;
    }

    f_out__info("Set screen filter to '%s'", Value);

    if(system(cmd) < 0) {
        f_out__error("%s: failed", cmd);
        return;
    }
}

void f_platform_pandora__init(void)
{
    for(int i = 0; i < 2; i++) {
        const char* path = f_str__fmt512("/proc/pandora/nub%d/mode", i);
        AFile* nub = f_file_new(path, A_FILE_READ);

        if(nub && f_file_lineRead(nub)) {
            g_nubModes[i] = f_str_dup(f_file_lineBufferGet(nub));
            f_out__info("Current nub%d mode is '%s'", i, g_nubModes[i]);
        }

        f_file_free(nub);
    }

    // Gamepad mode
    pandora_setNubModes("absolute", "absolute");

    // Crisp pixels, no blur filter
    pandora_setScreenFilter("none");
}

void f_platform_pandora__uninit(void)
{
    pandora_setScreenFilter("default");

    if(g_nubModes[0] && g_nubModes[1]) {
        pandora_setNubModes(g_nubModes[0], g_nubModes[1]);
    }

    for(int i = 0; i < 2; i++) {
        f_mem_free(g_nubModes[i]);
    }
}
#endif // A_CONFIG_SYSTEM_PANDORA
