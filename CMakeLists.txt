cmake_minimum_required(VERSION 3.8)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message("Last I check, a clang compilation bug breaks compilation. See Clang Bug 31852 and 33222.")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -march=native")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -fuse-linker-plugin -march=native")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

endif()

add_subdirectory(lib)
add_subdirectory(sources)
