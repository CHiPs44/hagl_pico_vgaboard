cmake_minimum_required(VERSION 3.13)

add_library(hagl_hal INTERFACE)

# # cf. https://github.com/raspberrypi/pico-sdk/issues/1789
# set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/include/boards)

if (NOT PICO_NO_HARDWARE)
  target_sources(hagl_hal INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/framebuffer.S
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/buttons.c
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/font-bios-f08.c
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/pico-vgaboard-console.c
  )
endif()

target_sources(hagl_hal INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/font-bios-f08.c
  ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/palettes.c
  ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/pico-vgaboard-console.c
  ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/pico-vgaboard-framebuffer.c
  ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/pico-vgaboard.c
  ${CMAKE_CURRENT_LIST_DIR}/hagl_hal/src/hagl_hal.c
  ${CMAKE_CURRENT_LIST_DIR}/hagl_ext/src/hagl_ext_blit.c
  ${CMAKE_CURRENT_LIST_DIR}/hagl_ext/src/hagl_ext_char.c
  ${CMAKE_CURRENT_LIST_DIR}/hagl_ext/src/hagl_ext.c
)

target_include_directories(hagl_hal INTERFACE 
  ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/include
  ${CMAKE_CURRENT_LIST_DIR}/hagl_hal/include
  ${CMAKE_CURRENT_LIST_DIR}/hagl_ext/include
)

if (NOT PICO_NO_HARDWARE)
  target_link_libraries(hagl_hal INTERFACE 
    hardware_dma 
    hardware_gpio 
    hardware_spi 
    pico_stdlib 
    hagl
  )
endif()

target_link_libraries(hagl_hal INTERFACE 
  pico_stdlib 
  hagl
)

# From https://forums.raspberrypi.com/viewtopic.php?t=333955#p2001781
#   => https://github.com/BitBoxSwiss/mcu/blob/master/arm.cmake#L22
# Avoid known bug in linux giving: 
#    arm-none-eabi-gcc: error: unrecognized command line option '-rdynamic'
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# target_compile_definitions(hagl_hal INTERFACE 
#   NO_MENUCONFIG
#   PICO_SCANVIDEO_PLANE_COUNT=2
#   # PICO_SCANVIDEO_PLANE_COUNT=3
# )

# EOF
