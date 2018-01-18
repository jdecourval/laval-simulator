add_executable(LavalTests
        integration/multiplication.cpp
        integration/log2.cpp

        unit/assembler.cpp
        unit/core.cpp
        unit/core_array.cpp
        unit/direction.cpp
        unit/instruction.cpp
        unit/instructions.cpp
        unit/instruction_factory.cpp
        unit/main.cpp

        test_instructions.h
        test_instructions.cpp
        )

target_include_directories(LavalTests
        PRIVATE ..
        PRIVATE .
)


target_link_libraries(LavalTests
        LavalLib
        )

enable_testing()
