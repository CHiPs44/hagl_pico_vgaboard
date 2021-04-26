cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD vgaboard)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)
project(hagl_pico_vgaboard C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
add_executable(hagl_pico_vgaboard
  hagl_pico_vgaboard.c
  framebuffer.S
)
target_compile_definitions(hagl_pico_vgaboard PUBLIC 
  -DPICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=322
  -DPICO_SCANVIDEO_SCANLINE_BUFFER_COUNT=8)

pico_enable_stdio_uart(hagl_pico_vgaboard 0)
pico_enable_stdio_usb(hagl_pico_vgaboard 0)
pico_add_extra_outputs(hagl_pico_vgaboard)
target_link_libraries(hagl_pico_vgaboard PRIVATE
  pico_stdlib
  pico_scanvideo_dpi
  pico_multicore
  hardware_interp)
