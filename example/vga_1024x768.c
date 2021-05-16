// cf. https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712&start=50#p1864466 (from @kilograham)
// NB: according to http://tinyvga.com/vga-timing/1024x768@60Hz, h-sync and v-sync polarities are negative
const scanvideo_timing_t vga_timing_1024x768_60_default = {
    .clock_freq = 65000000,

    .h_active = 1024,
    .v_active = 768,

    .h_front_porch = 24,
    .h_pulse = 136,
    .h_total = 1344,
    .h_sync_polarity = 0,

    .v_front_porch = 3,
    .v_pulse = 6,
    .v_total = 806,
    // KG .v_sync_polarity = 1,
    .v_sync_polarity = 0,

    .enable_clock = 0,
    .clock_polarity = 0,

    .enable_den = 0,
};

const scanvideo_mode_t vga_mode_1024x768_60 = {
    .default_timing = &vga_timing_1024x768_60_default,
    // .default_timing = &vga_timing_1024x768_60,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 768,
    .xscale = 1,
    .yscale = 1,
};

const scanvideo_mode_t vga_mode_1024x384_60 = {
    .default_timing = &vga_timing_1024x768_60_default,
    // .default_timing = &vga_timing_1024x768_60,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 384,
    .xscale = 1,
    .yscale = 2,
};

const scanvideo_mode_t vga_mode_1024x192_60 = {
    .default_timing = &vga_timing_1024x768_60_default,
    // .default_timing = &vga_timing_1024x768_60,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 384,
    .xscale = 1,
    .yscale = 4,
};

const scanvideo_mode_t vga_mode_512x384_60 = {
    .default_timing = &vga_timing_1024x768_60_default,
    .pio_program = &video_24mhz_composable,
    .width = 512,
    .height = 384,
    .xscale = 2,
    .yscale = 2,
};

const scanvideo_mode_t vga_mode_256x192_60 = {
    .default_timing = &vga_timing_1024x768_60_default,
    .pio_program = &video_24mhz_composable,
    .width = 256,
    .height = 192,
    .xscale = 4,
    .yscale = 4,
};

// #define VGA_MODE (&vga_mode_1024x768_60)
// #define DISPLAY_WIDTH (1024)
// #define DISPLAY_HEIGHT (768)
// #define SYS_CLOCK_MHZ (260)

// #define VGA_MODE (&vga_mode_1024x384_60)
// #define DISPLAY_WIDTH (1024)
// #define DISPLAY_HEIGHT (384)
// #define SYS_CLOCK_MHZ (260)

// #define VGA_MODE (&vga_mode_1024x192_60)
// #define DISPLAY_WIDTH (1024)
// #define DISPLAY_HEIGHT (192)
// #define SYS_CLOCK_MHZ (260)

// #define VGA_MODE (&vga_mode_512x384_60)
// #define DISPLAY_WIDTH (512)
// #define DISPLAY_HEIGHT (384)
// #define SYS_CLOCK_MHZ (260)

// #define VGA_MODE (&vga_mode_256x192_60)
// #define DISPLAY_WIDTH (256)
// #define DISPLAY_HEIGHT (192)
// #define SYS_CLOCK_MHZ (260)
