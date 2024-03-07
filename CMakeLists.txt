add_library(hagl_hal INTERFACE)

if (NOT PICO_NO_HARDWARE)
  target_sources(hagl_hal INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/framebuffer.S
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/palettes.c
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/pico-vgaboard.c
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/buttons.c
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/pico-vgaboard-text.c
    ${CMAKE_CURRENT_LIST_DIR}/hagl_hal/src/hagl_hal.c
    ${CMAKE_CURRENT_LIST_DIR}/hagl_hal/src/hagl_ext_blit.c
    ${CMAKE_CURRENT_LIST_DIR}/hagl_hal/src/hagl_ext_char.c
  )
else()
  target_sources(hagl_hal INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/palettes.c
    ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/src/pico-vgaboard.c
    ${CMAKE_CURRENT_LIST_DIR}/hagl_hal/src/hagl_hal.c
    ${CMAKE_CURRENT_LIST_DIR}/hagl_hal/src/hagl_ext_blit.c
    ${CMAKE_CURRENT_LIST_DIR}/hagl_hal/src/hagl_ext_char.c
  )
endif()

target_include_directories(hagl_hal INTERFACE 
  ${CMAKE_CURRENT_LIST_DIR}/pico_vgaboard/include
  ${CMAKE_CURRENT_LIST_DIR}/hagl_hal/include
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

target_compile_definitions(hagl_hal INTERFACE NO_MENUCONFIG)

# EOF
