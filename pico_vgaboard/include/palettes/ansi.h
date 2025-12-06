/*

MIT License

Copyright (c) 2025 CHiPs44 <chips44@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

This file is part of the Raspberry Pi Pico VGA board HAL for the HAGL graphics library:

https://github.com/CHiPs44/hagl_pico_vgaboard

https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#ifndef _PALETTE_RGB121_H
#define _PALETTE_RGB121_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Let's go for the 8 dark colors */

#define ANSI_BLACK          PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x00, 0x00, 0x00)
#define ANSI_DARK_RED       PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x10, 0x00, 0x00)
#define ANSI_DARK_GREEN     PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x00, 0x10, 0x00)
#define ANSI_DARK_YELLOW    PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x10, 0x10, 0x00)
#define ANSI_DARK_BLUE      PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x00, 0x00, 0x10)
#define ANSI_DARK_MAGENTA   PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x10, 0x00, 0x10)
#define ANSI_DARK_CYAN      PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x00, 0x10, 0x10)
#define ANSI_LIGHT_GREY     PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x15, 0x15, 0x15) // "Dark White"

/* And then the 8 brighter ones */

#define ANSI_DARK_GREY      PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x0a, 0x0a, 0x0a) // "Light Black"
#define ANSI_RED            PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1f, 0x00, 0x00)
#define ANSI_GREEN          PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x00, 0x1f, 0x00)
#define ANSI_YELLOW         PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1f, 0x1f, 0x00)
#define ANSI_BLUE           PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x00, 0x00, 0x1f)
#define ANSI_MAGENTA        PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1f, 0x00, 0x1f)
#define ANSI_CYAN           PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x00, 0x1f, 0x1f)
#define ANSI_WHITE          PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1f, 0x1f, 0x1f)

/** @brief ANSI 16 colors palette */
extern const BGAR5515 palette_4bpp_ansi[16];

/** @brief ANSI 256 colors palette (16 ANSI colors + 216 colors RGB cube + 24 grey levels) */
extern const BGAR5515 palette_8bpp_ansi[256];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_RGB121_H */

/* EOF */
