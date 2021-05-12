cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD vgaboard)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(hagl_pico_vgaboard_example C CXX ASM)

pico_sdk_init()

# include(external/hagl/CMakeLists.txt)
# include(../CMakeLists.txt)

add_executable(hagl_pico_vgaboard_example
  example.c
)

pico_set_program_name(hagl_pico_vgaboard_example "Raspberry Pi Pico HAGL VGA board example")
pico_set_program_version(hagl_pico_vgaboard_example "0.1")

target_compile_definitions(hagl_pico_vgaboard_example PUBLIC 
  -DPICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=322
  -DPICO_SCANVIDEO_SCANLINE_BUFFER_COUNT=8
)

pico_enable_stdio_uart(hagl_pico_vgaboard_example 0)
pico_enable_stdio_usb(hagl_pico_vgaboard_example 1)

pico_add_extra_outputs(hagl_pico_vgaboard_example)

target_link_libraries(
  hagl_pico_vgaboard_example 
    PRIVATE
      pico_stdlib
      pico_scanvideo_dpi
      pico_multicore
      hardware_interp
      # hagl
      hagl_hal
)

add_subdirectory(external/hagl/)
# add_subdirectory(../)

target_include_directories(hagl_pico_vgaboard_example PRIVATE
  ./external/hagl/include/
  ../include/
)

# EOF
