cmake_minimum_required(VERSION 3.8)
project(LavalLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra")

set(SOURCE_FILES
        core.h
        core.cpp
        core.hpp
        core_array.h
        core_array.cpp
        cpu.h
        cpu.cpp
        direction.h
        direction.cpp
        direction.hpp
        instruction.h
        instruction.hpp
        instruction_factory.cpp
        instruction_factory.h
        instruction_factory.hpp
        memory.h
        opcodes.h
        opcodes.cpp
        registers.h
        registers.cpp
        settings.h
        tools.h
        tools.hpp
        )

add_library(LavalLib ${SOURCE_FILES})

target_include_directories(LavalLib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(LavalLib
        PUBLIC "-Wno-unknown-pragmas")

add_subdirectory(tests)
