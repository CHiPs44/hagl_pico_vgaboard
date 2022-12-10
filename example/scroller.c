/* Scroller */

#include <wchar.h>
#include <string.h>
// HAGL
#include "hagl_hal.h"
#include "hagl.h"

#define SCROLLER_DEBUG 0
// #define SCROLLER_DEBUG 1

typedef struct {
    wchar_t  *text;
    uint16_t index;
    uint16_t length;
    uint16_t x;
    uint16_t y;
    font_t   *font;
    int8_t   pixel;
    uint8_t  speed;
    uint8_t  modulo;
    color_t  color;
} scroller_t;

scroller_t _scroller;
scroller_t *scroller = &_scroller;

#include "pico-vgaboard.h"

color_t scroller_get_color()
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
    color_t index;
    color_t color;
    uint8_t tries = 0;
    do {
        index = 1 + (rand() % COLORS - 1);
        color_t rgb = vgaboard_get_palette_color(index);
        uint8_t r = PICO_SCANVIDEO_R5_FROM_PIXEL(rgb) << 3;
        uint8_t g = PICO_SCANVIDEO_G5_FROM_PIXEL(rgb) << 3;
        uint8_t b = PICO_SCANVIDEO_B5_FROM_PIXEL(rgb) << 3;
        if (r >= 0x80 && g >= 0x80 && b >= 0x80)
            return index;
        tries += 1;
        if (tries > 10)
            return 1 + (rand() % (COLORS - 1));
    } while (1);
}

void scroller_init(scroller_t *scroller)
{
    /* Parameters */
    /* Some text in english and french, somewhat in the tone from 80's demos */
    scroller->text = 
        L"                                        "
        L"Yo lamers!!!                            "
        L"Salut bande de nazes !!!                "
        L"This is CHiPs44 speaking through the awesome VGA demo board for the mighty Raspberry Pi Pico and the magnificent HAGL library..."
        L"C'est CHiPs44 qui parle depuis la super carte de démo VGA pour le génial Raspberry Pi Pico et la magnifique bibliothèque HAGL..."
        L"Source code is available at https://github.com/CHiPs44/hagl_pico_vgaboard/ under MIT/BSD license..."
        L"Le code source est disponible à l'URL https://github.com/CHiPs44/hagl_pico_vgaboard/ en licence MIT/BSD..."
        L"Hi to / Salut à Tuupola, Kilograham, Memotech Bill, DarkElvenAngel, HermannSW, lurk101, Rumbledethumps & Pimoroni's staff!"
        L"                                        ";
    scroller->font   = &FONT8X13B;//FONT8X8;
    scroller->x      = WIDTH  - 1;
    scroller->y      = HEIGHT - scroller->font->h;
    /* Variables */
    scroller->length = wcslen(scroller->text);
    scroller->index  = 0;
    scroller->pixel  = 0;
    scroller->color  = scroller_get_color();
    scroller->speed  = 1;
    scroller->modulo = 1;
#if SCROLLER_DEBUG
    hagl_put_text(
        hagl_backend, 
        L"0123456789012345678901234567890123456789", 
        0, 
        scroller->y, 
        0xff, 
        scroller->font
    );
#endif
}

void scroller_draw(scroller_t *scroller)
{
    // Pixels per byte
    //                               2 colors       4 colors       16 colors      256 colors
    uint8_t pixels_per_byte = DEPTH==1 ? 8 : DEPTH==2 ? 4 : DEPTH==4 ? 2 : DEPTH==8 ? 1 : 0;
    if (pixels_per_byte==0) return;
    // Bytes per line
    uint16_t bytes_per_line = WIDTH / pixels_per_byte;
    uint16_t offset;
    uint8_t *destination;
    uint8_t *source;
    size_t  size;
#if SCROLLER_DEBUG
    hagl_put_text(
        hagl_backend, 
        L"0123456789012345678901234567890123456789", 
        0, 
        HEIGHT / 2, 
        0xff, 
        scroller->font
    );
    // if (scroller->index > 60 && scroller->pixel==0) return;
#endif
    if (frame_counter % scroller->modulo == 0) {
        // Move text "speed" byte(s) left, 1 pixel in 8bpp, 2 pixels in 4bbp and so on...
        for (uint16_t y = scroller->y; y < scroller->y + scroller->font->h; y += 1)
        {
            // Offset of line from beginning of framebuffer
            offset = y * bytes_per_line;
            // Start
            destination = FRAMEBUFFER + offset;
            // source      = FRAMEBUFFER + offset;
            // size        = bytes_per_line / 2; // - scroller->speed;
            // memcpy(destination, source, size);
            // Seems memcpy does not copy in the right direction...
            for (uint16_t byte = scroller->speed; byte < bytes_per_line/* - scroller->font->w / pixels_per_byte*/ - scroller->pixel / pixels_per_byte; byte += 1)
            {
                destination[byte - scroller->speed] = destination[byte];
            }
        }
#if SCROLLER_DEBUG
        hagl_put_char(
            hagl_backend, 
            scroller->text[scroller->index], 
            scroller->x - scroller->font->w, 
            scroller->y - scroller->font->h, 
            COLORS - 1, 
            scroller->font
        );
#endif
        hagl_put_char(
            hagl_backend, 
            scroller->text[scroller->index], 
            scroller->x - scroller->pixel, 
            scroller->y, 
            scroller->color, 
            scroller->font->fontx
        );
        // Increment visible char area
        scroller->pixel += scroller->speed;
        if (scroller->pixel > scroller->font->w) {
            // Reset visible char area
            scroller->pixel = 0;
            // Next char
            scroller->index += 1;
            // Wrap?
            if (scroller->index >= scroller->length) {
                scroller->index = 0;
            }
            // Change color between words
            if (scroller->text[scroller->index]==L' ') {
                scroller->color = scroller_get_color();
            }
        }
    }
}

// EOF
