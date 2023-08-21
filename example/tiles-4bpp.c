#define B0 0
#define G0 1
#define G1 2
#define G2 3
#define G3 4
#define BF 14

// clang-format off

hagl_color_t tile_8x8x4_0_bitmap[] = {
    G0, G0, G0, G0, G0, G0, G0, B0,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

hagl_color_t tile_8x8x4_1_bitmap[] = {
    G1, G1, G1, G1, G1, G1, G1, B0,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

hagl_color_t tile_8x8x4_2_bitmap[] = {
    G2, G2, G2, G2, G2, G2, G2, B0,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

hagl_color_t tile_8x8x4_3_bitmap[] = {
    G3, G3, G3, G3, G3, G3, G3, B0,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

// clang-format on

hagl_bitmap_t tile_8x8x4_0 = {
    .width = 8,
    .height = 8,
    .depth = 4,                          // bits
    .pitch = 16,                         // 8 * 2, bytes per row
    .size = sizeof(tile_8x8x4_0_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile_8x8x4_0_bitmap};

hagl_bitmap_t tile_8x8x4_1 = {
    .width = 8,
    .height = 8,
    .depth = 4,                          // bits
    .pitch = 16,                         // 8 * 2, bytes per row
    .size = sizeof(tile_8x8x4_1_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile_8x8x4_1_bitmap};

hagl_bitmap_t tile_8x8x4_2 = {
    .width = 8,
    .height = 8,
    .depth = 4,                          // bits
    .pitch = 16,                         // 8 * 2, bytes per row
    .size = sizeof(tile_8x8x4_2_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile_8x8x4_2_bitmap};

hagl_bitmap_t tile_8x8x4_3 = {
    .width = 8,
    .height = 8,
    .depth = 4,                          // bits
    .pitch = 16,                         // 8 * 2, bytes per row
    .size = sizeof(tile_8x8x4_3_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile_8x8x4_3_bitmap};

hagl_bitmap_t *tiles_8x8x4[] = {
    &tile_8x8x4_0,
    &tile_8x8x4_1,
    &tile_8x8x4_2,
    &tile_8x8x4_3,
};

const int tile_zoom = 1;
int tile_width;
int tile_height;
int tile_columns;
int tile_lines;
uint8_t *tile_map;

void tile_draw()
{
    for (int line = 0; line < tile_lines; line++)
    {
        for (int column = 0; column < tile_columns; column++)
        {
            hagl_blit_xywh(
                hagl_backend,
                DEMO.x + tile_width * column,
                DEMO.y + tile_height * line,
                tile_width, tile_height,
                tiles_8x8x4[tile_map[line * tile_columns + column]]);
        }
    }
}

bool tile_init()
{
    tile_width = 8 * tile_zoom;
    tile_height = 8 * tile_zoom;
    tile_columns = DEMO.w / tile_width;
    tile_lines = DEMO.h / tile_height;
    tile_map = malloc(sizeof(uint8_t) * tile_columns * tile_lines);
    if (tile_map == NULL)
    {
        return false;
    }
    for (int line = 0; line < tile_lines; line++)
    {
        for (int column = 0; column < tile_columns; column++)
        {
            tile_map[line * tile_columns + column] = rand() % 4;
        }
    }
    return true;
}

void tile_anim()
{
    // nothing!
}

// EOF
