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

## VGA modes status (2022-11-14)

For now, the library allocates a 64k static framebuffer.

### 640x400@70Hz - 32000 bytes framebuffer

Timings: <http://tinyvga.com/vga-timing/640x400@70Hz>

| Mode WxH@F | Depth   | Status  | Notes         |
| ---------- | :-----: | :-----: | ------------- |
| 640x400@70 | 1/2     | KO      | timing issues |
| 640x200@70 | 2/4     | KO      | timing issues |
| 320x200@70 | 4/16    | OK      | optimized     |
| 160x200@70 | 8/256   | KO      | timing issues |
| 160x100@70 | 16/32k  | OK?     | no demo yet   |

### 640x400@70Hz - 64000 bytes framebuffer

Timings: <http://tinyvga.com/vga-timing/640x400@70Hz>

| Mode WxH@F | Depth   | Status  | Notes         |
| ---------- | :-----: | :-----: | ------------- |
| 640x400@70 | 2/4     | KO      | timing issues |
| 640x200@70 | 4/16    | OK      | optimized     |
| 320x200@70 | 8/256   | KO      | timing issues |
| 160x200@70 | 16/32k  | OK?     | no demo yet   |

### 640x480@60Hz - 38400 bytes framebuffer

Timings: <http://tinyvga.com/vga-timing/640x480@60Hz>

| Mode WxH@F | Depth   | Status  | Notes         |
| ---------- | :-----: | :-----: | ------------- |
| 640x480@60 | 1/2     | KO      | timing issues |
| 640x240@60 | 2/4     | KO      | timing issues |
| 320x240@60 | 4/16    | OK      | optimized     |
| 160x240@60 | 8/256   | KO      | timing issues |
| 160x120@60 | 16/32k  | OK?     | no demo yet   |

### 1024x768@60Hz - 49152 bytes framebuffer

Timings:

- <https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712&start=50#p1864466> (from @kilograham)
- <http://tinyvga.com/vga-timing/1024x768@60Hz> (h-sync and v-sync polarities are negative)

| Mode WxH@F  | Depth   | Status  | Notes         |
| ----------- | :-----: | :-----: | ------------- |
| 1024x384@60 | 1/2     | KO      | timing issues |
| 512*768@60  | 1/2     | KO      | timing issues |
| 512x384@60  | 2/4     | KO      | timing issues |
| 512x192@60  | 4/16    | OK      | optimized     |
| 160x240@60 | 8/256   | KO      | timing issues |
| 160x120@60 | 16/32k  | OK?     | no demo yet   |

### 1280x1024@60Hz - 40960 bytes framebuffer

Timings:

- <http://tinyvga.com/vga-timing/1280x1024@60Hz>
- <http://martin.hinner.info/vga/timing.html>
- <https://www.ibm.com/docs/en/power8?topic=display-supported-resolution-timing-charts>
- <https://tomverbeure.github.io/video_timings_calculator>

Since it is the native resolution of my my 19" LG Flatron L1919S, I wanted that to work, but did not manage to have a stable mode at 4bpp yet.

I made many attempts, some where quite stable on an another old Dell LCD monitor, but are not at all on the LG with many timings.

It seems you have to divide the pixel clock by 2, perhaps it's the problem...

## License(s?)

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.

There's a mix with BSD 3 Clause that should be sorted out ASAP, but IANAL.

CHiPs44.

`EOF`
