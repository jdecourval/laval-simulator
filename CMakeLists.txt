cmake_minimum_required(VERSION 3.8)


set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -fuse-linker-plugin -march=native")

add_subdirectory(lib)
add_subdirectory(sources)
