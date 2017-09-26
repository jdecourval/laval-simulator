cmake_minimum_required(VERSION 3.8)
project(LavalLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra")

set(SOURCE_FILES
        core.h
        core.cpp
        cpu.h
        cpu.cpp
        direction.h
        direction.cpp
        instruction.h
        instruction.hpp
        instruction_factory.cpp
        instruction_factory.h
        instruction_factory.hpp
        memory.h
        multidimensional_array.h
        opcodes.h
        opcodes.cpp
        registers.h
        settings.h
        tools.h
        tools.hpp
        )

add_library(LavalLib ${SOURCE_FILES})

add_subdirectory(tests)
