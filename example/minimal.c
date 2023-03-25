#include <memory.h>

bool minimal_init()
{
    // Upper half: color stripes writing directly to framebuffer
    // assume 4bpp => 2 pixels per byte
    size_t real_fb_size = vgaboard->width * vgaboard->height / 2;
    for(int c = 0; c < 16; c += 1) {
        size_t offset = c * real_fb_size / 16 / 2;
        size_t bytes = real_fb_size / 16 / 2;
        uint8_t byte = c * 16 + c;
        uint8_t *address = vgaboard->framebuffer + offset;
        // printf(
        //     "framebuffer=%p address=%p offset=%04x bytes=%04x byte=%02x\r\n", 
        //     vgaboard->framebuffer, address, offset, bytes, byte
        // );
        memset(address, byte,  bytes);
    }
    // Lower half: color stripes using HAGL
    // hagl_clear(hagl_backend);
    for (int i = 0; i < 16; i += 1) {
        // scanvideo_wait_for_vblank();
        hagl_fill_rectangle_xywh(
            hagl_backend, 
            0, vgaboard->height / 2 + (vgaboard->height / 2 / 16) * i, 
            vgaboard->width, vgaboard->height / 16 / 2, 
            i
        );
    }
    // wprintf(L"%p %c %d %d %d %p\r\n", hagl_backend, L'A', 16, 16, 12, font8x13B);
    // hagl_put_char(hagl_backend, L'A', 16, 16, 12, font8x13B);
    return true;
}

void minimal_draw()
{
    int x0, y0, x1, y1;
    hagl_color_t c0, c1;
    for (int i = 0; i < 10; i += 1) {
        x0 = get_rand_32() % vgaboard->width;
        y0 = get_rand_32() % vgaboard->height;
        c0 = hagl_get_pixel(hagl_backend, x0, y0);
        x1 = get_rand_32() % vgaboard->width;
        y1 = get_rand_32() % vgaboard->height;
        c1 = hagl_get_pixel(hagl_backend, x1, y1);
        hagl_put_pixel(hagl_backend, x0, y0, c1);
        hagl_put_pixel(hagl_backend, x1, y1, c0);
    }
}

void minimal_loop()
{
    int counter = 0;
    printf("counter=%d\r\n", counter / 100);
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

