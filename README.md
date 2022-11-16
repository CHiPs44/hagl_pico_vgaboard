# hagl_pico_vgaboard

This is an HAGL HAL for Raspberry Pi Pico VGA board (based on scanvideo).

This follows discussions on Raspberrry Pi forum [Understanding Pico VGA Code and CMakelists?](https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712), including:

- [Pico playground, scanvideo part](https://github.com/raspberrypi/pico-playground)
- [Pimoroni VGA Demo Base](https://shop.pimoroni.com/products/pimoroni-pico-vga-demo-base)
- [640x480x16 framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer)
- [HAGL](https://github.com/tuupola/hagl)
- [HAGL GD](https://github.com/tuupola/hagl_gd)

## Build Instructions

CMakeLists.txt is inspired by:

- [https://github.com/Memotech-Bill/pico-vga-framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer)
- [https://github.com/tuupola/hagl_pico_mipi](https://github.com/tuupola/hagl_pico_mipi)

[pico-sdk](https://github.com/raspberrypi/pico-sdk) and [pico-extras](https://github.com/raspberrypi/pico-extras) from Raspberry Pi Foundation are required.

To start with the project, do:

```shell
git clone https://github.com/CHiPs44/hagl_pico_vgaboard.git
cd hagl_pico_vgaboard
cd example
mkdir build
cd build
cmake ..
make
```

## VGA modes status (2022-11-16)

For now:

- the library allocates a 64k static framebuffer, and modes are selected to not go beyond that limit
- 4bpp modes are very well optimized trough the use of an ARM assembly function and a "double" palette
- 1bpp, 2bpp, 8bpp depths don't work with timings issues as they aren't optimized as 4bpp depth is
- 16bpp depth had just one seemingly succesful test

### Working modes

| Base WxH@F   | X | Y | Mode     | Depth   | Fbuff |
| -----------: | - | - | :------: | :-----: | :---: |
|   640x400@70 | 2 | 2 | 320x200  | 4/16    | 32000 |
|   640x480@60 | 2 | 2 | 320x240  | 4/16    | 38400 |
|  1024x768@60 | 2 | 2 | 512x192  | 4/16    | 49152 |
|   640x400@70 | 1 | 2 | 640x200  | 4/16    | 64000 |

### NON Working modes

| Base WxH@F   | X | Y | Mode     | Depth   | Fbuff |
| -----------: | - | - | :------: | :-----: | :---: |
| 1280x1024@60 | 2 | 2 | 320x256  | 4/16    | 40960 |

### Timings & notes

#### 640x400@70Hz

Timings: <http://tinyvga.com/vga-timing/640x400@70Hz>

#### 640x480@60Hz

Timings: <http://tinyvga.com/vga-timing/640x480@60Hz>

### 1024x768@60Hz - 49152 bytes framebuffer

Timings:

- <https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712&start=50#p1864466> (from @kilograham)
- <http://tinyvga.com/vga-timing/1024x768@60Hz> (h-sync and v-sync polarities are negative)

### 1280x1024@60Hz

Timings:

- <http://tinyvga.com/vga-timing/1280x1024@60Hz>
- <http://martin.hinner.info/vga/timing.html>
- <https://www.ibm.com/docs/en/power8?topic=display-supported-resolution-timing-charts>
- <https://tomverbeure.github.io/video_timings_calculator>

Since it is the native resolution of my my 19" LG Flatron L1919S, I wanted that to work, but did not manage to have a stable mode yet.

I made many attempts, some where quite stable on an another old Dell LCD monitor, but are not at all on the LG.

It seems you have to divide the pixel clock by 2 and xscale, too, perhaps it's the problem...

## License(s?)

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.

There's a mix with BSD 3 Clause that should be sorted out ASAP, but IANAL.

CHiPs44.

`EOF`
