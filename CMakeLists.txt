cmake_minimum_required(VERSION 3.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(pico_sdk_import.cmake)

project(pico_rectangle CXX C)

pico_sdk_init()

include_directories(include)

add_executable(md2040
  src/main.cpp
  src/pins.cpp
  src/output_modes.cpp
  src/controller_state.cpp
)

target_link_libraries(md2040 pico_stdlib hardware_gpio)

pico_add_extra_outputs(md2040)
