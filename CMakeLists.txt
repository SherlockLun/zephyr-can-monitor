cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(zephyr_can_monitor VERSION 1.0.0)

target_sources(app PRIVATE
    src/main.c
    src/can_monitor.c
    src/can_tx.c
    src/logger.c
)

target_include_directories(app PRIVATE src)
