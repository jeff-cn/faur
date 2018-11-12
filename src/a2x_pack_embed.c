/*
    Copyright 2017, 2018 Alex Margarit

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

#include "a2x_pack_embed.v.h"

#include "a2x_pack_mem.v.h"
#include "a2x_pack_strhash.v.h"

#include "media/console.png.h"
#include "media/font.png.h"

typedef struct {
    const uint8_t* buffer;
    size_t size;
} AEmbeddedData;

static AStrHash* g_files; // table of AEmbeddedData

void a_embed__init(void)
{
    g_files = a_strhash_new();

    A_UNUSED(a__bin__media_console_png_path);
    A_UNUSED(a__bin__media_font_png_path);

    a__embed_addFile("/a2x/consoleTitles",
                     a__bin__media_console_png_data,
                     a__bin__media_console_png_size);

    a__embed_addFile("/a2x/defaultFont",
                     a__bin__media_font_png_data,
                     a__bin__media_font_png_size);

    a__embed_application();
}

void a_embed__uninit(void)
{
    a_strhash_freeEx(g_files, free);
}

void a__embed_addFile(const char* Path, const uint8_t* Buffer, size_t Size)
{
    AEmbeddedData* data = a_mem_malloc(sizeof(AEmbeddedData));

    data->buffer = Buffer;
    data->size = Size;

    a_strhash_add(g_files, Path, data);
}

bool a_embed__getFile(const char* Path, const uint8_t** Buffer, size_t* Size)
{
    AEmbeddedData* data = a_strhash_get(g_files, Path);

    if(data) {
        if(Buffer) {
            *Buffer = data->buffer;
        }

        if(Size) {
            *Size = data->size;
        }
    }

    return data != NULL;
}
