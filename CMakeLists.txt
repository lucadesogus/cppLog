cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)
project(cpplog)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
find_package (Threads REQUIRED)
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)
add_executable(${PROJECT_NAME} "example/main.cpp")
target_link_libraries(${PROJECT_NAME} Threads::Threads)
