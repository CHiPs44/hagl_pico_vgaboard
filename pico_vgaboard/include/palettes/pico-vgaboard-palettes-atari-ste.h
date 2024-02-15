/*

MIT License

Copyright (c) 2021-2024 CHiPs44 <chips44@gmail.com>

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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_ATARI_STE_H
#define _HAGL_PICO_VGABOARD_PALETTES_ATARI_STE_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*
        cf. https://github.com/sarnau/AtariSTRomListings/blob/master/TOS206%2C206STBook%2C206.asm#L1624

        *+
        * Default palette assignments.
        *  Sort of corresponding to the GSX spec.
        *-
        colors:     DC.W      $0fff                     ; 0 white
                    DC.W      $0f00                     ; 1 red
                    DC.W      $00f0                     ; 2 green
                    DC.W      $0ff0                     ; 3 yellow
                    DC.W      $000f                     ; 4 blue
                    DC.W      $0f0f                     ; 5 magenta
                    DC.W      $00ff                     ; 6 cyan
                    DC.W      $0555                     ; 7 "low white"
                    DC.W      $0333                     ; 8 grey
                    DC.W      $0f33                     ; 9 light red
                    DC.W      $03f3                     ; 10 light green
                    DC.W      $0ff3                     ; 11 light yellow
                    DC.W      $033f                     ; 12 light blue
                    DC.W      $0f3f                     ; 13 light magenta
                    DC.W      $03ff                     ; 14 light cyan
                    DC.W      $0000                     ; 15 black

        Each 4 bits component is translated as :
         - 0 => 0
         - 1..15 => x * 2 + 1

    */

    /* clang-format off */

#define ATARI_STE_00_WHITE         PICO_SCANVIDEO_PIXEL_FROM_RGB5((0xfu << 1) + 1, (0xfu << 1) + 1, (0xfu << 1) + 1) /* $0fff */
#define ATARI_STE_01_RED           PICO_SCANVIDEO_PIXEL_FROM_RGB5((0xfu << 1) + 1, (0x0u << 1) + 0, (0x0u << 1) + 0) /* $0f00 */
#define ATARI_STE_02_GREEN         PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x0u << 1) + 0, (0xfu << 1) + 1, (0x0u << 1) + 0) /* $00f0 */
#define ATARI_STE_03_YELLOW        PICO_SCANVIDEO_PIXEL_FROM_RGB5((0xfu << 1) + 1, (0xfu << 1) + 1, (0x0u << 1) + 0) /* $0ff0 */
#define ATARI_STE_04_BLUE          PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x0u << 1) + 0, (0x0u << 1) + 0, (0xfu << 1) + 1) /* $0ff0 */
#define ATARI_STE_05_MAGENTA       PICO_SCANVIDEO_PIXEL_FROM_RGB5((0xfu << 1) + 1, (0x0u << 1) + 0, (0xfu << 1) + 1) /* $0f0f */
#define ATARI_STE_06_CYAN          PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x0u << 1) + 0, (0xfu << 1) + 1, (0xfu << 1) + 1) /* $00ff */
#define ATARI_STE_07_LOW_WHITE     PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x5u << 1) + 1, (0x5u << 1) + 1, (0x5u << 1) + 1) /* $0555 */
#define ATARI_STE_08_GREY          PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x3u << 1) + 1, (0x3u << 1) + 1, (0x3u << 1) + 1) /* $0333 */
#define ATARI_STE_09_LIGHT_RED     PICO_SCANVIDEO_PIXEL_FROM_RGB5((0xfu << 1) + 1, (0x3u << 1) + 1, (0x3u << 1) + 1) /* $0f33 */
#define ATARI_STE_10_LIGHT_GREEN   PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x3u << 1) + 1, (0xfu << 1) + 1, (0x3u << 1) + 1) /* $03f3 */
#define ATARI_STE_11_LIGHT_YELLOW  PICO_SCANVIDEO_PIXEL_FROM_RGB5((0xfu << 1) + 1, (0xfu << 1) + 1, (0x3u << 1) + 1) /* $0ff3 */
#define ATARI_STE_12_LIGHT_BLUE    PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x3u << 1) + 1, (0x3u << 1) + 1, (0xfu << 1) + 1) /* $033f */
#define ATARI_STE_13_LIGHT_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x3u << 1) + 1, (0x3u << 1) + 1, (0xfu << 1) + 1) /* $033f */
#define ATARI_STE_14_LIGHT_CYAN    PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x3u << 1) + 1, (0xfu << 1) + 1, (0xfu << 1) + 1) /* $03ff */
#define ATARI_STE_15_BLACK         PICO_SCANVIDEO_PIXEL_FROM_RGB5((0x0u << 1) + 0, (0x0u << 1) + 0, (0x0u << 1) + 0) /* $0000 */

    /* clang-format on */

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_ATARI_STE_H */
