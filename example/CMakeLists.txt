cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD vgaboard)
set(UART_TYPE NONE)
set(UART_TYPE USB)
set(UART_TYPE GPIO)

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

set(PROJECT hagl_pico_vgaboard_example)

project(${PROJECT} 
  C CXX ASM
)

pico_sdk_init()

add_executable(${PROJECT}
  example.c
)

# Set program name and version for UF2 / picotool
pico_set_program_name(${PROJECT} 
  "Raspberry Pi Pico HAGL HAL for VGA board example"
)
pico_set_program_version(${PROJECT} 
  "0.1"
)

if (UART_TYPE EQUAL NONE)
  # Disable usb output, disable uart output
  pico_enable_stdio_usb(${PROJECT} 0)
  pico_enable_stdio_uart(${PROJECT} 0)
endif()

if (UART_TYPE EQUAL USB)
  # Enable usb output, disable uart output
  pico_enable_stdio_usb(${PROJECT} 1)
  pico_enable_stdio_uart(${PROJECT} 0)
endif()

if (UART_TYPE EQUAL GPIO)
  # Serial on GPIO20/GPIO21 with tracks cutted
  pico_enable_stdio_usb(${PROJECT} 0)
  pico_enable_stdio_uart(${PROJECT} 1)
  target_compile_definitions(${PROJECT} PUBLIC
    PICO_DEFAULT_UART_TX_PIN=20
    PICO_DEFAULT_UART_RX_PIN=21
  )
endif()

# Add the standard library to the build
target_link_libraries(${PROJECT} 
  pico_stdlib
)

# Add any user requested libraries
if (NOT PICO_NO_HARDWARE)
  target_link_libraries(${PROJECT} 
    hardware_interp
    pico_rand
  )
endif()
target_link_libraries(${PROJECT} 
  pico_scanvideo_dpi
  pico_multicore
  hagl_hal
  # hagl
)

pico_add_extra_outputs(${PROJECT})

# Add HAGL directory
add_subdirectory(external/hagl)

# Add this in a project that uses this HAL
# (git add submodule https://github.com/CHiPs44/hagl_pico_vgaboard.git external/hagl_hal)
# add_subdirectory(external/hagl_hal)

# Remove this in a project that uses this HAL, 
# it's specific to this example to make it work within the main project
include(../CMakeLists.txt)

# To change build type, use:
# cmake .. -DCMAKE_BUILD_TYPE=Debug
# cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
# cmake .. -DCMAKE_BUILD_TYPE=Release
# (clearing build directory before cmake .. is quite always a good idea)

target_compile_definitions(${PROJECT} PUBLIC
  # Allow VREG voltage to be more than default?
  # ALLOW_VREG_VOLTAGE_OVERRIDE=0
  ALLOW_VREG_VOLTAGE_OVERRIDE=1
  # Needed for 640x480 modes
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=322
  # Needed for 800x600 modes
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=402
  # Needed for 1024x768 modes
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=514
  # Needed for 1280 pixels width
  PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=642
  # Let's go with the default value (8)
  # PICO_SCANVIDEO_SCANLINE_BUFFER_COUNT=8
  # NB: works with 4bpp modes only
  # USE_INTERP=0
  USE_INTERP=1
  #  # VRAM SIZE: STATIC
  # TINY: OK for 640x400 based modes and below
  # PICO_VGABOARD_VRAM_SIZE=32768
  # NORMAL: OK for 640x480 based modes and below
  # PICO_VGABOARD_VRAM_SIZE=65536
  # BIG: OK for 16 or 256 colors based modes in "hi-res"
  # PICO_VGABOARD_VRAM_SIZE=76800
  # PICO_VGABOARD_VRAM_SIZE=98304
  # PICO_VGABOARD_VRAM_SIZE=131072
  # 2 x 320x240 16 colors framebuffers
  PICO_VGABOARD_VRAM_SIZE=76800
  # use MALLOC if size is zero => DOES NOT WORK, IMAGE IS CORRUPTED
  # PICO_VGABOARD_VRAM_SIZE=0
  # Double buffer management (WIP)
  PICO_VGABOARD_DOUBLE_BUFFER=0
  # As we use 10x20 fonts max, set char buffer size accordingly (10 * 20 * 2)
  HAGL_CHAR_BUFFER_SIZE=400
  # Why have every HAGL pixel use 16 bits when you know you are an 8 bit guy?
  HAGL_HAL_COLOR_BITS=8
  # HAGL_HAL_COLOR_BITS=16
  # Finding available RAM via successive calls to malloc() obviously doesn't work without this ;-)
  PICO_MALLOC_PANIC=0
  # Debbugging features
  HAGL_HAL_DEBUG=1
  PICO_VGABOARD_DEBUG=1
  # NB: won't work on Raspberry Pi Pico W for now
  USE_ONBOARD_LED=1
)

# To have this summary displayed at end of build:
# Memory region         Used Size  Region Size  %age Used
#            FLASH:      184432 B         2 MB      8.79%
#              RAM:       94392 B       256 KB     36.01%
#        SCRATCH_X:          2 KB         4 KB     50.00%
#        SCRATCH_Y:          0 GB         4 KB      0.00%
set_property(TARGET ${PROJECT} APPEND_STRING PROPERTY LINK_FLAGS "-Wl,--print-memory-usage")

# EOF
