/* SPDX-License-Identifier: MIT-0 */

int dblbuf_x[16];
int dblbuf_y[16];
int dblbuf_dx[16];
int dblbuf_dy[16];
int dblbuf_w[16];
int dblbuf_h[16];

#if PICO_VGABOARD_DOUBLE_BUFFER == 0
void pico_vgaboard_framebuffer_flip()
{
    wait_for_vblank();
}
#endif

bool dblbuf_init()
{
    uint8_t *framebuffer;
    framebuffer = (uint8_t *)(pico_vgaboard->framebuffer);
    memset(framebuffer, 0, pico_vgaboard->framebuffer_size);
    pico_vgaboard_framebuffer_flip();
    framebuffer = (uint8_t *)(pico_vgaboard->framebuffer);
    memset(framebuffer, 0, pico_vgaboard->framebuffer_size);
    pico_vgaboard_framebuffer_flip();
    for (int i = 0; i < 16; i++)
    {
        dblbuf_x[i] = rand() % WIDTH;
        dblbuf_y[i] = rand() % HEIGHT;
        dblbuf_dx[i] = rand() % 16 > 8 ? 1 : -1;
        dblbuf_dy[i] = rand() % 16 > 8 ? 1 : -1;
        dblbuf_w[i] = 16 + rand() % (WIDTH / 8);
        dblbuf_h[i] = 16 + rand() % (HEIGHT / 8);
    }
    return true;
}

void dblbuf_draw()
{
    uint8_t *framebuffer;
    framebuffer = (uint8_t *)(pico_vgaboard->framebuffer);
    memset(framebuffer, 0, pico_vgaboard->framebuffer_size);
    for (int i = 0; i < 16; i++)
    {
        /* clang-format off */
        hagl_fill_rounded_rectangle_xywh(hagl_backend, dblbuf_x[i], dblbuf_y[i], dblbuf_w[i], dblbuf_h[i], 3, 15 - i);
        dblbuf_x[i] += dblbuf_dx[i];
        if (dblbuf_x[i] < 0                   ) { dblbuf_x[i] = 0                   ; dblbuf_dx[i] = -dblbuf_dx[i]; }
        if (dblbuf_x[i] > WIDTH  - dblbuf_w[i]) { dblbuf_x[i] = WIDTH  - dblbuf_w[i]; dblbuf_dx[i] = -dblbuf_dx[i]; }
        dblbuf_y[i] += dblbuf_dy[i];
        if (dblbuf_y[i] < 0                   ) { dblbuf_y[i] = 0                   ; dblbuf_dy[i] = -dblbuf_dy[i]; }
        if (dblbuf_y[i] > HEIGHT - dblbuf_h[i]) { dblbuf_y[i] = HEIGHT - dblbuf_h[i]; dblbuf_dy[i] = -dblbuf_dy[i]; }
        /* clang-format on */
    }
}

void dblbuf_done()
{
    uint8_t *framebuffer;
    framebuffer = (uint8_t *)(pico_vgaboard->framebuffer);
    memset(framebuffer, 0, pico_vgaboard->framebuffer_size);
    pico_vgaboard_framebuffer_flip();
    framebuffer = (uint8_t *)(pico_vgaboard->framebuffer);
    memset(framebuffer, 0, pico_vgaboard->framebuffer_size);
    pico_vgaboard_framebuffer_flip();
}
