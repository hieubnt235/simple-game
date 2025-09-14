cmake_minimum_required(VERSION 4.0)
project(simple-game LANGUAGES CXX)
message("Project name is : ${PROJECT_NAME}")

set(TARGET ${PROJECT_NAME})
add_executable(${TARGET})
message("Target name is: ${TARGET} ${TARGET}")

target_sources(${TARGET} PRIVATE src/main.cpp)

#The same as set(CMAKE_CXX_STANDARD 17), but for only one target.
target_compile_features(${TARGET} PRIVATE cxx_std_17)
