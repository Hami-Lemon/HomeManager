cmake_minimum_required(VERSION 3.16)

# 源码文件
set(SOURCE
        src/serial/serial.c
        src/server/server.c
        src/zigbee/zigbee.c
        src/camera/camera.c
        src/cstring/cstring.c
        src/logger/logger.c
        src/voice/voice.c
        src/setting.c src/constdef.c src/handler.c src/main.c)

#是否使用arm-linux-gcc编译,默认使用gcc
set(ARM_LINUX)
option(ARM_LINUX "arm-linux" OFF)
if (${ARM_LINUX})
    set(CMAKE_C_COMPILER "arm-linux-gnueabi-gcc")
else ()
    set(CMAKE_C_COMPILER gcc)
endif ()
message(STATUS "compile with ${CMAKE_C_COMPILER}")

#默认生成debug版本
if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()
# C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

project(manager C)

#pthread库
find_package(Threads REQUIRED)
if (${Threads_FOUND})
    if (${CMAKE_USE_PTHREADS_INIT})
        message(STATUS "found pthread: OK")
    else ()
        message(STATUS "found thread library, but not pthread")
        message(FATAL_ERROR "pthread not found!")
    endif ()
endif ()

#cJson https://github.com/DaveGamble/cJSON v1.7.15
set(CJSON_SOURCE cJson/cJSON.c)
set(CJSON_HEADER cJson/)
add_library(cJson STATIC ${CJSON_SOURCE})

add_executable(${PROJECT_NAME} ${SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CJSON_HEADER})
target_link_libraries(${PROJECT_NAME} PRIVATE cJson)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

#默认安装路径
if (${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT})
    message(STATUS "Setting default CMAKE_INSTALL_PREFIX: ${PROJECT_BINARY_DIR}/install")
    set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE STRING "The path to use for make install" FORCE)
endif ()

set(SETTING_FILE src/setting.json)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_BUILD_TYPE}")
install(FILES ${SETTING_FILE}
        DESTINATION "${CMAKE_BUILD_TYPE}")

# uninstall
add_custom_target(uninstall
        COMMAND rm -rf "${CMAKE_INSTALL_PREFIX}")
