cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(vband C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_executable(vband
  main.c
  tusb_config.h
  usb_descriptors.c
  usb_descriptors.h
  keyboard.c
  keyboard.h
  keyer.c
  keyer.h
  )

#pico_enable_stdio_usb(vband 1)
pico_enable_stdio_uart(vband 1)
pico_add_extra_outputs(vband)
target_link_libraries(vband pico_stdlib tinyusb_device tinyusb_board hardware_gpio hardware_pwm)

target_sources(vband PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(vband PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(vband)
