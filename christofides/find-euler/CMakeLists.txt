cmake_minimum_required(VERSION 3.6)
project(find_euler)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("${PROJECT_SOURCE_DIR}/Blossom"
        "${PROJECT_SOURCE_DIR}/Blossom/GEOM"
        "${PROJECT_SOURCE_DIR}/Blossom/GeomPerfectMatching"
        "${PROJECT_SOURCE_DIR}/Blossom/MinCost"
        "${PROJECT_SOURCE_DIR}/Graph/"
        )
file(GLOB src "*.cpp" "./Blossom/*.cpp" "./Blossom/*/*.cpp" "./Graph/*.cpp")

set(SOURCE_FILES ${src})

add_executable(find_euler ${SOURCE_FILES})