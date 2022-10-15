cmake_minimum_required(VERSION 3.16)
project(particles)

set(CMAKE_CXX_STANDARD 20)

add_executable(particles
        src/main.cpp
        src/System.cpp
        src/Application.cpp
        src/Renderer.cpp)
include_directories(${CMAKE_SOURCE_DIR}/ext)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/kernel.cl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/res/particle.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

find_package(OpenCL REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(particles PRIVATE OpenCL::OpenCL)
target_link_libraries(particles PRIVATE raylib)
target_link_libraries(particles PRIVATE fmt::fmt-header-only)