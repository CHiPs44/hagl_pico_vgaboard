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

#ifndef PICO_VGABOARD_MODES_1024X600_H
#define PICO_VGABOARD_MODES_1024X600_H

/* clang-format off */

#if !PICO_NO_HARDWARE
#include "hardware/vreg.h"
#endif
#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PICO_VGABOARD_1024X600_FREQ_HZ        60
#define PICO_VGABOARD_1024X600_PIXEL_CLOCK_HZ 49000000L
/* Overclocking that should work on every RP2040/RP2350 */
#define PICO_VGABOARD_1024X600_SYS_CLOCK_KHZ  (4 * PICO_VGABOARD_1024X600_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1024X600_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#if !PICO_NO_HARDWARE
    #if PICO_RP2350
        /* RP2350 seems to be as overclockable as RP2040! */
        #if ALLOW_VREG_VOLTAGE_OVERRIDE
            #undef PICO_VGABOARD_1024X600_SYS_CLOCK_KHZ
            #undef PICO_VGABOARD_1024X600_VREG_VOLTAGE
            #define PICO_VGABOARD_1024X600_SYS_CLOCK_KHZ  (6 * PICO_VGABOARD_1024X600_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1024X600_VREG_VOLTAGE   (VREG_VOLTAGE_1_30)
        #endif
    #else
        #if ALLOW_VREG_VOLTAGE_OVERRIDE
            #undef PICO_VGABOARD_1024X600_SYS_CLOCK_KHZ
            #undef PICO_VGABOARD_1024X600_VREG_VOLTAGE
            #define PICO_VGABOARD_1024X600_SYS_CLOCK_KHZ  (6 * PICO_VGABOARD_1024X600_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1024X600_VREG_VOLTAGE   (VREG_VOLTAGE_1_20)
        #endif
    #endif
#endif

/**
 * @brief VGA timings for 1024x600@60Hz
 * cf. https://www.cse.cuhk.edu.hk/~mcyang/ceng3430/2122T2/Lec05%20Driving%20VGA%20Display%20with%20ZedBoard.pdf
 *  Format Pixel    Clock (MHz)     Horizontal (in Pixels)          Vertical (in Pixels)
 *                                  Active  Front   Sync    Back    Active  Front   Sync    Back
 *                                  Video   Porch   Pulse   Porch   Video   Porch   Pulse   Porch
 *  1024x600@60Hz   50.000          1024    32      48      240     600     10      3       12
 * 
 * cf. https://forums.raspberrypi.com/viewtopic.php?t=352841#p2115171
 * xrandr --newmode "1024x600_60.00"
 *                  49.00           1024    1072    1168    1312    600     603     613     624 -hsync +vsync
 *   --newmode <name> <clock MHz>
 *             <hdisp> <hsync-start> <hsync-end> <htotal>
 *             <vdisp> <vsync-start> <vsync-end> <vtotal>
 *             [flags...]
 *             Valid flags: +HSync -HSync +VSync -VSync
 *                          +CSync -CSync CSync Interlace DoubleScan
 * From ChatGPT, "VESA CVT Reduced Blanking (RB)":
 *      H_ACTIVE	    1024 pixels
 *      H_FRONT_PORCH	24 pixels
 *      H_SYNC	        136 pixels
 *      H_BACK_PORCH	160 pixels
 *      H_TOTAL	        1024 + 24 + 136 + 160 = 1344 pixels
 *      V_ACTIVE	    600 lines
 *      V_FRONT_PORCH	3 lines
 *      V_SYNC	        6 lines
 *      V_BACK_PORCH	26 lines
 *      V_TOTAL	        600 + 3 + 6 + 26 = 635 lines
 *      Pixel Clock	    48 MHz
 *      H_SYNC_POLARITY	+1 (active high)
 *      V_SYNC_POLARITY	+1 (active high)
 *      Refresh Rate	60 Hz (≈59,97 Hz exact)
 */
const scanvideo_timing_t scanvideo_timing_1024x600_ = {
    .clock_freq      = PICO_VGABOARD_1024X600_PIXEL_CLOCK_HZ,
    .h_active        = 1024,
    .v_active        = 600,
    .h_front_porch   = 24,
    .h_pulse         = 136,
    .h_total         = 1344,
    .h_sync_polarity = SCANVIDEO_POLARITY_POSITIVE,
    .v_front_porch   = 3,
    .v_pulse         = 6,
    .v_total         = 635,
    .v_sync_polarity = SCANVIDEO_POLARITY_POSITIVE,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_1024x600(__xscale__, __yscale__) {\
    .default_timing = &scanvideo_timing_1024x600_,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 1024 / (__xscale__),\
    .height         = 600 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_1024x600_11 = SCANVIDEO_MODE_1024x600(1, 1);
const scanvideo_mode_t pico_vga_mode_1024x300_12 = SCANVIDEO_MODE_1024x600(1, 2);
const scanvideo_mode_t pico_vga_mode_512x600_21  = SCANVIDEO_MODE_1024x600(2, 1);
const scanvideo_mode_t pico_vga_mode_512x300_22  = SCANVIDEO_MODE_1024x600(2, 2);
const scanvideo_mode_t pico_vga_mode_512x200_23  = SCANVIDEO_MODE_1024x600(2, 3);
const scanvideo_mode_t pico_vga_mode_512x150_24  = SCANVIDEO_MODE_1024x600(2, 4);
const scanvideo_mode_t pico_vga_mode_256x300_42  = SCANVIDEO_MODE_1024x600(4, 2);
const scanvideo_mode_t pico_vga_mode_256x200_43  = SCANVIDEO_MODE_1024x600(4, 3);
const scanvideo_mode_t pico_vga_mode_256x150_44  = SCANVIDEO_MODE_1024x600(4, 4);

#define PICO_VGABOARD_1024x600_60(__scanvideo_mode__) {\
    .scanvideo_mode = __scanvideo_mode__,\
    .freq_hz        = PICO_VGABOARD_1024X600_FREQ_HZ,\
    .sys_clock_khz  = PICO_VGABOARD_1024X600_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_1024X600_VREG_VOLTAGE,\
}

const pico_vgaboard_t pico_vgaboard_256x150_60 = PICO_VGABOARD_1024x600_60(&pico_vga_mode_256x150_44);
const pico_vgaboard_t pico_vgaboard_256x200_60 = PICO_VGABOARD_1024x600_60(&pico_vga_mode_256x200_43);
const pico_vgaboard_t pico_vgaboard_256x300_60 = PICO_VGABOARD_1024x600_60(&pico_vga_mode_256x300_42);
const pico_vgaboard_t pico_vgaboard_512x150_60 = PICO_VGABOARD_1024x600_60(&pico_vga_mode_512x150_24);
const pico_vgaboard_t pico_vgaboard_512x200_60 = PICO_VGABOARD_1024x600_60(&pico_vga_mode_512x200_23);
const pico_vgaboard_t pico_vgaboard_512x300_60 = PICO_VGABOARD_1024x600_60(&pico_vga_mode_512x300_22);
const pico_vgaboard_t pico_vgaboard_1024x300_60 = PICO_VGABOARD_1024x600_60(&pico_vga_mode_1024x300_12);
const pico_vgaboard_t pico_vgaboard_1024x600_60 = PICO_VGABOARD_1024x600_60(&pico_vga_mode_1024x600_11);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_1024X600_H */
