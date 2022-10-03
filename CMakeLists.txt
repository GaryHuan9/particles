cmake_minimum_required(VERSION 3.16)
project(particles)

set(CMAKE_CXX_STANDARD 20)

add_executable(particles src/main.cpp)

find_package(OpenCL REQUIRED)
target_link_libraries(particles PRIVATE OpenCL::OpenCL)