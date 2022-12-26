/* SPDX-License-Identifier: MIT-0 */

// HAGL
#include "hagl_hal.h"
#include "hagl.h"

#include "./external/hagl/include/font5x7.h"
#include "./external/hagl/include/font5x8.h"
#include "./external/hagl/include/font6x9.h"
#include "./external/embedded-fonts/X11/include/font8x13.h"
#include "./external/embedded-fonts/X11/include/font8x13B.h"
#include "./external/embedded-fonts/X11/include/font6x10.h"
// #include "./external/embedded-fonts/misc/viznut/include/unscii-8.h"
// #include "./external/embedded-fonts/misc/viznut/include/unscii-8-fantasy.h"
#include "./external/fontx2-fonts/font8x8.h"
#include "./external/fontx2-fonts/BIOS_F08.h"

//                                                                                1234567890123456
font_t BIOS_F08         = { .fontx = BIOS_F08_fnt    , .w = 8, .h =  8, .name = L"BIOS F08 CP437"   };
font_t FONT5X7          = { .fontx = font5x7         , .w = 5, .h =  7, .name = L"X11 5x7"          };
font_t FONT5X8          = { .fontx = font5x8         , .w = 5, .h =  8, .name = L"X11 5x8"          };
font_t FONT6X9          = { .fontx = font6x9         , .w = 6, .h =  9, .name = L"X11 6x9"          };
font_t FONT8X8          = { .fontx = font8x8_fnt     , .w = 8, .h =  8, .name = L"BIOS 8x8 Unicode" };
// font_t UNSCII_8_FANTASY = { .fontx = unscii_8_fantasy, .w = 8, .h =  8, .name = L"Unscii 8 Fantasy" };
font_t FONT8X13         = { .fontx = font8x13        , .w = 8, .h = 13, .name = L"X11 8x13"         };
font_t FONT8X13B        = { .fontx = font8x13B       , .w = 8, .h = 13, .name = L"X11 8x13 Bold"    };
#define NFONTS 7
font_t *FONTS[NFONTS] = { 
    &BIOS_F08, 
    &FONT5X7, 
    &FONT5X8, 
    &FONT6X9, 
    &FONT8X8, 
    // &UNSCII8_FANTASY, 
    &FONT8X13, 
    &FONT8X13B
};

font_t *get_small_font(rect_t *window)
{
    return window->w <= 160 ? &FONT5X7 : &BIOS_F08;
}

font_t *get_text_font(rect_t *window)
{
    return window->w <= 80 ? &FONT8X8 : &FONT8X13;
}

font_t *get_title_font(rect_t *window)
{
    return window->w <= 80 ? &FONT8X8 : &FONT8X13B;
}

/* EOF */
