# EXAMPLE

## TODO

## "IBM" TYPE 8X8 FONTS

Get file (256 8x8 chars / 2048 bytes):

```bash
wget <https://github.com/viler-int10h/vga-text-mode-fonts/raw/master/FONTS/PC-IBM/BIOS.F08>
```

Convert to C header file:

```bash
xxd -i -c8 BIOS.F08 > BIOS_F08.h
```

"API" ideas:

```c
hagl_backend_t *vgafont8_hagl;
void vgafont8_set_hagl(hagl_backend_t *hagl)
void vgafont8_set_font(uint8_t *font)
void vgafont8_set_background_color(color_t background)
void vgafont8_set_foreground_color(color_t foreground)
void vgafont8_put_char(uint16_t x0, uint16_t y0, uint8_t c)
void vgafont8_put_text(uint16_t x0, uint16_t y0, uint8_t *s)
void vgafont8_get_char(hagl_bitmap *bitmap, uint8_t c)
void vgafont8_get_text(hagl_bitmap *bitmap, uint8_t *s)

```
