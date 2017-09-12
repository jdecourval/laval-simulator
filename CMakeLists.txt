cmake_minimum_required(VERSION 3.8)
project(simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra")

set(SOURCE_FILES
        sources/main.cpp
        sources/core.h
        sources/core.cpp
        sources/direction.h
        sources/direction.cpp
        sources/memory.h
        sources/memory.cpp
        sources/multidimensional_array.h
        sources/opcodes.h
        sources/pool.h
        sources/pool.cpp
        sources/cpu.h
        sources/cpu.cpp
        sources/settings.h
        sources/tools.h
        sources/tools.hpp
        )
add_executable(simulator ${SOURCE_FILES})
