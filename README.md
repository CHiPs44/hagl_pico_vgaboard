# HAGL Pico VGA Board

|            **BIG FAT WARNING**            |
| :---------------------------------------: |
|                                           |
| **THIS PROJECT IS NOT PRODUCTION READY!** |
|                                           |

## Introduction

This is an HAGL HAL for the Raspberry Pi Pico demo VGA board, based on scanvideo from the pico-extras repository that comes alongside SDK, see [HAGL](https://github.com/tuupola/hagl).

This follows discussions on Raspberry Pi Pico's SDK forum [Understanding Pico VGA Code and CMakelists?](https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712) dating back from 2021.

The following projects are either bases for this one or sources of inspiration:

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
- **1/2/4/8/16 bits per pixel** modes, leading to 2/4/16/256/32768 colors at once
- **8 bits or 16 bits mode** choosed at compile time, too,
- **VGA modes from 640x400 to 1280x1024**, with 1x/2x/4x/8x scale in X and Y directions (not using scanvideo's `yscale_denominator`)
- **Letterbox mode** to handle for example ZX Spectrum 256x192 with borders inside 320x240 standard VGA
- **Handling of A/B/C buttons** (with clever? use in example allowing to go next/reset demo, cycling through palettes & changing borders ramdomly)
- **Native compilation** with [Pico host SDL](`https://github.com/raspberrypi/pico-host-sdl`) via use of `PICO_NO_HARDWARE`, allowing to quickly test display before using real hardware

There is an `example` directory with some demos:

- **Specifications**: mainly text demo with Pico's & VGA properties
- **Palettes**: grid display of current palette
- **Scroller**: 3 line scrollers from right to left with text from specifications, english and french texts somewhat inspired by 80's demos
- **Images**: 3 pictures (cat, cow & dog) slideshow (16 and 256 colors)
- **Hollow figures**: random hollow graphic primitives: rectangles, ellipses & so on
- **Filled figures**: same with filled primitives
- **Bars**: sliding bars at different speeds
- **Fonts**: sample text for every HAGL font (from 5x7 to 8x13) and 8x8 BIOS type one

## WIP

- Double buffering for smooth animation at lower resolutions, as it uses twice the memory
- Sprite demo that is not release grade yet, but got better in conjunction with double buffering

## BUGS

- Find why allocating framebuffer with `malloc()` leads to corrupted display (memory alignment problem?)

## TODO

- Allow to change VGA mode at runtime, either:
  - only color depth and resolution (for example to implement modes 0=160x200/1=320x200/2=640x200 of Amstrad CPC)
  - other VGA modes
- Text mode layer using multi plane capabilities of scanvideo, using another palette if desired
- Handle tilemap and tileset (with pixel scrolling, window or full screen,...)
- Sprites, as many as possible, eventually with scaling / rotation
- Optimize blitting operations to speed up rendering and capabilities

## Build Instructions

CMakeLists.txt is inspired by:

- [https://github.com/Memotech-Bill/pico-vga-framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer)
- [https://github.com/tuupola/hagl_pico_mipi](https://github.com/tuupola/hagl_pico_mipi)

[pico-sdk](https://github.com/raspberrypi/pico-sdk) and [pico-extras](https://github.com/raspberrypi/pico-extras) from Raspberry Pi Foundation are obviously required.

To start with the project, do:

```shell
git clone https://github.com/CHiPs44/hagl_pico_vgaboard.git
cd hagl_pico_vgaboard
git submodule init
cd example
mkdir build
cd build
cmake ..
# or
cmake .. -D CMAKE_BUILD_TYPE=RelWithDebInfo
make -j$(nproc)
# Put your Pico in USB mode with bootsel and reset button of VGA board
cp -pv hagl_pico_vgaboard_example.uf2 /media/chips/RPI-RP2/
```

If you use a Picoprobe or any SWD device, you should know how to upload ELF file to your Pico, or try to create/adapt your `launch.json` from <https://chips44.github.io/20231230-PicoDebugProbeHowto.html> to use OpenOCD & GDB. `[F5]` will then be your best friend!

## VGA modes

Most VGA timings come from:

- [VGA Signal Timing](http://tinyvga.com/vga-timing)
- Pico Extras itself

N.B.:

- Most modes are intended to be stretched by 2 (or more) in horizontal and/or vertical directions to reduce memory usage
- Wider modes have higher pixel clocks that limit system overclocking coefficient to 3, 2 or even 1, that's why I'm trying to provide "halved" versions which are tricky to achieve

### Working modes

These modes should work on any reasonable LCD display, I don't own any CRT monitor anymore so I can't make any tests.

|   A/R |  VGA Mode |     PC¹ |     PC² | Notes                                                                   |
| ----: | --------: | ------: | ------: | ----------------------------------------------------------------------- |
| 16:10 |   640x400 |  25.175 |  25.200 | Only mode using 70Hz refresh rate, all other use 60 Hz                  |
|   4:3 |   640x480 |  25.175 |  25.200 | Most standard, if only one can work, that must be this one              |
|   4:3 |   768x576 |  34.960 |  35.000 | Divisible by 3, fast system clock (280 MHz at 1.20V on my Pico B0 & B1) |
|   4:3 |   800x600 |  40.000 |  40.000 |                                                                         |
|   4:3 |  1024x768 |  65.000 |  65.000 |                                                                         |
|  16:9 |  1280x720 |  74.250 |  74.000 |                                                                         |
|   5:4 | 1280x1024 | 108.000 | 108.000 |                                                                         |
| 16:10 | 1680x1050 | 147.140 | 147.000 | Divisible by 3, fastest system clock (294 MHz at 1.30V on my Pico B1)   |

- PC¹ : Specified Pixel Clock in MHz
- PC² : Rounded Pixel Clock in MHz as achievable by RPi Pico (given by vcocalc tool)

### Experimental modes

These modes may not work with specific monitors, be flaky on my own ones or even do not work at all.

|   A/R | VGA Mode |    PC¹ | PC² | Notes                                                                        |
| ----: | -------: | -----: | --: | ---------------------------------------------------------------------------- |
|   5:4 |  640x512 | 56.000 |     | Half of 1280x1024, rounding v_pulse to 1 or 2 does not seem good...          |
| 16:10 |  840x525 | 73.125 |     | Half of 1680x1050, should satisfy my LG L204WT monitor but is "out of range" |
|  16:9 | 1024x576 |  46.50 |     |                                                                              |
| 16:10 | 1280x800 | 83.460 |     | Double of 640x400, can be used to make 640x400@60                            |

## License

The MIT License (MIT).

See [LICENSE](LICENSE) for more information.

There's a mix with BSD 3 Clause license that should be fine, but IANAL.

Example uses MIT No Attribution, see [LICENSE](example/LICENSE).

CHiPs44.

`EOF`
