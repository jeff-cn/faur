/*
    Copyright 2010, 2016 Alex Margarit

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

#include "a2x_pack_font.v.h"
#include "media/font.h"

#define CHAR_ENTRIES_NUM    128
#define CHAR_TO_INDEX(Char) ((unsigned)Char & (CHAR_ENTRIES_NUM - 1))

#define BLANK_SPACE  3
#define CHAR_SPACING 1
#define LINE_SPACING 1

typedef struct AFont {
    ASprite* sprites[CHAR_ENTRIES_NUM];
    int maxWidth;
    int maxHeight;
} AFont;

static const char g_chars[] =
    "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
    "0123456789"
    "_-+=*/\\&$@!?'\"()[]{}.,~:;%^#<>|`";

#define CHARS_NUM (sizeof(g_chars) - 1)

typedef struct AFontState {
    unsigned currentFont;
    AFontAlign align;
    int x, startX, y;
    int lineHeight;
    int wrapWidth, currentLineWidth;
} AFontState;

static AList* g_fontsList;
static AFont** g_fonts;
static AFontState g_state;
static AList* g_stateStack;

static unsigned charIndex(char Character)
{
    for(unsigned i = 0; i < CHARS_NUM; i++) {
        if(g_chars[i] == Character) {
            return i;
        }
    }

    a_out__fatal("Unsupported character '%c'", Character);
}

void a_font__init(void)
{
    g_fontsList = a_list_new();
    g_fonts = NULL;

    g_stateStack = a_list_new();

    ASprite* fontSprite = a_sprite_newFromData(g_media_font, "defaultFont");

    APixel colors[A_FONT_FACE_DEFAULT_NUM];
    colors[A_FONT_FACE_WHITE] = a_pixel_hex(0xffffff);
    colors[A_FONT_FACE_LIGHT_GRAY] = a_pixel_hex(0xafafaf);
    colors[A_FONT_FACE_GREEN] = a_pixel_hex(0x3fbf9f);
    colors[A_FONT_FACE_YELLOW] = a_pixel_hex(0x9fcf3f);
    colors[A_FONT_FACE_RED] = a_pixel_hex(0xcf2f4f);
    colors[A_FONT_FACE_BLUE] = a_pixel_hex(0x3f8fdf);

    a_font_load(fontSprite, 0, 0, A_FONT_LOAD_ALL);
    a_sprite_free(fontSprite);

    for(unsigned f = 1; f < A_FONT_FACE_DEFAULT_NUM; f++) {
        a_font_dup(A_FONT_FACE_WHITE, colors[f]);
    }

    a_font_reset();
}

void a_font__uninit(void)
{
    A_LIST_ITERATE(g_stateStack, AFontState*, state) {
        free(state);
    }

    A_LIST_ITERATE(g_fontsList, AFont*, f) {
        free(f);
    }

    a_list_free(g_stateStack);
    a_list_free(g_fontsList);
    free(g_fonts);
}

unsigned a_font_load(const ASprite* Sheet, int X, int Y, AFontLoad Loader)
{
    AFont* f = a_mem_malloc(sizeof(AFont));

    for(int i = CHAR_ENTRIES_NUM; i--; ) {
        f->sprites[i] = NULL;
    }

    f->maxWidth = 0;
    f->maxHeight = 0;

    a_list_addLast(g_fontsList, f);

    free(g_fonts);
    g_fonts = (AFont**)a_list_toArray(g_fontsList);

    unsigned start = 0;
    unsigned end = CHARS_NUM - 1;

    if(Loader & A_FONT_LOAD_ALPHANUMERIC) {
        end = charIndex('9');
    } else if(Loader & A_FONT_LOAD_ALPHA) {
        end = charIndex('z');
    } else if(Loader & A_FONT_LOAD_NUMERIC) {
        start = charIndex('0');
        end = charIndex('9');
    }

    ASpriteFrames* sf = a_spriteframes_new(Sheet, X, Y, 1);

    for(unsigned i = start; i <= end; i++) {
        ASprite* spr = a_spriteframes_next(sf);

        f->sprites[CHAR_TO_INDEX(g_chars[i])] = spr;
        f->maxWidth = a_math_max(f->maxWidth, spr->w);
        f->maxHeight = a_math_max(f->maxHeight, spr->h);

        if((Loader & A_FONT_LOAD_CAPS) && isalpha(g_chars[i])) {
            f->sprites[CHAR_TO_INDEX(g_chars[i + 1])] = spr;
            i++;
        }
    }

    a_spriteframes_free(sf, false);

    return a_list_getSize(g_fontsList) - 1;
}

