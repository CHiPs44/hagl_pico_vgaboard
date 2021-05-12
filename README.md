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

## License(s?)

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.

There's a mix with BSD 3 Clause that should be sorted out ASAP, but IANAL.

CHiPs44.

`EOF`
