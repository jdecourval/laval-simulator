cmake_minimum_required(VERSION 3.8)
project(simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra")

set(SOURCE_FILES
        main.cpp
        settings.h
        )

add_executable(simulator ${SOURCE_FILES})

target_link_libraries(simulator
        LavalLib)
