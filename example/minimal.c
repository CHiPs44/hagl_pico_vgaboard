/* SPDX-License-Identifier: MIT-0 */

#include <memory.h>

bool minimal_init()
{
    if (DEPTH != 4) {
        hagl_put_text(hagl_backend, L"4BPP MODE ONLY!", 0, 0, 1, font8x8);
        return true;
    }
    // 4bpp => 16 colors, 2 pixels per byte
    // Upper half: color stripes writing directly to framebuffer
    size_t real_fb_size = WIDTH * HEIGHT / 2;
    for(int c = 0; c < COLORS; c += 1) {
        size_t offset = c * real_fb_size / 16 / 2;
        size_t bytes = real_fb_size / 16 / 2;
        uint8_t byte = c * 16 + c;
        uint8_t *address = pico_vgaboard->framebuffer + offset;
        memset(address, byte,  bytes);
    }
    // Lower half: color stripes using HAGL
    for (int c = 0; c < COLORS; c += 1) {
        hagl_fill_rectangle_xywh(
            hagl_backend, 
            0, HEIGHT / 2 + (HEIGHT / 2 / 16) * c, 
            WIDTH, HEIGHT / 16 / 2, 
            c
        );
    }
    return true;
}

void minimal_draw()
{
    int x0, y0, x1, y1;
    hagl_color_t c0, c1;
    for (int i = 0; i < 10; i += 1) {
        x0 = rand() % WIDTH;
        y0 = rand() % HEIGHT;
        c0 = hagl_get_pixel(hagl_backend, x0, y0);
        x1 = rand() % WIDTH;
        y1 = rand() % HEIGHT;
        c1 = hagl_get_pixel(hagl_backend, x1, y1);
        hagl_put_pixel(hagl_backend, x0, y0, c1);
        hagl_put_pixel(hagl_backend, x1, y1, c0);
    }
}

void minimal_loop()
{
    int counter = 0;
    while (true) {
        // tight_loop_contents();
        scanvideo_wait_for_vblank();
        minimal_draw();
        counter += 1;
        if (counter % 1000 == 0) {
            printf("counter=%d\r\n", counter / 1000);
        }
    }
}
