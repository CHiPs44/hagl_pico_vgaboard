/* SPDX-License-Identifier: MIT-0 */

int dblbuf_x[16];
int dblbuf_y[16];
int dblbuf_w[16];
int dblbuf_h[16];
int dblbuf_dx[16];
int dblbuf_dy[16];

bool dblbuf_init()
{
    for (int i = 0; i < 16; i++)
    {
        dblbuf_x[i] = DEMO.x + rand() % DEMO.w;
        dblbuf_y[i] = DEMO.y + rand() % DEMO.h;
        dblbuf_w[i] = DEMO.w / 8 + rand() % (DEMO.w / 8);
        dblbuf_h[i] = DEMO.h / 8 + rand() % (DEMO.h / 8);
        dblbuf_dx[i] = rand() % 16 > 8 ? 1 : -1;
        dblbuf_dy[i] = rand() % 16 > 8 ? 1 : -1;
    }
    return true;
}

void dblbuf_draw()
{
    for (int i = 0; i < 16; i++)
    {
        hagl_fill_rounded_rectangle_xywh(hagl_backend, dblbuf_x[i], dblbuf_y[i], dblbuf_w[i], dblbuf_h[i], 4, 15 - i);
        dblbuf_x[i] += dblbuf_dx[i];
        // Left out?
        if (dblbuf_x[i] < DEMO.x)
        {
            dblbuf_x[i] = DEMO.x;
            dblbuf_dx[i] = -dblbuf_dx[i];
        }
        // Right out?
        if (dblbuf_x[i] > DEMO.w - dblbuf_w[i])
        {
            dblbuf_x[i] = DEMO.x + DEMO.w - dblbuf_w[i];
            dblbuf_dx[i] = -dblbuf_dx[i];
        }
        // Top out?
        dblbuf_y[i] += dblbuf_dy[i];
        if (dblbuf_y[i] < DEMO.y)
        {
            dblbuf_y[i] = DEMO.y;
            dblbuf_dy[i] = -dblbuf_dy[i];
        }
        // Bottom out?
        if (dblbuf_y[i] > DEMO.y + DEMO.h - dblbuf_h[i])
        {
            dblbuf_y[i] = DEMO.y + DEMO.h - dblbuf_h[i];
            dblbuf_dy[i] = -dblbuf_dy[i];
        }
    }
}

void dblbuf_done()
{
    // Nothing!
}
