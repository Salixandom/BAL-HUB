cmake_minimum_required(VERSION 3.10)
project(bal_git VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include path for JSON and utils
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/utils
)

file(GLOB SRC_FILES
    src/*.cpp
    src/utils/*.cpp
)

add_executable(bal ${SRC_FILES})
