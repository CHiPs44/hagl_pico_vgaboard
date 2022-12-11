/* SPDX-License-Identifier: MIT-0 */

rect_t *bars_window;
int16_t bars[15];
int16_t dirs[15];
color_t cols[15];

void bars_init(rect_t *window)
{
    bars_window->x = window->x;
    bars_window->y = window->y;
    bars_window->w = window->w;
    bars_window->h = window->h;

    for (uint8_t bar = 0; bar < 15; bar++)
    {
        bars[bar] = bars_window->x + rand() % (bars_window->w);
        dirs[bar] = (rand() % 2 == 0 ? 1 : -1) * (1 + rand() % 8);
        cols[bar] = 1 + bar % (COLORS  - 1);
    }
}

void bars_draw()
{
    uint16_t x, y, w, h;
    uint16_t x0, y0, w0, h0, x1, y1;//, x2, y2;

    x0 = bars_window->x;
    y0 = bars_window->y;
    w0 = bars_window->w;
    h0 = bars_window->h;// - scroller->font->h;
    hagl_set_clip(hagl_backend, x0, y0, x0 + w0 -1, y0 + h0 - 1);
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
