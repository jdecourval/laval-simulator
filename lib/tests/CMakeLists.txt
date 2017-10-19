add_executable(LavalTests
        catch.hpp
        core.cpp
        core_array.cpp
        direction.cpp
        instruction.cpp
        instructions.cpp
        instruction_factory.cpp
        main.cpp
        test_instructions.h
        test_instructions.cpp
        )

target_include_directories(LavalTests
        PRIVATE ..)

target_link_libraries(LavalTests
        LavalLib
        )

enable_testing()
