#include <stdio.h>

/*
    Launch as
        gcc make-rgb332.c && ./a.out && rm a.out
    then copy/paste into palettes.c
*/

int main(void)
{
    int _r[] = {0x00, 0x1f, 0x3f, 0x5f, 0x7f, 0x9f, 0xbf, 0xff};
    int _g[] = {0x00, 0x1f, 0x3f, 0x5f, 0x7f, 0x9f, 0xbf, 0xff};
    int _b[] = {0x00, 0x55, 0xaa, 0xff};
    int i = 0;
    /* 3 bits for red */
    for (int r = 0; r < 8; r++)
    {
        /* 3 bits for green */
        for (int g = 0; g < 8; g++)
        {
            /* 2 bits for blue */
            for (int b = 0; b < 4; b++)
            {
                printf(
                    "    /* %03d */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x%02x, 0x%02x, 0x%02x),\n",
                    i, _r[r], _g[g], _b[b]);
                i += 1;
            }
        }
    }
    return 0;
}
