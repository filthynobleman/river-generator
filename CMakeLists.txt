cmake_minimum_required(VERSION 3.16.0)
project(RiverTerrains LANGUAGES C CXX)


# External dependencies home directories
set(STB_HOME "${CMAKE_SOURCE_DIR}/ext/stb")
set(JSON_HOME "${CMAKE_SOURCE_DIR}/ext/json")
set(SFML_HOME "${CMAKE_SOURCE_DIR}/ext/sfml")
set(EIGEN3_HOME "${CMAKE_SOURCE_DIR}/ext/eigen3")

# Include dependencies
# Nlohmann's json
include_directories("${JSON_HOME}/include")

# STB
include_directories("${STB_HOME}")
add_library(STB STATIC "${CMAKE_SOURCE_DIR}/src/stb/stb.cpp")

# SFML
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/build-sfml/install")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS Graphics REQUIRED)

# Eigen
include_directories("${EIGEN3_HOME}")



# Application
include_directories("${CMAKE_SOURCE_DIR}/include")

add_library(RTLib STATIC    "${CMAKE_SOURCE_DIR}/src/sfLine.cpp"
                            "${CMAKE_SOURCE_DIR}/src/sfSmoothLine.cpp"
                            "${CMAKE_SOURCE_DIR}/src/graph.cpp"
                            "${CMAKE_SOURCE_DIR}/src/delaunay.cpp"
                            "${CMAKE_SOURCE_DIR}/src/spline.cpp"
                            "${CMAKE_SOURCE_DIR}/src/gauss_blur.cpp"
                            "${CMAKE_SOURCE_DIR}/src/river.cpp"
                            "${CMAKE_SOURCE_DIR}/src/hmap.cpp"
                            "${CMAKE_SOURCE_DIR}/src/noises.cpp"
                            "${CMAKE_SOURCE_DIR}/src/parser.cpp"
                            "${CMAKE_SOURCE_DIR}/src/plane.cpp")
target_link_libraries(RTLib STB SFML::Graphics)

add_executable(RiverGen "${CMAKE_SOURCE_DIR}/src/main.cpp")
target_link_libraries(RiverGen STB SFML::Graphics RTLib)
set_target_properties(RiverGen PROPERTIES CXX_STANDARD 17)