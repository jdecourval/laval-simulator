cmake_minimum_required(VERSION 3.8)
project(LavalLib)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        core.h
        core.cpp
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


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    target_compile_options(LavalLib
        PUBLIC "-Wall"                      # Activate most warnings.
        PUBLIC "-Wextra"                    # Activate more warnings.
        PUBLIC "-Wnull-dereference"         # Warn of undefined behaviors due to dereferencing a null pointer.
        PUBLIC "-Wmissing-include-dirs"     # Warn about missing user-supplied include directories.
        PUBLIC "-Wuninitialized"            # Warn if an automatic variable is used without first being initialized. Also warn if a non-static reference or non-static const member appears in a class without constructors.
        PUBLIC "-Wstrict-overflow=5"        # Warn about cases where the compiler optimizes based on the assumption that signed overflow does not occur.
        PUBLIC "-Wsuggest-final-types"      # Warn where code quality would be improved by making a class final.
        PUBLIC "-Wsuggest-final-methods"    # Warn where code quality would be improved by making a method final.
        PUBLIC "-Wsuggest-override"         # Warn about overriding virtual functions that are not marked with the override keyword.
        PUBLIC "-Wundef"                    # Warn if an undefined identifier is evaluated in an #if directive.
        PUBLIC "-Wcast-align"               # Warn whenever a pointer is cast such that the required alignment of the target is increased.
        PUBLIC "-Wuseless-cast"             # Warn when an expression is casted to its own type.
        PUBLIC "-Wredundant-decls"          # Warn if anything is declared more than once in the same scope.
#        PUBLIC "-Wconversion"              # Activate conversion warnings. This yield a large number of false positive.

        PUBLIC "-Wno-unknown-pragmas"       # Make GCC not scream about CLion/clang pragmas
        )

endif()

add_subdirectory(tests)
