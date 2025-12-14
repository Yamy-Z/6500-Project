cmake_minimum_required(VERSION 3.10)
project(rmi_project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

include_directories(include)

add_executable(rmi_bench
    src/main.cpp
    src/dataloader.cpp
    src/linear.cpp
    src/RMI.cpp
)