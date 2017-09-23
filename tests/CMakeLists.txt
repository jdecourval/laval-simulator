add_executable(tests
        core.cpp
        main.cpp
        direction.cpp
        )

target_include_directories(tests
        PRIVATE ../sources)

enable_testing()
add_test(NAME MyTests COMMAND tests)
