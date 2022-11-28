/* Scroller */

/* @brief Some text in english and french, somewhat in the tone from 80's demos */
const wchar_t *scroller_text = 
    //1234567890123456789012345678901234567890
    // L"0123456789012345678901234567890123456789"
    // L"0123456789012345678901234567890123456789";
    L"EN: Yo lamers!!!"
    L"                                        "
    L"FR : Salut bande de nazes !"
    L"                                        "
    L"EN: This is CHiPs44 speaking through the awesome VGA demo board for the mighty Raspberry Pi Pico and the magnificent HAGL library..."
    L"                                        "
    L"FR : C'est CHiPs44 qui déblatère depuis la super carte de démo VGA pour le flamboyant Raspberry Pi Pico et la magnifique bibliothèque HAGL..."
    L"                                        "
    L"EN: Source code is available at https://github.com/CHiPs44/hagl_pico_vgaboard/ under MIT/BSD license..."
    L"                                        "
    L"FR : Le code source est disponible à l'URL https://github.com/CHiPs44/hagl_pico_vgaboard/ en licence MIT/BSD..."
    L"                                        "
    L"Hi to / Merci à Tuupola, Kilograham, Memotech Bill, DarkElvenAngel, HermannSW, Rumbledethumps & Pimoroni's staff!"
    L"                                        ";
uint16_t scroller_index;
uint16_t scroller_length;
uint16_t scroller_x;
uint16_t scroller_y;
const uint8_t *scroller_font  = font8x8_fnt;
const uint8_t scroller_font_w = 8;
const uint8_t scroller_font_h = 8;
int8_t scroller_pixel;
color_t scroller_color;

#include "pico-vgaboard.h"

color_t get_scroller_color()
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
        color_t rgab5515 = vgaboard_get_palette_color(vgaboard_get_palette_color(index));
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

void init_scroller()
{
    scroller_length = wcslen(scroller_text);
    scroller_index = 0;
    scroller_x = WIDTH  - 1;
    scroller_y = HEIGHT / 2 - scroller_font_h;
    scroller_pixel = 0;
    scroller_color = get_scroller_color();
}

#include <wchar.h>
#include <string.h>

void draw_scroller()
{
    uint8_t pbb = DEPTH==1 ? 8 : DEPTH==2 ? 4 : DEPTH==4 ? 2 : DEPTH==8 ? 1 : 0;
    if (pbb==0) return;
    uint8_t speed = 1;
    uint16_t bytes;
    uint16_t offset;
    uint8_t *destination;
    uint8_t *source;
    size_t  size;

    // hagl_put_text(
    //     hagl_backend, 
    //     L"0123456789012345678901234567890123456789", 
    //     0, 
    //     HEIGHT / 2, 
    //     0xff, 
    //     scroller_font
    // );
    // if (scroller_index > 60 && scroller_pixel==0) return;

    // if (counter % 10 == 0) {
        for (uint16_t y = scroller_y; y < scroller_y + scroller_font_h; y += 1)
        {
            // Bytes per line
            bytes = WIDTH / pbb;
            // Offset of line from beginning of framebuffer
            offset = y * bytes;
            // Start
            destination = FRAMEBUFFER + offset;
            // source      = FRAMEBUFFER + offset;
            // size        = bytes / 2; // - speed;
            // memcpy(destination, source, size);
            // Seems memcpy does not copy in the right direction...
            for (uint16_t x = speed; x < WIDTH / pbb; x += 1)
            {
                destination[x - speed] = destination[x];
            }
        }
        // hagl_put_char(
        //     hagl_backend, 
        //     scroller_text[scroller_index], 
        //     scroller_x - scroller_font_w, 
        //     scroller_y - scroller_font_h, 
        //     scroller_color, 
        //     scroller_font
        // );
        hagl_put_char(
            hagl_backend, 
            scroller_text[scroller_index], 
            scroller_x - scroller_pixel, 
            scroller_y, 
            scroller_color, 
            scroller_font
        );
        // Increment visible char area
        scroller_pixel += speed;
        if (scroller_pixel >= scroller_font_w) {
            // Reset visible char area
            scroller_pixel = 0;
            // Next char
            scroller_index += 1;
            // Wrap?
            if (scroller_index >= scroller_length) {
                scroller_index = 0;
            }
            // Change color between words
            if (scroller_text[scroller_index]==L' ') {
                scroller_color = get_scroller_color();
            }
        }
    // }
}

// EOF
