# VGA TYPE 8X8 FONTS

## Introduction

FontX2 fonts as used by HAGL are a little too much for the Pico, as they handle Unicode and such need to work with wide chars and strings, which aren't needed to recreate the feeling of an 8 bits micro from the 80s.

Also, there isn't any 8x8 font, and many don't work at all at the moment.

Without messing with PETSCII or BBC micro charsets, an old and well-known source of 8x8 fonts is VGA BIOS dumps, in CP-437 or CP-850 encodings.

The best source i found seems to be <https://github.com/viler-int10h/vga-text-mode-fonts/>.

Licenses seems unclear: is it public domain, free for personal use, or non free?

The project distributes other fonts with different heights indicated by the file name extension.

For example, `.F14` stands for an 14 pixels height font, as used in text modes of the time.

## How to make the "C" header file for a specific font

Get file (256 8x8 chars / 2048 bytes), for example IBM's BIOS one:

```bash
wget <https://github.com/viler-int10h/vga-text-mode-fonts/raw/master/FONTS/PC-IBM/BIOS.F08>
```

Convert to C header file:

```bash
xxd -i -c8 BIOS.F08 > BIOS_F08.h
```

## "API" ideas

Contrary to HAGL's philosophy, use of "static" configuration will be used, e.g. not passing backend, background, foreground & font to each call, but setting them in advance.

There should be an option to set transparency for background, and another to experiment with modes like AND, OR, XOR.

We should handle when x and y are a factor of 8 differently, as it could be interpreted as text mode stuff.

```c
static hagl_backend_t *vgafont8_hagl;
static uint8_t *vgafont8_font;
static hagl_color_t vgafont8_background_color;
static hagl_color_t vgafont8_foreground_color;

// Configuration
void vgafont8_set_hagl(hagl_backend_t *hagl)
void vgafont8_set_font_bitmap(uint8_t *font)
void vgafont8_set_background_color(hagl_color_t background)
void vgafont8_set_foreground_color(hagl_color_t foreground)

// Display single char or string
void vgafont8_put_char(uint16_t x, uint16_t y, uint8_t c)
void vgafont8_put_text(uint16_t x, uint16_t y, uint8_t *s)

// Retrieve single char or text string an an HAGL bitmap
void vgafont8_get_char(uint8_t c, hagl_bitmap *bitmap)
void vgafont8_get_text(uint8_t *s, hagl_bitmap *bitmap)
```
