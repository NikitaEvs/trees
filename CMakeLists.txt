cmake_minimum_required(VERSION 3.15)
project(task3)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        visLib/thirdparty
)

add_executable(avl trees/avl.cpp)
add_executable(bst trees/bst.cpp)
add_executable(test testCode/test.cpp)
add_executable(kurevo trees/kurevo.cpp)
add_executable(huffman huffman/src/huffman.cpp)
add_executable(seminarHuffman huffman/src/seminarHuffman.cpp)
add_executable(vitter huffman/src/vitterSobaka.cpp)
add_executable(other testCode/otherTest.cpp)
add_executable(simpleHuffman huffman/src/simpleHuffman.cpp)

target_compile_options (simpleHuffman PRIVATE -O0 -fno-stack-limit -x c++ -std=c++11 -Walloc-size-larger-than=PTRDIFF_MAX -Walloc-zero -Walloca -Wformat-overflow -Wformat-truncation -Wnonnull -Wstringop-overflow -Wvla)