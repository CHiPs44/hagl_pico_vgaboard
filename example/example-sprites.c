// #include "sprites-4bpp.h"
// /* Draw tiles */
// x = WIDTH / 2;
// for (uint8_t column = 0; column < 2 /*WIDTH / 2 / 8*/; column += 1)
// {
//     for (uint8_t line = 0; line < 2 /*(HEIGHT / 2 - y) / 8*/; line += 1)
//     {
//         // hagl_blit_xy(hagl_backend, x + column * 8, y + line * 8, &tile_8x8x4_1);
//         hagl_blit_xywh(hagl_backend, x + column * 8, y + line * 8, 8 * 4, 8 * 4, &tile_8x8x4_1);
//     }
// }
// uint8_t buffer[8 * 13 * sizeof(color_t)];
// hagl_bitmap_t bitmap;
// bitmap.size = sizeof(buffer);
// bitmap.buffer = buffer;
// uint8_t glyph = hagl_get_glyph(hagl_backend, L'W', 13, &bitmap, font8x13B);
// printf("glyph   %d\n", glyph);
// printf("WIDTH   %d\n", bitmap.WIDTH);
// printf("HEIGHT  %d\n", bitmap.HEIGHT);
// printf("depth   %d\n", bitmap.depth);
// printf("pitch   %d\n", bitmap.pitch);
// printf("size    %d\n", bitmap.size);
// printf("buffer! %d\n", sizeof(buffer));
// for (int i = 0; i < sizeof(buffer); i += 1)
// {
//     printf("%04x ", buffer[i]);
//     if (i % 16 == 0)
//     {
//         printf("\n");
//     }
// }
// printf("\n");
// hagl_blit_xywh(hagl_backend, WIDTH / 2 * 3 / 2, y, 8 * 4, 13 * 4, &bitmap);
