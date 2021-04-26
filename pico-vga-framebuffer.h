#ifndef _HAGL_PICO_VGAFRAMEBUFFER_H
#define _HAGL_PICO_VGAFRAMEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#define WIDTH   DISPLAY_WIDTH
#define HEIGHT  DISPLAY_HEIGHT
#define NCLR    16

#define BLACK        PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u,   0u)
#define DARK_RED     PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u,   0u,   0u)
#define DARK_GREEN   PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 128u,   0u)
#define DARK_YELLOW  PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u, 128u,   0u)
#define DARK_BLUE    PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u, 128u)
#define DARK_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u,   0u, 128u)
#define DARK_CYAN    PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 128u, 128u)
#define LIGHT_GREY   PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u, 128u, 128u)
#define DARK_GREY    PICO_SCANVIDEO_PIXEL_FROM_RGB8( 64u,  64u,  64u)
#define RED          PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u,   0u,   0u)
#define GREEN        PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 255u,   0u)
#define YELLOW       PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u,   0u)
#define BLUE         PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u, 255u)
#define MAGENTA      PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u,   0u, 255u)
#define CYAN         PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 255u, 255u)
#define WHITE        PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u, 255u)

void setup_video(void);

void set_colours(uint16_t *pclr);

void plot_point(int x, int y, int clr);

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_PICO_VGAFRAMEBUFFER_H */
