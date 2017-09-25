add_executable(LavalTests
        catch.hpp
        core.cpp
        direction.cpp
        instructions.cpp
        main.cpp
        )

target_include_directories(LavalTests
        PRIVATE ..)

target_link_libraries(LavalTests
        LavalLib
        )

enable_testing()
