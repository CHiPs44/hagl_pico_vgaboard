/* SPDX-License-Identifier: MIT-0 */

int16_t bars[15];
int16_t dirs[15];
hagl_color_t cols[15];

bool bars_init()
{
    for (uint8_t bar = 0; bar < 15; bar++)
    {
        bars[bar] = window.x + get_rand_32() % (window.w);
        dirs[bar] = (get_rand_32() % 2 == 0 ? 1 : -1) * (1 + get_rand_32() % 8);
        switch (DEPTH)
        {
        case 1:
        case 2:
        case 4:
            cols[bar] = 1 + bar % (COLORS  - 1);
            break;
        case 8:
            cols[bar] = 1 + get_rand_32() % (COLORS  - 1);
            break;
        case 16:
            cols[bar] = PICO_SCANVIDEO_PIXEL_FROM_RGB5(get_rand_32() % 0x20, get_rand_32() % 0x20, get_rand_32() % 0x20);
            break;        
        default:
            break;
        }
    }
    return true;
}

void bars_draw()
{
    uint16_t x, y, w, h;
    uint16_t x1, y1;

    for (uint8_t bar = 0; bar < 15; bar++)
    {
        x = window.x;
        h = window.h / 15 - 1;
        y = window.y + (h + 1) * bar;
        bars[bar] += dirs[bar];
        if (bars[bar] < window.x)
        {
            dirs[bar] = 1 + get_rand_32() % 8;
            bars[bar] = window.x - dirs[bar];
        }
        else
        {
            if (bars[bar] > window.x + window.w)
            {
                dirs[bar] = -(1 + get_rand_32() % 8);
                bars[bar] = window.x + window.w + dirs[bar];
            }
        }
        w = bars[bar];
        hagl_fill_rectangle_xywh(hagl_backend, x    , y, w           , h, cols[bar]);
        hagl_fill_rectangle_xywh(hagl_backend, x + w, y, window.w - w, h, 0        );
    }
}

// EOF
