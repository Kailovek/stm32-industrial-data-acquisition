cmake_minimum_required(VERSION 3.16)
project(stm32_industrial_data_acquisition_demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(daq_core
    src/acquisition.c
    src/modbus_rtu.c
    src/moving_average.c
    src/sensor_scheduler.c
    src/uplink_state.c
)
target_include_directories(daq_core PUBLIC include)

add_library(mock_transport platform/mock_transport.c)
target_include_directories(mock_transport PUBLIC include platform)
target_link_libraries(mock_transport PUBLIC daq_core)

if(MSVC)
    target_compile_options(daq_core PRIVATE /W4 /WX)
    target_compile_options(mock_transport PRIVATE /W4 /WX)
else()
    target_compile_options(daq_core PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(mock_transport PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(host_demo examples/host_demo.c)
target_link_libraries(host_demo PRIVATE daq_core mock_transport)
