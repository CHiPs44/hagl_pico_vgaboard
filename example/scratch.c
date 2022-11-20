// // Testing 1bpp with a known working mode
// const vgaboard_t vgaboard_320x240x1bpp = {
//     .scanvideo_mode = &vga_mode_320x240_60_chips44,
//     .width = 320,
//     .height = 240,
//     .depth = 1,
//     .colors = 2,
//     .palette = ((uint16_t *)(&vgaboard_bw_palette_1bpp)),
//     .sys_clock_khz = 250000L,
// };

// const scanvideo_mode_t vga_mode_640x120_60_chips44 = {
//     .default_timing = &vga_timing_640x480_60_default,
//     .pio_program = &video_24mhz_composable,
//     .width = 640,
//     .height = 480,
//     .xscale = 1,
//     .yscale = 4,
// };

// /** @brief 640x120@60Hz, 4bpp, 16 colors */
// const vgaboard_t vgaboard_640x120x4bpp = {
//     .scanvideo_mode = &vga_mode_640x120_60_chips44,
//     .freq_hz = 60,
//     .depth = 4,
//     .palette = ((uint16_t *)(&vgaboard_palette_4bpp_irgb_default)),
//     .sys_clock_khz = 250000L,
// };

// /** @brief 640x120@60Hz, 4bpp, 16 colors */
// const vgaboard_t vgaboard_640x120x2bpp = {
//     .scanvideo_mode = &vga_mode_640x120_60_chips44,
//     .freq_hz = 60,
//     .depth = 2,
//     .palette = ((uint16_t *)(&vgaboard_default_palette_2bpp)),
//     .sys_clock_khz = 250000L,
// };

// // base from src/common/pico_scanvideo/vga_modes.c
// /*
// from https://www.intel.com/content/dam/support/us/en/programmable/support-resources/fpga-wiki/asset03/basic-vga-controller-design-example.pdf
// Pixel Clock Frequency: 33.33MHz (33MHz will suffice)
// Table 1: LCD timing parameters for the NEEK board
// Horizontal Scanline Part Pixels Vertical Frame part Lines
// Visible area                800 Visible area          480
// Front Porch                 210 Front Porch            22
// Sync Pulse                   30 Sync Pulse             13
// Back Porch                   16 Back Porch             10
// Whole line                 1056 Whole frame           525
// */
// const scanvideo_timing_t vga_timing_wide_480_intel =
// {
//     // .clock_freq = 33000000,
//     // 34.483 kHz
//     .clock_freq = 34000000,

//     .h_active = 800,
//     .v_active = 480,

// // cf. https://misterfpga.org/viewtopic.php?t=2552
//     // .h_front_porch = 210,
//     .h_front_porch = 40,
//     // .h_pulse = 30,
//     .h_pulse = 48,
//     // .h_total = 1056,
//     .h_total = 928,
//     .h_sync_polarity = 0,

//     // .v_front_porch = 22,
//     .v_front_porch = 13,
//     // .v_pulse = 13,
//     .v_pulse = 3,
//     .v_total = 525,
//     .v_sync_polarity = 0,

//     .enable_clock = 1,
//     .clock_polarity = 0,

//     .enable_den = 1
// };

// const scanvideo_mode_t vga_mode_tft_400x240_intel =
// {
//     .default_timing = &vga_timing_wide_480_intel,
//     .pio_program = &video_24mhz_composable,
//     // .width = 400,
//     // .height = 240,
//     .width = 800,
//     .height = 480,
//     .xscale = 2,
//     .yscale = 2,
// };

// /** @brief 400x240@50Hz, 4bpp, 16 colors */
// const vgaboard_t vgaboard_400x240x4bpp = {
//     .scanvideo_mode = &vga_mode_tft_400x240_intel,
//     .freq_hz = 255,
//     .depth = 4,
//     .palette = ((uint16_t *)(&vgaboard_palette_4bpp_irgb_default)),
//     // .sys_clock_khz = 33000L * 8L,
//     .sys_clock_khz = 34000L * 8L,
// };
