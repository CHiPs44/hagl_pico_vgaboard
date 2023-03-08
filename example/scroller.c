/* Scroller */

#include <wchar.h>
#include <string.h>
#include "hagl_hal.h"
#include "hagl.h"
#include "pico-vgaboard.h"

#define SCROLLER_DEBUG 0
// #define SCROLLER_DEBUG 1

typedef struct _scroller_t {
    /** @brief text */
    wchar_t  *text;
    /** @brief length of text */
    uint16_t length;
    /** @brief current index in text */
    uint16_t index;
    /** @brief top-left X */
    uint16_t x;
    /** @brief top-left Y */
    uint16_t y;
    /** @brief width */
    uint16_t w;
    /** @brief height */
    uint16_t h;
    /** @brief font */
    font_t   *font;
    /** @brief pixel (column) from 0 to font width - 1 in the current char */
    int8_t   pixel;
    /** @brief speed in bytes, *not* in pixels */
    uint8_t  speed_in_bytes;
    /** @brief animate every modulo frame */
    uint8_t  modulo;
    /** @brief text color */
    hagl_color_t  color;
    /** @brief delta y */
    int16_t dy;
    /** @brief y offset */
    int16_t y_offset;
} scroller_t;

scroller_t _scroller1;
scroller_t *s1 = &_scroller1;
scroller_t _scroller2;
scroller_t *s2 = &_scroller2;

hagl_color_t scroller_get_color()
{
    if (DEPTH == 1)
        return 1;
    if (DEPTH == 2)
        return 1 + (rand() % (COLORS - 1));
    if (DEPTH == 16)
        return PICO_SCANVIDEO_PIXEL_FROM_RGB8(
            0x80 + rand() % 0x80, 
            0x80 + rand() % 0x80, 
            0x80 + rand() % 0x80
        );
    // 16 & 256 colors
    hagl_color_t index;
    hagl_color_t color;
    uint8_t tries = 0;
    do {
        index = 1 + (rand() % COLORS - 1);
        hagl_color_t rgb = vgaboard_get_palette_color(index);
        uint8_t r = PICO_SCANVIDEO_R5_FROM_PIXEL(rgb) << 3;
        uint8_t g = PICO_SCANVIDEO_G5_FROM_PIXEL(rgb) << 3;
        uint8_t b = PICO_SCANVIDEO_B5_FROM_PIXEL(rgb) << 3;
        if (r >= 0x80 && g >= 0x80 && b >= 0x80)
            return index;
        tries += 1;
        if (tries > 10)
            return index;
    } while (1);
}

void scroller_init_one(scroller_t *s, int index)
{
    /* Parameters */
    /* Some text in english and french, somewhat in the tone of 80's demos */
    if (index == 1) {
        s->text   = 
            // L"0123456789012345678901234567890123456789";
            // L"                                        "
            L"Yo lamers!!!                            "
            L"This is CHiPs44 speaking through the awesome VGA demo board for the mighty Raspberry Pi Pico and the magnificent HAGL library... "
            L"Source code is available at https://github.com/CHiPs44/hagl_pico_vgaboard/ under MIT/BSD license...        "
            L"Thanks to Tuupola, Kilograham, Memotech Bill, DarkElvenAngel, HermannSW, lurk101, Rumbledethumps & Pimoroni's team!       "
            L"                                        ";
    } else {
        s->text   = 
            // L"0123456789012345678901234567890123456789";
            // L"                                        "
            L"Salut bande de nazes !!!                "
            L"C'est CHiPs44 qui parle depuis la super carte de démo VGA pour le génial Raspberry Pi Pico et la magnifique bibliothèque HAGL... "
            L"Le code source est disponible à l'URL https://github.com/CHiPs44/hagl_pico_vgaboard/ en licence MIT/BSD... "
            L"Merci à Tuupola, Kilograham, Memotech Bill, DarkElvenAngel, HermannSW, lurk101, Rumbledethumps et à l'équipe de Pimoroni !"
            L"                                        ";
    }
    s->length         = wcslen(s->text);
    s->font           = index == 1 ? &FONT8X13B: &FONT8X13B;
    s->x              = 0;
    s->y              = index == 1 ? HEIGHT *  1 / 4 : HEIGHT *  3 / 4;
    s->w              = WIDTH;
    s->h              = s->font->h;
    s->speed_in_bytes = 1;
    s->modulo         = index == 1 ? 0 : 2;
    s->dy             = index == 1 ? 1 : 0;
    /* Variables */
    s->index          = 0;
    s->pixel          = 0;
    s->color          = scroller_get_color();
    s->y_offset       = 0;
}

