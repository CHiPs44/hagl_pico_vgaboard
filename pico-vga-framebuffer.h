#ifndef _HAGL_PICO_VGA_FRAMEBUFFER_H
#define _HAGL_PICO_VGA_FRAMEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEBUG
#define DEBUG 0
#endif
#ifndef USE_INTERP
#define USE_INTERP 1
#endif

#define WIDTH   DISPLAY_WIDTH
#define HEIGHT  DISPLAY_HEIGHT
#define NCLR    16

extern uint16_t palette[NCLR];

void setup_video(const scanvideo_mode_t *vga_mode);

void set_palette(uint16_t *pclr);

void render_loop(void);

void plot_point(uint16_t x, uint16_t y, uint8_t clr);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGA_FRAMEBUFFER_H */
