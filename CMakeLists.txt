add_library(hagl_hal INTERFACE)

target_sources(hagl_hal INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/framebuffer.S
  ${CMAKE_CURRENT_LIST_DIR}/src/hagl_hal.c
  ${CMAKE_CURRENT_LIST_DIR}/src/pico-vga-framebuffer.c
)

target_include_directories(hagl_hal INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(hagl_hal INTERFACE pico_stdlib hardware_spi hardware_gpio hardware_dma hagl)

target_compile_definitions(hagl_hal INTERFACE NO_MENUCONFIG)

# EOF
