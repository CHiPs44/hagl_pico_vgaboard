# EXAMPLE

## TODO

## "IBM" 8X8 FONT

256 8x8 chars / 2048 bytes :

```bash
wget <https://github.com/viler-int10h/vga-text-mode-fonts/blob/master/FONTS/PC-IBM/BIOS.F08?raw=true>
```

Convert to C header file:

```bash
xxd -i -c8 BIOS.F08 > BIOS_F08.h
```

```c
void bitmap_font_set_font(void *self, uint8_t *font, uint8_t height)
void bitmap_font_set_paper(void *self, color_t paper)
void bitmap_font_set_ink(void *self, color_t ink)
void bitmap_font_put_char(void *self, uint16_t x0, uint16_t y0, uint8_t c)
void bitmap_font_put_text(void *self, uint16_t x0, uint16_t y0, uint8_t *s)
```
