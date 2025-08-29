cmake_minimum_required(VERSION 3.16)
project(GraphDatabase VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Dependencies
find_package(Threads REQUIRED)

# Include headers
include_directories(include)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)
