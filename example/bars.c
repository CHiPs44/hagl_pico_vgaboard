/* SPDX-License-Identifier: MIT-0 */

int16_t bars[15];
int16_t dirs[15];
color_t cols[15];

void bars_init()
{
    for (uint8_t bar = 0; bar < 15; bar++)
    {
        bars[bar] = window.x + rand() % (window.w);
        dirs[bar] = (rand() % 2 == 0 ? 1 : -1) * (1 + rand() % 8);
        switch (DEPTH)
        {
        case 1:
        case 2:
        case 4:
            cols[bar] = 1 + bar % (COLORS  - 1);
            break;
        case 8:
            cols[bar] = 1 + rand() % (COLORS  - 1);
            break;
        case 16:
            cols[bar] = PICO_SCANVIDEO_PIXEL_FROM_RGB5(rand() % 0x20, rand() % 0x20, rand() % 0x20);
            break;        
        default:
            break;
        }
    }
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
            dirs[bar] = 1 + rand() % 8;
            bars[bar] = window.x - dirs[bar];
        }
        else
        {
            if (bars[bar] > window.x + window.w)
            {
                dirs[bar] = -(1 + rand() % 8);
                bars[bar] = window.x + window.w + dirs[bar];
            }
        }
        w = bars[bar];
        hagl_fill_rectangle_xywh(hagl_backend, x    , y, w           , h, cols[bar]);
        hagl_fill_rectangle_xywh(hagl_backend, x + w, y, window.w - w, h, 0        );
    }
}

// EOF
