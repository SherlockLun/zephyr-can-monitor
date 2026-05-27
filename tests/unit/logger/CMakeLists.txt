cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(test_logger)

get_filename_component(APP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)

target_sources(app PRIVATE
    src/test_logger.c
    ${APP_ROOT}/src/logger.c
)

target_include_directories(app PRIVATE
    ${APP_ROOT}/src
)
