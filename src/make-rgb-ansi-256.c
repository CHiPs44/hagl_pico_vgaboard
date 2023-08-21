#include <stdio.h>

/* 
    cf. https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit 

    Launch as 
        gcc make-rgb-ansi-256.c && ./a.out && rm a.out 
    then copy/paste into pico-vgaboard-palettes.c
*/

int main(void)
{
    //   0-  7: standard colors (as in ESC [ 30–37 m)
    printf(
        "    /* 000 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00),\n"
        "    /* 001 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x00),\n"
        "    /* 002 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x00),\n"
        "    /* 003 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0x00),\n"
        "    /* 004 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x80),\n"
        "    /* 005 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x80),\n"
        "    /* 006 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x80),\n"
        "    /* 007 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xc0, 0xc0, 0xc0),\n"
    );
    //   8- 15: high intensity colors (as in ESC [ 90–97 m)
    printf(
        "    /* 008 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0x80),\n"
        "    /* 009 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x00),\n"
        "    /* 010 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00),\n"
        "    /* 011 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x00),\n"
        "    /* 012 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xff),\n"
        "    /* 013 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0xff),\n"
        "    /* 014 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0xff),\n"
        "    /* 015 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff),\n"
    );
    //  16-231: 6 × 6 × 6 cube (216 colors): 16 + 36 × r + 6 × g + b (0 ≤ r, g, b ≤ 5)
    int c6[] = { 0x00, 0x5f, 0x87, 0xaf, 0xd7, 0xff };
    int i = 16;
    for (int r = 0; r < 6; r++)
    {
        for (int g = 0; g < 6; g++)
        {
            for (int b = 0; b < 6; b++)
            {
                printf(
                    "    /* %03d */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x%02x, 0x%02x, 0x%02x),\n", 
                            i,                                     c6[r] , c6[g] , c6[b]
                );
                i += 1;
            }
        }
    }
    // 232-255: grayscale from dark to light in 24 steps,
    //          even if we only have 14 beside black and white...
    int c24[] = {
        0x08, 0x12, 0x1c, 0x26, 0x30, 0x3a, 0x44, 0x4e, 0x58, 0x62, 0x6c, 0x76, 
        0x80, 0x8a, 0x94, 0x9e, 0xa8, 0xb2, 0xbc, 0xc6, 0xd0, 0xda, 0xe4, 0xee
    };
    for (i = 0; i < 24; i++)
    {
        printf(
            "    /* %03d */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x%02x, 0x%02x, 0x%02x),\n", 
                    232 + i,                               c24[i], c24[i], c24[i]
        );
    }

    return 0;
}
