# hagl_pico_vgaboard

HAGL HAL for Raspberry Pi Pico VGA board (scanvideo).

This follows discussions on Raspberrry Pi forum [Understanding Pico VGA Code and CMakelists?](https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712), including:

- [Pico playground, scanvideo part](https://github.com/raspberrypi/pico-playground)
- [Pimoroni VGA Demo Base](https://shop.pimoroni.com/products/pimoroni-pico-vga-demo-base)
- [640x480x16 framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer)
- [HAGL](https://github.com/tuupola/hagl)

## Build Instructions

CMakeLists.txt is inspired by [https://github.com/Memotech-Bill/pico-vga-framebuffer](https://github.com/Memotech-Bill/pico-vga-framebuffer) and by [https://github.com/tuupola/hagl_pico_mipi](https://github.com/tuupola/hagl_pico_mipi).

BEWARE! Project is not even functional at the moment!

You require pico-sdk and pico-extras from Raspberry Pi.

Then from the folder containing these:

```shell
git clone https://github.com/CHiPs44/hagl_pico_vgaboard.git
cd hagl_pico_vgaboard
mkdir build
cd build
cmake ..
make
```

## License(s?)

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.
