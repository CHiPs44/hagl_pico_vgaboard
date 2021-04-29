#ifndef _HAGL_PICO_VGAFRAMEBUFFER_H
#define _HAGL_PICO_VGAFRAMEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG 1
#define USE_INTERP 1

#define WIDTH   DISPLAY_WIDTH
#define HEIGHT  DISPLAY_HEIGHT
#define NCLR    16

void setup_video(void);

void set_colours(uint16_t *pclr);

void render_loop(void);

void plot_point(int x, int y, int clr);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGAFRAMEBUFFER_H */
