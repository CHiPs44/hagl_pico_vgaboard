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

#ifndef PICO_VGABOARD_MODES_TIC80_H
#define PICO_VGABOARD_MODES_TIC80_H

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
    240x136 * 1 => 240x136
        => 256x192  with  8 pixels horizontal margin and   8 pixels vertical margin
        => 320x200  with 40 pixels horizontal margin and  32 pixels vertical margin
        => 320x240  with 40 pixels horizontal margin and  52 pixels vertical margin
    240x136 * 2 => 480x272
        => 512x384  with 16 pixels horizontal margin and  56 pixels vertical margin
        => 640x360  with 80 pixels horizontal margin and  44 pixels vertical margin
        => 640x400  with 80 pixels horizontal margin and  64 pixels vertical margin
        => 640x480  with 80 pixels horizontal margin and 104 pixels vertical margin
    240x136 * 3 => 720x408
        => 768x576  with 24 pixels horizontal margin and  84 pixels vertical margin
        => 800x600  with 40 pixels horizontal margin and  96 pixels vertical margin
    240x136 * 4 => 960x544
        (does not fit in real 16:9 960x540)
        => 1024*576 with 32 pixels horizontal margin and  16 pixels vertical margin
    240x136 * 5 => 1200x680
        => 1280*720 with 40 pixels horizontal margin and  20 pixels vertical margin
*/

#ifdef __cplusplus
}
#endif

#endif /* PICO_VGABOARD_MODES_TIC80_H */
