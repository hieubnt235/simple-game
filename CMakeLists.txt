cmake_minimum_required(VERSION 4.0)
project(simple-game LANGUAGES CXX)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE c_std_17)

