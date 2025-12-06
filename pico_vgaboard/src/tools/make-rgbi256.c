#include <stdio.h>

/*
    Launch as
        gcc make-rgbi256.c && ./a.out && rm a.out
    then copy/paste into palettes.c
*/

int main(void)
{
    /*                           0          96         160         224 */
    /*                           0           3           5           7 */
    /*                  ----------  ----------  ----------  ---------- */
    const int msb[4] = {0b00000000, 0b01100000, 0b10100000, 0b11100000};
    /*                           0           8          16          24 */
    /*                  ----------  ----------  ----------  ---------- */
    const int lsb[4] = {0b00000000, 0b00001000, 0b00010000, 0b00011000};
    int c = 0;
    int r, g, b;
    for (int _r = 0; _r < 4; _r++)
    {
        for (int _g = 0; _g < 4; _g++)
        {
            for (int _b = 0; _b < 4; _b++)
            {
                for (int _i = 0; _i < 4; _i++)
                {
                    r = msb[_r] | lsb[_i];
                    g = msb[_g] | lsb[_i];
                    b = msb[_b] | lsb[_i];
                    printf(
                        "    /* %03d */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x%02x, 0x%02x, 0x%02x),\n", 
                        c++, r, g, b
                    );
                }
            }
        }
    }
    return 0;
}