unsigned a_font_dup(unsigned Font, APixel Color)
{
    AFont* src = g_fonts[Font];
    AFont* f = a_mem_malloc(sizeof(AFont));

    *f = *src;

    a_pixel_push();
    a_pixel_setPixel(Color);
    a_pixel_setBlitFillFlat(true);

    for(int i = CHAR_ENTRIES_NUM; i--; ) {
        ASprite* sprite = NULL;
        ASprite* srcSprite = src->sprites[i];

        if(srcSprite) {
            #if A_CONFIG_RENDER_SOFTWARE
                sprite = a_sprite_newBlank(srcSprite->w,
                                           srcSprite->h,
                                           srcSprite->colorKeyed);
            #elif A_CONFIG_RENDER_SDL2
                sprite = a_sprite_newBlank(srcSprite->w,
                                           srcSprite->h,
                                           true);
            #endif

            a_screen_targetPushSprite(sprite);
            a_sprite_blit(srcSprite, 0, 0);
            a_screen_targetPop();
        }

        f->sprites[i] = sprite;
    }

    a_pixel_pop();

    a_list_addLast(g_fontsList, f);

    free(g_fonts);
    g_fonts = (AFont**)a_list_toArray(g_fontsList);

    return a_list_getSize(g_fontsList) - 1;
}

void a_font_push(void)
{
    AFontState* state = a_mem_malloc(sizeof(AFontState));

    *state = g_state;
    a_list_push(g_stateStack, state);
}

void a_font_pop(void)
{
    AFontState* state = a_list_pop(g_stateStack);

    if(state == NULL) {
        a_out__fatal("Cannot pop AFontState: stack is empty");
    }

    g_state = *state;
    free(state);
}

void a_font_reset(void)
{
    a_font_setFace(A_FONT_FACE_WHITE);
    a_font_setAlign(A_FONT_ALIGN_LEFT);
    a_font_setCoords(0, 0);
    a_font_setWrap(0);
}

void a_font_setFace(unsigned Font)
{
    g_state.currentFont = Font;
    g_state.lineHeight = g_fonts[Font]->maxHeight + LINE_SPACING;
}

void a_font_setAlign(AFontAlign Align)
{
    g_state.align = Align;
}

void a_font_setCoords(int X, int Y)
{
    g_state.x = g_state.startX = X;
    g_state.y = Y;
    g_state.currentLineWidth = 0;
}

int a_font_getX(void)
{
    return g_state.x;
}

int a_font_getY(void)
{
    return g_state.y;
}

void a_font_newLine(void)
{
    g_state.x = g_state.startX;
    g_state.y += g_state.lineHeight;
    g_state.currentLineWidth = 0;
}

int a_font_getLineHeight(void)
{
    return g_state.lineHeight;
}

void a_font_setLineHeight(int Height)
{
    g_state.lineHeight = Height;
}

void a_font_setWrap(int Width)
{
    g_state.wrapWidth = Width;
    g_state.currentLineWidth = 0;
}

static int getWidth(const char* Text, ptrdiff_t Length)
{
    int width = 0;
    AFont* font = g_fonts[g_state.currentFont];

    if(g_state.align & A_FONT_ALIGN_MONOSPACED) {
        width = (font->maxWidth + CHAR_SPACING) * (int)Length;
    } else {
        for( ; Length--; Text++) {
            ASprite* spr = font->sprites[CHAR_TO_INDEX(*Text)];
            width += (spr ? spr->w : BLANK_SPACE) + CHAR_SPACING;
        }
    }

    if(width > 0) {
        width -= CHAR_SPACING;
    }

    return width;
}

