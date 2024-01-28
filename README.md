# hagl_pico_vgaboard

|              BIG FAT WARNING              |
| :---------------------------------------: |
| This project is **NOT** production ready! |

This is an HAGL HAL for Raspberry Pi Pico VGA board, based on scanvideo from pico-extras.

This follows discussions on Raspberrry Pi forum [Understanding Pico VGA Code and CMakelists?](https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712), including:

- [Pico extras, scanvideo part](https://github.com/raspberrypi/pico-extras/tree/master/src/rp2_common/pico_scanvideo_dpi)
- [Pico playground, scanvideo part](https://github.com/raspberrypi/pico-playground/tree/master/scanvideo)
- [Pimoroni VGA Demo Base](https://shop.pimoroni.com/products/pimoroni-pico-vga-demo-base)
- [640x480x16 framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer)
- [HAGL](https://github.com/tuupola/hagl)
- [HAGL GD](https://github.com/tuupola/hagl_gd)

To follow up things, I currently try to use [Github's project board](https://github.com/users/CHiPs44/projects/1/views/2) to be able to sort them between ideas, TODOs, WIPs & done items.

## Features

- Statically allocated framebuffer,
- 8 bits or 16 bits mode (choose at compile time),
- 1/2/4/8 bits per pixel modes, leading to 2/4/16/256 colors at once
- Many VGA modes, with 1x/2x/4x/8x scale in X and Y (no use of scanvideo's `yscale_denominator`)
- Letterbox to handle for example ZX Spectrum 256x192 with borders inside 320x240 standard VGA
- Handling of A/B/C buttons (clever? use in example allowing to go next/reset demo, palette & borders)

## WIP

- Text mode layer using multi plane capabilities of scanvideo

## TODO

- Find why allocating framebuffer with `malloc()` leads to corrupted display
- Allow to change VGA mode at runtime, either:
  - only color depth and resolution (for example to implement mode 0/1/2 of Amstrad CPC)
  - other VGA modes (without restarting)
- Manage blitting operations to speed up functions below
- Handle tilemap and tileset (with pixel scrolling, window or full screen,...)
- Sprites, eventually with scaling / rotation

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

| Status | Mode (WxH@F) | Notes             |
| ------ | ------------ | ----------------- |
|        | 640x400@70   |                   |
|        | 640x480@60   |                   |
|        | 640x512@60   | Half of 1280x1024 |
|        | 768x576@60   |                   |
|        | 800x600@60   |                   |
|        | 1024x768@60  |                   |
|        | 1280x720     |                   |
|        | 1280x1024    |                   |

## License

The MIT License (MIT).

See [LICENSE](LICENSE) for more information.

There's a mix with BSD 3 Clause that should be fine, but IANAL.

CHiPs44.

`EOF`
