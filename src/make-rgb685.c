#include <stdio.h>

/* cf. https://en.wikipedia.org/wiki/List_of_software_palettes#6-8-5_levels_RGB */

/* Launch as gcc make-rgb685.c && ./a.out && rm a.out then copy/paste into pico-vgaboard-palettes.c */

int main(void)
{
    int _r[] = { 0x00, 0x33, 0x66, 0x99, 0xcc, 0xff };
    int _g[] = { 0x00, 0x24, 0x49, 0x6d, 0x92, 0xb6, 0xdb, 0xff };
    int _b[] = { 0x00, 0x40, 0x80, 0xbf, 0xff };
    // 240 rgb685 colors
    for (int r = 0; r < 6; r++)
    {
        for (int g = 0; g < 8; g++)
        {
            for (int b = 0; b < 5; b++)
            {
                int i = 40 * r + 5 * g + b;
                printf(
                    "    /* %03d */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x%02x, 0x%02x, 0x%02x),\n", 
                    i, _r[r], _g[g], _b[b]
                );
            }
        }
    }
    // 16 levels of gray/grey
    for (int i = 0; i < 16; i++)
    {
        printf(
            "    /* %03d */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x%02x, 0x%02x, 0x%02x),\n", 
            240 + i, 2 * i, 2 * i, 2 * i
        );
    }

    return 0;
}