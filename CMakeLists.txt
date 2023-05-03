cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_policy(SET CMP0054 NEW)

set(CMAKE_C_STANDARD 99)
set (PROJECT two_motor_74HC)
project (${PROJECT})

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

add_definitions(
    -D_DEFAULT_SOURCE
)
# add_link_options(-Wall -g -Wl,-gc-sections)

set (MAIN_SRC
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/src/gpio.c
    ${PROJECT_SOURCE_DIR}/src/stepm_74hc.c
)

add_executable(${PROJECT} ${MAIN_SRC})

find_package(Git)
add_custom_target(version
  ${CMAKE_COMMAND} -D SRC=${PROJECT_SOURCE_DIR}/app_version.h.in
                   -D DST=${PROJECT_SOURCE_DIR}/src/app_version.h
                   -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                   -P ${CMAKE_SOURCE_DIR}/GenerateVersionHeader.cmake
  )

add_dependencies(${PROJECT} version)

target_link_libraries(${PROJECT}

)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
install(TARGETS ${PROJECT} DESTINATION bin)