static void drawString(const char* Text, ptrdiff_t Length)
{
    AFont* font = g_fonts[g_state.currentFont];

    if(g_state.align & A_FONT_ALIGN_MIDDLE) {
        g_state.x -= getWidth(Text, Length) / 2;
    } else if(g_state.align & A_FONT_ALIGN_RIGHT) {
        g_state.x -= getWidth(Text, Length);
    }

    if(g_state.align & A_FONT_ALIGN_MONOSPACED) {
        for( ; Length--; Text++) {
            ASprite* spr = font->sprites[CHAR_TO_INDEX(*Text)];

            if(spr) {
                a_sprite_blit(spr,
                              g_state.x + (font->maxWidth - spr->w) / 2,
                              g_state.y);
            }

            g_state.x += font->maxWidth + CHAR_SPACING;
        }
    } else {
        for( ; Length--; Text++) {
            ASprite* spr = font->sprites[CHAR_TO_INDEX(*Text)];

            if(spr) {
                a_sprite_blit(spr, g_state.x, g_state.y);
                g_state.x += spr->w + CHAR_SPACING;
            } else {
                g_state.x += BLANK_SPACE + CHAR_SPACING;
            }
        }
    }
}

static void wrapString(const char* Text)
{
    AFont* font = g_fonts[g_state.currentFont];
    const char* lineStart = Text;

    while(*Text != '\0') {
        int tally = 0;

        if(g_state.currentLineWidth > 0) {
            if(g_state.align & A_FONT_ALIGN_MONOSPACED) {
                for( ; *Text == ' '; Text++) {
                    tally += font->maxWidth + CHAR_SPACING;
                }
            } else {
                for( ; *Text == ' '; Text++) {
                    tally += BLANK_SPACE + CHAR_SPACING;
                }
            }
        } else {
            // Do not start a line with spaces
            for( ; *Text == ' '; Text++) {
                lineStart++;
            }
        }

        const char* wordStart = Text;

        if(g_state.currentLineWidth + tally <= g_state.wrapWidth) {
            if(g_state.align & A_FONT_ALIGN_MONOSPACED) {
                for( ; *Text != ' ' && *Text != '\0'; Text++) {
                    tally += font->maxWidth + CHAR_SPACING;
                }
            } else {
                for( ; *Text != ' ' && *Text != '\0'; Text++) {
                    ASprite* spr = font->sprites[CHAR_TO_INDEX(*Text)];
                    tally += (spr ? spr->w : BLANK_SPACE) + CHAR_SPACING;
                }
            }
        }

        if(tally > 0) {
            tally -= CHAR_SPACING;
        }

        if(g_state.currentLineWidth + tally > g_state.wrapWidth) {
            if(g_state.currentLineWidth == 0) {
                // Print lone word regardless of overflow
                drawString(lineStart, Text - lineStart);
            } else {
                // Print line up to overflowing word, and go back to word
                drawString(lineStart, wordStart - lineStart);
                Text = wordStart;
            }

            lineStart = Text;
            g_state.currentLineWidth = 0;
            a_font_newLine();
        } else {
            if(*Text == '\0') {
                // Print last line
                drawString(lineStart, Text - lineStart);
            }

            if(tally > 0) {
                g_state.currentLineWidth += tally + CHAR_SPACING;
            }
        }
    }
}

void a_font_print(const char* Text)
{
    if(g_state.wrapWidth > 0) {
        wrapString(Text);
    } else {
        drawString(Text, (ptrdiff_t)strlen(Text));
    }
}

void a_font_printf(const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    a_font_printv(Format, args);

    va_end(args);
}

void a_font_printv(const char* Format, va_list Args)
{
    char buffer[256];

    if(vsnprintf(buffer, sizeof(buffer), Format, Args) > 0) {
        a_font_print(buffer);
    }
}

int a_font_getWidth(const char* Text)
{
    return getWidth(Text, (ptrdiff_t)strlen(Text));
}

int a_font_getWidthf(const char* Format, ...)
{
    int width;
    va_list args;

    va_start(args, Format);
    width = a_font_getWidthv(Format, args);
    va_end(args);

    return width;
}

int a_font_getWidthv(const char* Format, va_list Args)
{
    char buffer[256];

    if(vsnprintf(buffer, sizeof(buffer), Format, Args) > 0) {
        return a_font_getWidth(buffer);
    }

    return 0;
}
