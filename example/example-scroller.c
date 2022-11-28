/* Scroller */
const wchar_t *scroller_text = 
    //1234567890123456789012345678901234567890
    // L"                                        "
    L"Yo lamers!!! "
    L"This is CHiPs44 speaking through the awesome VGA demo board for the mighty Raspberry Pi Pico and the magnificent HAGL library... "
    L"Hi to Tuupola, Kilograham, Memotech Bill, DarkElvenAngel, HermannSW & Rumbledethumps!"
    L"                                        ";
uint16_t scroller_index;
uint16_t scroller_length;
uint16_t scroller_x;
uint16_t scroller_y;
const uint8_t *scroller_font  = font8x8_fnt;
const uint8_t scroller_font_w = 8;
const uint8_t scroller_font_h = 8;
int8_t scroller_pixel;

void init_scroller()
{
    scroller_length = wcslen(scroller_text);
    scroller_index = 0;
    scroller_x = WIDTH  -  8;
    scroller_y = HEIGHT / 2 - scroller_font_h;
    scroller_pixel = scroller_font_w;
}

#include <wchar.h>
#include <string.h>

void draw_scroller()
{
    uint8_t pbb = DEPTH==1 ? 8 : DEPTH==2 ? 4 : DEPTH==4 ? 2 : DEPTH==8 ? 1 : 0;
    if (pbb==0)
        return;
    uint8_t speed = 1;
    uint16_t bytes;
    uint16_t offset;
    uint8_t *destination;
    uint8_t *source;
    size_t  size;

    // if (counter % 10 == 0) {
        for (uint16_t y = scroller_y; y < scroller_y + scroller_font_h; y += 1)
        {
            bytes = WIDTH / pbb;
            offset = y * bytes;
            destination = FRAMEBUFFER + offset;
            // source      = FRAMEBUFFER + offset + speed;
            // size        = bytes / 2; // - speed;
            // memcpy(destination, source, size);
            for (uint16_t x = speed; x < WIDTH / pbb; x += 1)
            {
                destination[x - speed] = destination[x];
            }
        }
        hagl_put_char(
            hagl_backend, 
            scroller_text[scroller_index], 
            scroller_x + scroller_pixel, 
            scroller_y, 
            1 + (rand() % COLORS - 1), 
            scroller_font
        );
        scroller_pixel -= speed;
        if (scroller_pixel < 0) {
            scroller_pixel = scroller_font_w;
            scroller_index += 1;
            // Wrap?
            if (scroller_index > scroller_length) {
                scroller_index = 0;
            }
        }
    // }
}

// EOF
