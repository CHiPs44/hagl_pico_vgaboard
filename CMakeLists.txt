add_library(hagl_hal INTERFACE)

if (NOT PICO_NO_HARDWARE)
  target_sources(hagl_hal INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/framebuffer.S
    ${CMAKE_CURRENT_LIST_DIR}/src/pico-vgaboard-palettes.c
    ${CMAKE_CURRENT_LIST_DIR}/src/pico-vgaboard.c
    ${CMAKE_CURRENT_LIST_DIR}/src/hagl_hal.c
  )
else()
  target_sources(hagl_hal INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/pico-vgaboard-palettes.c
    ${CMAKE_CURRENT_LIST_DIR}/src/pico-vgaboard.c
    ${CMAKE_CURRENT_LIST_DIR}/src/hagl_hal.c
  )
endif()

target_include_directories(hagl_hal INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

if (NOT PICO_NO_HARDWARE)
  target_link_libraries(hagl_hal INTERFACE 
    hardware_spi 
    hardware_dma 
    pico_stdlib 
    hardware_gpio 
    hagl
  )
endif()

target_link_libraries(hagl_hal INTERFACE 
  pico_stdlib 
  hardware_gpio 
  hagl
)

target_compile_definitions(hagl_hal INTERFACE NO_MENUCONFIG)

# EOF
