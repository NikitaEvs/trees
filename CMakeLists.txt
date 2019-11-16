cmake_minimum_required(VERSION 3.15)
project(task3)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        visLib/thirdparty
)

add_executable(avl trees/avl.cpp)
add_executable(test testCode/test.cpp)
add_executable(kurevo trees/kurevo.cpp)
add_executable(huffman huffman/src/huffman.cpp)