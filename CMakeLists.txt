cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)
project(RayTracer)

#GLM
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/RayTacer/includes/glm)
#Custom Include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/RayTracer/includes)

add_library(BasicLib
    "RayTracer/includes/Ray.cpp"
    "RayTracer/includes/Hitable.cpp"
    "RayTracer/includes/Camera.cpp"
    "RayTracer/includes/material.cpp"
    "RayTracer/includes/drand48.cpp"
)

add_executable(main "RayTracer/main.cpp")
target_link_libraries(main BasicLib)