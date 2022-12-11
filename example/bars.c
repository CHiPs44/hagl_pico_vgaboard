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
        cols[bar] = 1 + bar % (COLORS  - 1);
    }
}

void bars_draw()
{
    uint16_t x, y, w, h;
    uint16_t x0, y0, w0, h0, x1, y1;//, x2, y2;

    x0 = window.x;
    y0 = window.y;
    w0 = window.w;
    h0 = window.h;// - scroller->font->h;
    for (uint8_t bar = 0; bar < 15; bar++)
    {
        x = x0;
        h = h0 / 15 - 1;
        y = y0 + (h + 1) * bar;
        bars[bar] += dirs[bar];
        if (bars[bar] < x0)
        {
            dirs[bar] = 1 + rand() % 8;
            bars[bar] = x0 - dirs[bar];
        }
        else
        {
            if (bars[bar] > x0 + w0)
            {
                dirs[bar] = -(1 + rand() % 8);
                bars[bar] = x0 + w0 + dirs[bar];
            }
        }
        w = bars[bar];
        hagl_fill_rectangle_xywh(hagl_backend, x    , y, w     , h, cols[bar]);
        hagl_fill_rectangle_xywh(hagl_backend, x + w, y, w0 - w, h, 0        );
    }
}

// EOF
