# HAGL Pico VGA Board

|                      **BIG FAT WARNING**                      |
| :-----------------------------------------------------------: |
|                                                               |
|           **THIS PROJECT IS NOT PRODUCTION READY!**           |
|                                                               |
| **SOME VGA MODES IMPLY OVERCLOCKING AND OVERVOLTING RP2040!** |
|                   **USE AT YOUR OWN RISK!**                   |
|                                                               |

## Introduction

This is an HAGL HAL for the Raspberry Pi Pico demo VGA board, based on scanvideo from the pico-extras repository that comes alongside SDK.

This follows discussions on Raspberry Pi Pico's SDK forum [Understanding Pico VGA Code and CMakelists?](https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712) dating back from 2021.

The following projects are either bases for this one or sources of inspiration:

- [HAGL](https://github.com/tuupola/hagl)
- [Pico extras, scanvideo part](https://github.com/raspberrypi/pico-extras/tree/master/src/rp2_common/pico_scanvideo_dpi)
- [Pico playground, scanvideo part](https://github.com/raspberrypi/pico-playground/tree/master/scanvideo)
- [Pimoroni VGA Demo Base](https://shop.pimoroni.com/products/pimoroni-pico-vga-demo-base)
- [640x480x16 framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer)
- [HAGL GD](https://github.com/tuupola/hagl_gd)
- [MEMU - Memotech EMUlator](https://github.com/Memotech-Bill/MEMU)
- [PicoBB - BBC BASIC for Raspberry Pi Pico](https://github.com/Memotech-Bill/PicoBB)
- [Rumbledethumps' Picocomputer 6502](https://github.com/picocomputer/rp6502)

To manage this project, I use [Github's project board](https://github.com/users/CHiPs44/projects/1/views/2) to be able to organize myself between ideas, TODOs, WIPs & done items.

## Features

As of February 2024, this project has the following features:

- **Framebuffer**, statically allocated at compile time (see below for corruption using `malloc()`),
- **1/2/4/8/16 bits per pixel** modes, leading to 2/4/16/256/32768 colors at once,
- **8 bits or 16 bits mode** choosed at compile time, too,
- **Double buffering** for smooth animation at lower resolutions, as it uses twice the memory
- **VGA modes from 640x400 to 1280x1024**, with 1x/2x/4x/8x scale in X and Y directions (not using scanvideo's `yscale_denominator`)
- **Letterbox mode** to handle for example MSX / ZX Spectrum 256x192 inside 320x240 standard VGA
- **Handling of A/B/C buttons** (with clever? use in example allows going to next/reset demo, cycling through palettes & changing borders ramdomly)
- **Native execution** with [Pico Host SDL](`https://github.com/raspberrypi/pico-host-sdl`) using `PICO_NO_HARDWARE`, allowing to debug display algorithms before using real hardware
- **Serial console** on GPIO20/GPIO21 with cutted tracks

There is an [example](https://github.com/CHiPs44/hagl_pico_vgaboard_example) project with some demos.

## BUGS

- Find why allocating framebuffer dynamically with `malloc()` leads to corrupted display (memory alignment problems?)

## TODO

- VGA mode change at runtime, either:
  - only color depth and resolution (for example to implement modes 0=160x200/16, 1=320x200/4, 2=640x200/2 of Amstrad CPC)
  - other VGA modes, only being limited by the amount of statically allocated memory
- Using multiplane capabilities of scanvideo:
  - Framebuffer with its own palette
  - Text mode layer, using another palette if desired
  - Tilemap and tileset (with pixel scrolling, window or full screen,...)
  - Sprites, as many as possible, eventually with scaling / rotation
  - only 3 will be available at the same time
- Optimizations:
  - Blitting operations either in C or assembly to speed up rendering and capabilities
  - Packing of HAGL's bitmaps when in 1/2/4 bits per pixel modes, or implement them as VGA board only
- Replacement of buttons with Pico Host SDL target

## Build Instructions

CMakeLists.txt is inspired by:

- [Memotech Bill's Pico VGA framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer)
- [Tuupola's HAGL Pico MIPI](https://github.com/tuupola/hagl_pico_mipi)

[pico-sdk](https://github.com/raspberrypi/pico-sdk) and [pico-extras](https://github.com/raspberrypi/pico-extras) from Raspberry Pi Foundation are obviously required.

## VGA modes

Most VGA timings come from:

- [VGA Signal Timing](http://tinyvga.com/vga-timing)
- Pico Extras itself

N.B.:

- Most modes are intended to be stretched by 2 (or more) in horizontal and/or vertical directions to reduce memory usage
- Wider modes have higher pixel clocks that limit system overclocking coefficient to 3, 2 or even 1, that's why I'm trying to provide "halved" versions which are tricky to achieve
- For `h_sync_polarity` and `v_sync_polarity`, `1` means NEGATIVE, and `0` means POSITIVE

### Working modes

These modes should work on any reasonable LCD display, I don't own any CRT monitor anymore so I can't make any tests.

|   A/R |  VGA Mode |    PC^1 |    PC^2 | Notes                                                                          |
| ----: | --------: | ------: | ------: | ------------------------------------------------------------------------------ |
| 16:10 |   640x400 |  25.175 |  25.200 | Only mode using 70Hz refresh rate, all other use something very near of 60 Hz  |
|   4:3 |   640x480 |  25.175 |  25.200 | Most standard, if only one can work, that must be this one                     |
|   4:3 |   768x576 |  34.960 |  35.000 | Divisible by 3, fast system clock (280 MHz at 1.20V^3 on my Pico B0 & B1)      |
|   4:3 |   800x600 |  40.000 |  40.000 | I think 768x756 is better ;-)                                                  |
|   4:3 |  1024x768 |  65.000 |  65.000 | Even my 15" LCD from 2001 or 2002 has this as native resolution!               |
|  16:9 |  1280x720 |  74.250 |  74.000 | Only 16:9 mode for now                                                         |
|   5:4 | 1280x1024 | 108.000 | 108.000 | I own an 19" LCD with this native resolution, too...                           |
| 16:10 | 1680x1050 | 147.140 | 147.000 | Divisible by 3 or 5^4, fastest system clock (294 MHz at 1.30V^3 on my Pico B1) |

- ^[1] Specifications Pixel Clock in MHz
- ^[2] Rounded Pixel Clock in MHz as achievable by RPi Pico (as given by `vcocalc` tool)
- ^[3] **Be aware that these values may either not work on specific Picos, shorten their lifespan or even destroy them!**, see `ALLOW_VREG_VOLTAGE_OVERRIDE`
- ^[4] That Makes a very good fit at 336x210 with 320x200 letterbox, very tight margins at native resolution on my LG L204WT monitor ;-)

### Experimental modes

These modes are flaky on my own monitors or even do not work at all.

|   A/R | VGA Mode |    PC^1 |    PC^2 | Notes                                                                              |
| ----: | -------: | ------: | ------: | ---------------------------------------------------------------------------------- |
|   5:4 |  640x512 |  56.000 |  56.000 | Half of 1280x1024, rounding v_pulse to 1 or 2 does not seem good...                |
| 16:10 |  840x525 |  73.125 |  73.500 | Half of 1680x1050, should satisfy my LG L204WT monitor but is "out of range"       |
|  16:9 | 1024x576 |  46.500 |  46.500 |                                                                                    |
|  16:9 | 1600x900 | 108.000 | 108.000 | Native resolution of my 17" laptop: good for fullscreen tests, KO on my LG monitor |
| 16:10 | 1280x800 |  83.460 |  83.200 | Double of 640x400, can be used to make 640x400@60                                  |

## License

The MIT License (MIT).

See [LICENSE](LICENSE) for more information.

There's a mix with BSD 3 Clause license between HAGL and Pico SDK / extras that should be fine, but IANAL.

CHiPs44.

`EOF`
