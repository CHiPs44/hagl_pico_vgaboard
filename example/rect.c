typedef struct {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
} rect_t;

rect_t FULL_SCREEN;
rect_t TOP_LEFT;
rect_t TOP_RIGHT;
rect_t BOTTOM_LEFT;
rect_t BOTTOM_RIGHT;

void dump_rect(rect_t *rect)
{
    printf("RECT %p: x=%d, y=%d, w=%d, h=%d\r\n", rect, rect->x, rect->y, rect->w, rect->h);
}

void init_rect(rect_t *rect, int16_t x, int16_t y, int16_t w, int16_t h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    // dump_rect(rect);
}

void init_windows()
{
    init_rect(&FULL_SCREEN , 0        , 0         , WIDTH    , HEIGHT    );
    init_rect(&TOP_LEFT    , 0        , 0         , WIDTH / 2, HEIGHT / 2);
    init_rect(&TOP_RIGHT   , WIDTH / 2, 0         , WIDTH / 2, HEIGHT / 2);
    init_rect(&BOTTOM_LEFT , 0        , HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
    init_rect(&BOTTOM_RIGHT, WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
}

void clip(rect_t *rect)
{
    hagl_set_clip(hagl_backend, rect->x, rect->y, rect->x + rect->w - 1, rect->y + rect->h - 1);
}

