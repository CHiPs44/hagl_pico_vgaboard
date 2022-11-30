/* Scroller */

#include <wchar.h>
#include <string.h>

typedef struct {
    wchar_t *text;
    uint16_t index;
    uint16_t length;
    uint16_t x;
    uint16_t y;
    uint8_t *font;
    uint8_t font_w;
    uint8_t font_h;
    int8_t pixel;
    color_t color;
} scroller_t;

scroller_t _scroller;
scroller_t *scroller = &_scroller;

// const wchar_t *text = 
// uint16_t scroller->index;
// uint16_t scroller->length;
// uint16_t scroller->x;
// uint16_t scroller->y;
// const uint8_t *scroller->font  = font8x13B;//font8x8_fnt;
// const uint8_t scroller->font_w = 8;
// const uint8_t scroller->font_h = 13;//8;
// int8_t scroller->pixel;
// color_t scroller->color;

#include "pico-vgaboard.h"

color_t scroller_get_color()
{
    if (DEPTH <= 2)
        return 1 + (rand() % (COLORS - 1));
    if (DEPTH==16)
        return PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80 + rand() % 0x80, 0x80 + rand() % 0x80, 0x80 + rand() % 0x80); 
    color_t index;
    color_t color;
    uint8_t tries = 0;
    do {
        index = 1 + (rand() % COLORS - 1);
        color_t rgab5515 = vgaboard_get_palette_color(index);
        uint8_t r = PICO_SCANVIDEO_R5_FROM_PIXEL(rgab5515) << 3;
        uint8_t g = PICO_SCANVIDEO_G5_FROM_PIXEL(rgab5515) << 3;
        uint8_t b = PICO_SCANVIDEO_B5_FROM_PIXEL(rgab5515) << 3;
        if (r >= 0x1f / 2 && g >= 0x1f / 2 && b >= 0x1f / 2)
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
        //1234567890123456789012345678901234567890
        L"0123456789 0123456789 0123456789 0123456789"
        L"0123456789 0123456789 0123456789 0123456789";
        // L"                                        "
        // L"Yo lamers!!!"
        // L"                                        "
        // L"Salut bande de nazes !!!"
        // L"                                        "
        // L"This is CHiPs44 speaking through the awesome VGA demo board for the mighty Raspberry Pi Pico and the magnificent HAGL library..."
        // L"                                        "
        // L"C'est CHiPs44 qui déblatère depuis la super carte de démo VGA pour le génial Raspberry Pi Pico et la magnifique bibliothèque HAGL..."
        // L"                                        "
        // L"Source code is available at https://github.com/CHiPs44/hagl_pico_vgaboard/ under MIT/BSD license..."
        // L"                                        "
        // L"Le code source est disponible à l'URL https://github.com/CHiPs44/hagl_pico_vgaboard/ en licence MIT/BSD..."
        // L"                                        "
        // L"Hi to / Salut à Tuupola, Kilograham, Memotech Bill, DarkElvenAngel, HermannSW, Rumbledethumps & Pimoroni's staff!"
        // L"                                        ";
    scroller->font   = font8x13B;//font8x8_fnt;
    scroller->font_w = 8;
    scroller->font_h = 13;//8;
    scroller->x      = WIDTH  - 1;
    scroller->y      = HEIGHT / 2 - scroller->font_h;
    /* Variables */
    scroller->length = wcslen(scroller->text);
    scroller->index  = 0;
    scroller->pixel  = 0;
    scroller->color  = scroller_get_color();
    hagl_put_text(
        hagl_backend, 
        L"0123456789012345678901234567890123456789", 
        0, 
        scroller->y, 
        0xff, 
        scroller->font
    );
}

void scroller_draw(scroller_t *scroller)
{
    // Pixels per byte
    //            2 colors       4 colors       16 colors      256 colors
    uint8_t pixels_per_byte = DEPTH==1 ? 8 : DEPTH==2 ? 4 : DEPTH==4 ? 2 : DEPTH==8 ? 1 : 0;
    if (pixels_per_byte==0) return;
    // Bytes per line
    uint16_t bytes_per_line = WIDTH / pixels_per_byte;
    uint8_t speed = 1;
    uint16_t offset;
    uint8_t *destination;
    uint8_t *source;
    size_t  size;

    hagl_put_text(
        hagl_backend, 
        L"0123456789012345678901234567890123456789", 
        0, 
        HEIGHT / 2, 
        0xff, 
        scroller->font
    );
    // if (scroller->index > 60 && scroller->pixel==0) return;

    if (counter % 1 == 0) {
        // Move text "speed" byte(s) left, 1 pixel in 8bpp, 2 pixels in 4bbp and so on...
        for (uint16_t y = scroller->y; y < scroller->y + scroller->font_h; y += 1)
        {
            // Offset of line from beginning of framebuffer
            offset = y * bytes_per_line;
            // Start
            destination = FRAMEBUFFER + offset;
            // source      = FRAMEBUFFER + offset;
            // size        = bytes_per_line / 2; // - speed;
            // memcpy(destination, source, size);
            // Seems memcpy does not copy in the right direction...
            for (uint16_t byte = speed; byte < bytes_per_line/* - scroller->font_w / pixels_per_byte*/ - scroller->pixel / pixels_per_byte; byte += 1)
            {
                destination[byte - speed] = destination[byte];
            }
        }
        hagl_put_char(
            hagl_backend, 
            scroller->text[scroller->index], 
            scroller->x - scroller->font_w, 
            scroller->y - scroller->font_h, 
            COLORS - 1, 
            scroller->font
        );
        hagl_put_char(
            hagl_backend, 
            scroller->text[scroller->index], 
            scroller->x - scroller->pixel, 
            scroller->y, 
            scroller->color, 
            scroller->font
        );
        // Increment visible char area
        scroller->pixel += speed;
        if (scroller->pixel > scroller->font_w) {
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
