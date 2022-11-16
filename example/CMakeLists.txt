cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD vgaboard)

# hagl/CMakeLists.txt wants this
set(PICO_SDK 1)

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(hagl_pico_vgaboard_example 
  C CXX ASM
)

pico_sdk_init()

add_executable(hagl_pico_vgaboard_example
  example.c
)

pico_set_program_name(hagl_pico_vgaboard_example 
  "Raspberry Pi Pico HAGL HAL for VGA board example"
)
pico_set_program_version(hagl_pico_vgaboard_example 
  "0.1"
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(hagl_pico_vgaboard_example 1)
pico_enable_stdio_uart(hagl_pico_vgaboard_example 0)

# Add the standard library to the build
target_link_libraries(hagl_pico_vgaboard_example 
  pico_stdlib
)

# Add any user requested libraries
target_link_libraries(hagl_pico_vgaboard_example 
  pico_scanvideo_dpi
  pico_multicore
  hardware_interp
  hagl_hal
  # hagl
)

pico_add_extra_outputs(hagl_pico_vgaboard_example)

add_subdirectory(external/hagl)

# Add this in a project that uses this HAL
# (git add submodule https://github.com/CHiPs44/hagl_pico_vgaboard.git external/hagl_hal)
# add_subdirectory(external/hagl_hal)

# Remove this in a project that uses this HAL, 
# it's specific to this example to malke it work within the main project
include(../CMakeLists.txt)

target_compile_definitions(hagl_pico_vgaboard_example PUBLIC 
  -DPICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=322
  -DPICO_SCANVIDEO_SCANLINE_BUFFER_COUNT=8
  -DUSE_INTERP=1
  -DHAGL_HAL_PICO_VGABOARD_DEBUG=1
  -DPICO_VGABOARD_DEBUG=1
)

# 1024x768 tests, does nothing
# target_compile_definitions(hagl_pico_vgaboard_example PUBLIC 
#   -DPICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=514
#   -DPICO_SCANVIDEO_SCANLINE_BUFFER_COUNT=8
#   -DUSE_INTERP=1
# )

# EOF
