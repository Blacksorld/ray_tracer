cmake_minimum_required(VERSION 3.7)
project(ray_tracer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp vector3d.cpp vector3d.h ray.cpp ray.h object.h object.cpp object.h sphere.cpp sphere.h triangle.cpp triangle.h screen.cpp screen.h ray_tracer.cpp ray_tracer.h illuminant.cpp illuminant.h quadrilateral.cpp quadrilateral.h)
add_executable(ray_tracer ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(ray_tracer ${SFML_LIBRARIES})
endif()