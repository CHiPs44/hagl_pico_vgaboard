cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD vgaboard)

# hagl/CMakeLists.txt wants this
set(PICO_SDK 1)

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.0")
  message(
    FATAL_ERROR 
    "Raspberry Pi Pico SDK version 1.5.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}"
  )
endif()

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
  hardware_interp
  pico_rand
  pico_scanvideo_dpi
  pico_multicore
  hagl_hal
  # hagl
)

pico_add_extra_outputs(hagl_pico_vgaboard_example)

# Add HAGL directory
add_subdirectory(external/hagl)

# Add this in a project that uses this HAL
# (git add submodule https://github.com/CHiPs44/hagl_pico_vgaboard.git external/hagl_hal)
# add_subdirectory(external/hagl_hal)

# Remove this in a project that uses this HAL, 
# it's specific to this example to make it work within the main project
include(../CMakeLists.txt)

target_compile_definitions(hagl_pico_vgaboard_example PUBLIC 
  #CMAKE_BUILD_TYPE=Debug
  CMAKE_BUILD_TYPE=Release
  # needed for 640x480 modes
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=322
  # needed for 800x600 modes
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=402
  # needed for 1024x768 modes
  PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=514
  # needed for 1280 pixels width
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=642
  # PICO_SCANVIDEO_SCANLINE_BUFFER_COUNT=8
  # Works with 4bpp modes only
  USE_INTERP=1
  #USE_INTERP=0
  # TINY: OK for 640x400 based modes and below
  # PICO_VGABOARD_FRAMEBUFFER_SIZE=32768
  # NORMAL: OK for 640x480 based modes and below
  # PICO_VGABOARD_FRAMEBUFFER_SIZE=65536
  # BIG: OK for 16 or 256 colors based modes in "hi-res"
  # PICO_VGABOARD_FRAMEBUFFER_SIZE=76800
  # PICO_VGABOARD_FRAMEBUFFER_SIZE=98304
  # PICO_VGABOARD_FRAMEBUFFER_SIZE=131072
  # As we use 8x13 fonts max, set char buffer size accordingly (8 * 13 * 2)
  HAGL_CHAR_BUFFER_SIZE=208
  # Why have every HAGL pixel use 16 bits when you know you are an 8 bit guy?
  HAGL_HAL_COLOR_BITS=8
  # -DHAGL_HAL_COLOR_BITS=16
  # Finding available RAM via successive calls to malloc() obviously doesn't work without this ;-)
  PICO_MALLOC_PANIC=0
  # Debbugging features
  HAGL_HAL_DEBUG=1
  PICO_VGABOARD_DEBUG=1
  # NB: won't work on Raspberry Pi Pico W for now
  USE_ONBOARD_LED=1
)

# EOF
