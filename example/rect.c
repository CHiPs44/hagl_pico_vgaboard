/* SPDX-License-Identifier: MIT-0 */

typedef struct _rect_t {
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
rect_t TITLE;
rect_t DEMO;
rect_t SCROLLER;

void rect_dump(char *message, rect_t *rect)
{
    printf(
        "RECT %s: p=%p, x=%d, y=%d, w=%d, h=%d\r\n", 
        message, rect, rect->x, rect->y, rect->w, rect->h
    );
}

void rect_init(rect_t *rect, int16_t x, int16_t y, int16_t w, int16_t h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

void rect_copy(rect_t *source, rect_t *destination)
{
    destination->x = source->x;
    destination->y = source->y;
    destination->w = source->w;
    destination->h = source->h;
}

void init_windows(int16_t title_font_h, int16_t scroller_font_h)
{
    rect_init(&FULL_SCREEN , 0        , 0                       , WIDTH    , HEIGHT                                 );
    // Screen quarters
    rect_init(&TOP_LEFT    , 0        , 0                       , WIDTH / 2, HEIGHT / 2                             );
    rect_init(&TOP_RIGHT   , WIDTH / 2, 0                       , WIDTH / 2, HEIGHT / 2                             );
    rect_init(&BOTTOM_LEFT , 0        , HEIGHT / 2              , WIDTH / 2, HEIGHT / 2                             );
    rect_init(&BOTTOM_RIGHT, WIDTH / 2, HEIGHT / 2              , WIDTH / 2, HEIGHT / 2                             );
    // Demo layout: title + demo + scroller
    rect_init(&TITLE       , 0        , 0                       , WIDTH    , title_font_h                           );
    rect_init(&DEMO        , 0        , TITLE.h + 1             , WIDTH    , HEIGHT - title_font_h - scroller_font_h);
    rect_init(&SCROLLER    , 0        , HEIGHT - scroller_font_h, WIDTH    , scroller_font_h                        );
    rect_dump("TITLE   ", &TITLE);
    rect_dump("DEMO    ", &DEMO);
    rect_dump("SCROLLER", &SCROLLER);
}

void clip(rect_t *rect)
{
    rect_dump("CLIP    ", rect);
    hagl_set_clip(hagl_backend, rect->x, rect->y, rect->x + rect->w - 1, rect->y + rect->h - 1);
}

/* EOF */
