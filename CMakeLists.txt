cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_C_STANDARD 99)
set (PROJECT two_motor_74HC)
project (${PROJECT})

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

add_definitions(
    -D_DEFAULT_SOURCE
)
add_link_options(-Wall -g -Wl,-gc-sections)

set (MAIN_SRC
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/src/gpio.c
    ${PROJECT_SOURCE_DIR}/src/stepm_74hc.c
)

add_executable(${PROJECT} ${MAIN_SRC})

target_link_libraries(${PROJECT}

)
