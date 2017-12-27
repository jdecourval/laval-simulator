cmake_minimum_required(VERSION 3.8)
project(LavalLib)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        assembler.h
        core.h
        cpu.h
        direction.h
        instruction.h
        memory.h
        opcodes.h
        registers.h
        thread_pool.h

        impl/assembler.cpp
        impl/core.cpp
        impl/core_array.h
        impl/core_array.cpp
        impl/cpu.cpp
        impl/demangle.h
        impl/demangle.cpp
        impl/direction.cpp
        impl/direction.hpp
        impl/instruction.hpp
        impl/instruction_factory.cpp
        impl/instruction_factory.h
        impl/instruction_factory.hpp
        impl/memory.cpp
        impl/opcodes.cpp
        impl/registers.cpp
        impl/settings.cpp
        impl/thread_pool.cpp
        impl/thread_pool.hpp
        impl/tools.h
        impl/tools.hpp
        )

add_library(LavalLib ${SOURCE_FILES})

find_package(Threads)

target_link_libraries(LavalLib
        ${CMAKE_THREAD_LIBS_INIT}
        )

# Needed for std::experimental::filesyste,
if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
    target_link_libraries(LavalLib
            "-lstdc++fs"
            )
endif()

target_include_directories(LavalLib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/impl
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/externals
        )


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    target_compile_options(LavalLib
        PUBLIC "-pipe"                      # Use pipe instead of temporary files. This slightly speed up compilation.

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