void scroller_draw_one(scroller_t *s)
{
    // Pixels per byte               2 colors       4 colors       16 colors      256 colors
    uint8_t pixels_per_byte = DEPTH==1 ? 8 : DEPTH==2 ? 4 : DEPTH==4 ? 2 : DEPTH==8 ? 1 : 0;
    if (pixels_per_byte==0) return;
    uint8_t speed_in_pixels = s->speed_in_bytes * pixels_per_byte;
    // Bytes per line
    uint16_t bytes_per_line = s->w / pixels_per_byte;
    uint16_t offset;
    uint8_t *destination;
    uint8_t *source;
    size_t  size;
    wchar_t text[40];
    wchar_t c = s->text[s->index];
#if SCROLLER_DEBUG
    hagl_put_text(
        hagl_backend, 
        L"0123456789012345678901234567890123456789", 
        0, 
        s->y - s->font->h * 2, 
        0xff, 
        s->font->fontx
    );
    // if (scroller->index > 60 && scroller->pixel==0) return;
#endif
    if (frame_counter % s->modulo == 0) {
        // Move text "speed_in_bytes" byte(s) left, 1 pixel in 8bpp, 2 pixels in 4bbp, and so on...
        for (uint16_t y = s->y; y < s->y + s->font->h; y += 1)
        {
            // Start of line in framebuffer
            destination = vgaboard->framebuffer + y * bytes_per_line;// + scroller->x * bytes_per_pixel;
            source      = destination + s->speed_in_bytes;
            // size = bytes_per_line - 1;
            // memcpy(destination, source, size);
            // Seems memcpy does not copy in the right direction...
            for(size_t byte = 0; byte < bytes_per_line - s->speed_in_bytes; byte += s->speed_in_bytes)
            {
                *destination++ = *source++;
            }
        }
#if SCROLLER_DEBUG
        swprintf(text, sizeof(text), 
            L"px/b: %d spdb: %d spdp: %d pix: %d  ", 
            pixels_per_byte, s->speed_in_bytes, speed_in_pixels, s->pixel
        );
        hagl_put_text(hagl_backend, text, 0, s->y - s->font->h, SWEETIE16_WHITE, s->font->fontx);
#endif
#if SCROLLER_DEBUG
        hagl_put_char(hagl_backend, c, 
            s->x + s->w - s->font->w, 
            s->y - s->font->h, 
            SWEETIE16_WHITE, s->font->fontx
        );
#endif
        // hagl_fill_rectangle_xyxy(
        //     hagl_backend, 
        //     scroller->x + scroller->w - 1 - speed_in_pixels, 
        //     scroller->y, 
        //     scroller->x + scroller->w - 1, 
        //     scroller->y + scroller->h - 1
        //     , COLORS - 1
        // );
        hagl_put_char(hagl_backend, c, 
            s->x + s->w - 2 - s->pixel, 
            s->y, // + s->font->h * s->pixel, 
            s->color, s->font->fontx
        );
        // Increment visible char area
        s->pixel += speed_in_pixels;
        if (s->pixel >= s->font->w - 1) {
            // Reset visible char area
            s->pixel = 0;
            // Next char
            s->index += 1;
            // Text wrap?
            if (s->index >= s->length) {
                s->index = 0;
            }
            // Change color between words
            if (s->text[s->index]==L' ') {
                s->color = scroller_get_color();
            }
        }
        // s1->y_offset += s1->dy;
        // if (s1->y_offset >= s1->font->height) {
        //     s1->dy += 1;
        //     if (s1->dy > )
        // }
    }
}

bool scroller_init()
{
    scroller_init_one(s1, 1);
    scroller_init_one(s2, 2);
    return true;
}

void scroller_draw()
{
    scroller_draw_one(s1);
    scroller_draw_one(s2);
}

// EOF
