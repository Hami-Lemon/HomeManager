cmake_minimum_required(VERSION 3.16)

set(CMAKE_BUILD_TYPE Debug)
set(SOURCE
        src/serial/serial.c
        src/server/server.c
        src/server/connection.c
        src/server/const.c

        src/zigbee/zigbee.c

        src/util.c
        src/main.c)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 99)

project(manager C)

add_executable(${PROJECT_NAME} ${SOURCE})
