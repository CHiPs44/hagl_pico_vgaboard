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
    scroller_y = 0;//HEIGHT / 2 - scroller_font_h;
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
    uint8_t *destination;
    uint8_t *source;
    size_t  size = WIDTH / pbb - /*scroller_font_w +*/ speed;

    if (counter % 15 == 0) {
        for (uint8_t y = scroller_y + 0; y < scroller_y + scroller_font_h; y++)
        {
            destination = FRAMEBUFFER + y * (WIDTH / pbb);
            source      = destination + speed;
            memcpy(destination, source, size);
        }
        hagl_put_char(hagl_backend, scroller_text[scroller_index], scroller_x + scroller_pixel, scroller_y, COLORS - 1, scroller_font);
        scroller_pixel -= speed;
        if (scroller_pixel < 0) {
            scroller_pixel = scroller_font_w;
            scroller_index += 1;
            // Wrap?
            if (scroller_index > scroller_length) {
                scroller_index = 0;
            }
        }
    }
}