cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_NAME pico_vgaboard_console_example)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
   set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# ====================================================================================

# Use cmake --log-level=debug to get debug output.
set(CMAKE_MESSAGE_LOG_LEVEL "DEBUG")

# cf. https://github.com/raspberrypi/pico-sdk/issues/1789
set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/boards)

# cf. https://forums.raspberrypi.com/viewtopic.php?t=375074#p2248387

# # RP2040
# set(PICO_PLATFORM rp2040)
# # Pico
# set(PICO_BOARD vgaboard)

# RP2350
set(PICO_PLATFORM rp2350)
# Pico 2
set(PICO_BOARD vgaboard-pico2)
# # Pimoroni Pico Plus 2 / RP2350
# set(PICO_BOARD vgaboard-pimorono-pico-plus-2)

# set(UART_TYPE NONE)
set(UART_TYPE USB)
# set(UART_TYPE GPIO)

# hagl/CMakeLists.txt wants this
set(PICO_SDK 1)

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.0.0")
  message(
    FATAL_ERROR 
    "Raspberry Pi Pico SDK version 2.0.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}"
  )
endif()

project(${PROJECT_NAME} C CXX ASM)

pico_sdk_init()

add_executable(${PROJECT_NAME} example.c ..)

# Add this in a project that uses this HAL
include(../CMakeLists.txt)

include(../../hagl/CMakeLists.txt)

# Set program name and version for UF2 / picotool
pico_set_program_name(${PROJECT_NAME} 
  "Raspberry Pi Pico multiplane example for VGA board"
)
pico_set_program_version(${PROJECT_NAME} 
  "0.1"
)

if (UART_TYPE EQUAL NONE)
  # Disable USB output, disable UART output
  pico_enable_stdio_usb(${PROJECT_NAME} 0)
  pico_enable_stdio_uart(${PROJECT_NAME} 0)
endif()

if (UART_TYPE EQUAL USB)
  # Enable USB output, disable UART output
  pico_enable_stdio_usb(${PROJECT_NAME} 1)
  pico_enable_stdio_uart(${PROJECT_NAME} 0)
endif()

if (UART_TYPE EQUAL GPIO)
  # Serial on GPIO20/GPIO21 with cutted tracks
  pico_enable_stdio_usb(${PROJECT_NAME} 0)
  pico_enable_stdio_uart(${PROJECT_NAME} 1)
  target_compile_definitions(${PROJECT_NAME} PUBLIC
    PICO_DEFAULT_UART_TX_PIN=20
    PICO_DEFAULT_UART_RX_PIN=21
  )
  # Serial means SD card is 1 bit (4 bits otherwise)
  set(PICO_SD_DAT_PIN_COUNT 1)
endif()

# Add the standard library to the build
target_link_libraries(${PROJECT_NAME} 
  pico_stdlib
)

# Add any user requested libraries
if (NOT PICO_NO_HARDWARE)
  target_link_libraries(${PROJECT_NAME} 
    hardware_interp
    pico_rand
  )
endif()
target_link_libraries(${PROJECT_NAME} 
  pico_scanvideo_dpi
  pico_multicore
  hagl
  hagl_hal
)

pico_add_extra_outputs(${PROJECT_NAME})

# To change build type, use:
# cmake .. -DCMAKE_BUILD_TYPE=Debug
# cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
# cmake .. -DCMAKE_BUILD_TYPE=Release
# (clearing build directory before cmake .. is quite always a good idea)
# No more <optimized out> messages (really?)
# -DPICO_DEOPTIMIZED_DEBUG=1

target_compile_definitions(${PROJECT_NAME} PUBLIC
  # PICO_SCANVIDEO_PLANE_COUNT=2
  PICO_SCANVIDEO_PLANE_COUNT=3
  # Allow VREG voltage to be more than default?
  ALLOW_VREG_VOLTAGE_OVERRIDE=0
  # ALLOW_VREG_VOLTAGE_OVERRIDE=1
  # Needed for 640x480 modes
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=322
  # Needed for 800x600 modes
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=402
  # Needed for 1024x768 modes
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=514
  # Needed for 1280 pixels width
  # PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=642
  # Needed for 1680 pixels width
  PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=842
  # Let's go with the default value (8)
  # PICO_SCANVIDEO_SCANLINE_BUFFER_COUNT=8
  # NB: works with 4bpp modes only
  # USE_INTERP=0
  # USE_INTERP=1
  #  # VRAM SIZE: STATIC
  # TINY: OK for 640x400 based modes and below
  # PICO_VGABOARD_VRAM_SIZE=32768
  # NORMAL: OK for 640x480 based modes and below
  # PICO_VGABOARD_VRAM_SIZE=65536
  # 2 x 320x240 16 colors framebuffers
  # PICO_VGABOARD_VRAM_SIZE=76800
  # BIG: OK for 16 or 256 colors based modes in "hi-res"
  # PICO_VGABOARD_VRAM_SIZE=98304
  # 128 KB
  # PICO_VGABOARD_VRAM_SIZE=131072
  # 2 x 320x240 256 colors framebuffers
  # PICO_VGABOARD_VRAM_SIZE=153600
  # 2 x 512*384 16 colors framebuffers
  PICO_VGABOARD_VRAM_SIZE=196608
  # use MALLOC if size is zero => DOES NOT WORK, IMAGE IS CORRUPTED
  # PICO_VGABOARD_VRAM_SIZE=0
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
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS "-Wl,--print-memory-usage")

# EOF
