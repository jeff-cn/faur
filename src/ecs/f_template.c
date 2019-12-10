/*
    Copyright 2018-2019 Alex Margarit <alex@alxm.org>
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

#include "f_template.v.h"
#include <faur.v.h>

struct FTemplate {
    unsigned instanceNumber; // Incremented by each new entity
    FEntityInit* init; // Optional, runs after components init and parent init
    FBitfield* componentBits; // Set if template or a parent has component
    FBitfield* componentBitsOwn; // Set if template actually has the component
    const FTemplate* parent; // template chain
    void* data[]; // [f_init__ecs_com] Loaded and parsed config data, or NULL
};

static FStrHash* g_templates; // table of FTemplate

static FTemplate* templateNew(const char* Id)
{
    FTemplate* t = f_mem_zalloc(
                    sizeof(FTemplate) + sizeof(void*) * f_init__ecs_com);

    t->componentBits = f_bitfield_new(f_init__ecs_com);
    t->componentBitsOwn = f_bitfield_new(f_init__ecs_com);

    char* parentId = f_str_prefixGetToLast(Id, '.');

    if(parentId) {
        FTemplate* parentTemplate = f_strhash_get(g_templates, parentId);

        if(parentTemplate == NULL) {
            parentTemplate = templateNew(parentId);
        }

        t->parent = parentTemplate;

        for(unsigned c = f_init__ecs_com; c--; ) {
            if(f_bitfield_test(parentTemplate->componentBits, c)) {
                f_bitfield_set(t->componentBits, c);
                t->data[c] = parentTemplate->data[c];
            }
        }

        f_mem_free(parentId);
    }

    f_strhash_add(g_templates, Id, t);

    return t;
}

static void templateFree(FTemplate* Template)
{
    for(unsigned c = f_init__ecs_com; c--; ) {
        if(f_bitfield_test(Template->componentBitsOwn, c)
            && Template->data[c]) {

            f_component__templateFree(f_component__get(c), Template->data[c]);
        }
    }

    f_bitfield_free(Template->componentBits);
    f_bitfield_free(Template->componentBitsOwn);

    f_mem_free(Template);
}

void f_template__init(void)
{
    g_templates = f_strhash_new();
}

void f_template__uninit(void)
{
    f_strhash_freeEx(g_templates, (FFree*)templateFree);
}

void f_template_new(const char* FilePath)
{
    FBlock* root = f_block_new(FilePath);

    F_LIST_ITERATE(f_block_blocksGet(root), const FBlock*, b) {
        const char* templateId = f_block_lineGetString(b, 0);

        #if F_CONFIG_BUILD_DEBUG
            if(f_strhash_contains(g_templates, templateId)) {
                F__FATAL("f_template_new(%s): '%s' already declared",
                         FilePath,
                         templateId);
            }
        #endif

        FTemplate* t = templateNew(templateId);

        F_LIST_ITERATE(f_block_blocksGet(b), const FBlock*, b) {
            const char* componentId = f_block_lineGetString(b, 0);
            unsigned componentIndex = f_component__stringToIndex(componentId);

            if(componentIndex == UINT_MAX) {
                f_out__error("f_template_new(%s): Unknown component '%s'",
                             templateId,
                             componentId);

                continue;
            }

            f_bitfield_set(t->componentBits, componentIndex);
            f_bitfield_set(t->componentBitsOwn, componentIndex);
            t->data[componentIndex] =
                f_component__templateInit(f_component__get(componentIndex), b);
        }
    }

    f_block_free(root);
}

const FTemplate* f_template__get(const char* TemplateId)
{
    FTemplate* t = f_strhash_get(g_templates, TemplateId);

    #if F_CONFIG_BUILD_DEBUG
        if(t == NULL) {
            F__FATAL("Unknown template '%s'", TemplateId);
        }
    #endif

    t->instanceNumber++;

    return t;
}

void f_template__initRun(const FTemplate* Template, FEntity* Entity, const void* Context)
{
    if(Template->parent) {
        f_template__initRun(Template->parent, Entity, Context);
    }

    if(Template->init) {
        Template->init(Entity, Context);
    }
}

void f_template_init(const char* Id, FEntityInit* Init)
{
    FTemplate* t = f_strhash_get(g_templates, Id);

    #if F_CONFIG_BUILD_DEBUG
        if(t == NULL) {
            F__FATAL("f_template_init(%s): Unknown template", Id);
        }
    #endif

    t->init = Init;
}

unsigned f_template__instanceGet(const FTemplate* Template)
{
    return Template->instanceNumber;
}

bool f_template__componentHas(const FTemplate* Template, unsigned ComponentIndex)
{
    return f_bitfield_test(Template->componentBits, ComponentIndex);
}

const void* f_template__dataGet(const FTemplate* Template, unsigned ComponentIndex)
{
    return Template->data[ComponentIndex];
}
