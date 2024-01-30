# HAGL Pico VGA Board

|              BIG FAT WARNING              |
| :---------------------------------------: |
| This project is **NOT** production ready! |

## Introduction

This is an HAGL HAL for the Raspberry Pi Pico demo VGA board, based on scanvideo from the pico-extras repository that comes alongside SDK.

This follows discussions on Raspberrry Pi forum [Understanding Pico VGA Code and CMakelists?](https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712), including:

- [Pico extras, scanvideo part](https://github.com/raspberrypi/pico-extras/tree/master/src/rp2_common/pico_scanvideo_dpi)
- [Pico playground, scanvideo part](https://github.com/raspberrypi/pico-playground/tree/master/scanvideo)
- [Pimoroni VGA Demo Base](https://shop.pimoroni.com/products/pimoroni-pico-vga-demo-base)
- [640x480x16 framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer)
- [HAGL](https://github.com/tuupola/hagl)
- [HAGL GD](https://github.com/tuupola/hagl_gd)

To manage this project, I currently try to use [Github's project board](https://github.com/users/CHiPs44/projects/1/views/2) to be able to organize myself between ideas, TODOs, WIPs & done items.

## Features

As of january 2024, this project has the following features:

- **Framebuffer**, statically allocated at compile time (see below for corruption using `malloc()`),
- **1/2/4/8/16 bits per pixel** modes, leading to 2/4/16/256/32768 colors at once
- **8 bits or 16 bits mode** choosed at compile time, too,
- **VGA modes from 640x400 to 1280x1024**, with 1x/2x/4x/8x scale in X and Y directions (not using of scanvideo's `yscale_denominator`)
- **Letterbox** to handle for example ZX Spectrum 256x192 with borders inside 320x240 standard VGA
- **Handling of A/B/C buttons** (with clever? use in example allowing to go next/reset demo, palette & borders)
- **Native compilation** with [Pico host SDL](`https://github.com/raspberrypi/pico-host-sdl`) via use of `PICO_NO_HARDWARE`

There is an `example` directory with some demos:

- **Specifications**: mainly text demo with Pico's & VGA properties
- **Palettes**: grid display of current palette
- **Scroller**: 3 line scrollers from right to left with text from specifications and english (top) and french (bottom) texts
- **Images**: 3 pictures (cat, cow & dog) slideshow (16 and 256 colors)
- **HAGL**:
  - **Hollow figures**: random hollow graphic primitives: rectangles, ellipses & so on
  - **Filled figures**: same with filled primitives
  - **Bars**: sliding bars
  - **Fonts**: sample text for every HAGL font (from 5x7 to 8x13) and 8x8 BIOS type one

## WIP

- Sprite demo that is not release grade yet
- Text mode layer using multi plane capabilities of scanvideo, using another palette if desired

## TODO

- Find why allocating framebuffer with `malloc()` leads to corrupted display (memory alignment problem?)
- Allow to change VGA mode at runtime, either:
  - only color depth and resolution (for example to implement mode 0/1/2 of Amstrad CPC)
  - other VGA modes
- Manage blitting operations to speed up functions below
- Handle tilemap and tileset (with pixel scrolling, window or full screen,...)
- Sprites, as many as possible, eventually with scaling / rotation

## Build Instructions

CMakeLists.txt is inspired by:

- [https://github.com/Memotech-Bill/pico-vga-framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer)
- [https://github.com/tuupola/hagl_pico_mipi](https://github.com/tuupola/hagl_pico_mipi)

[pico-sdk](https://github.com/raspberrypi/pico-sdk) and [pico-extras](https://github.com/raspberrypi/pico-extras) from Raspberry Pi Foundation are required.

To start with the project, do:

```shell
git clone https://github.com/CHiPs44/hagl_pico_vgaboard.git
cd hagl_pico_vgaboard
git submodule init
cd example
mkdir build
cd build
cmake ..
make
```

## VGA modes

Most VGA timings come either from:

- Pico SDK itself
- [VGA Signal Timing](http://tinyvga.com/vga-timing)

| Status       |  A/R  | Mode (WxH) | F   | Notes             |
| ------------ | :---: | ---------- | --- | ----------------- |
| OK           |       | 640x400    | 70  |                   |
| OK           |  4:3  | 640x480    | 60  |                   |
| Experimental |  5:4  | 640x512    | 60  | Half of 1280x1024 |
| OK           |  4:3  | 768x576    | 60  |                   |
| OK           |  4:3  | 800x600    | 60  |                   |
| Experimental |       | 1024x576   | 60  |                   |
| OK           |  4:3  | 1024x768   | 60  |                   |
|              |       | 1280x720   |     |                   |
| Experimental |       | 1280x800   |     |                   |
|              |  5:4  | 1280x1024  |     |                   |
| Experimental | 16:10 | 1680x1050  |     |                   |

## License

The MIT License (MIT).

See [LICENSE](LICENSE) for more information.

There's a mix with BSD 3 Clause that should be fine, but IANAL.

CHiPs44.

`EOF`
