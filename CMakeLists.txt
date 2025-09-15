cmake_minimum_required(VERSION 4.0)

project(simple-game LANGUAGES CXX)
message("- Project name : ${PROJECT_NAME}")

set(TARGET ${PROJECT_NAME})
add_executable(${TARGET})
target_sources(${TARGET} PRIVATE src/main.cpp)
target_compile_features(${TARGET} PRIVATE cxx_std_17) #The same as set(CMAKE_CXX_STANDARD 17), but for only one target.
message("- Target name : ${TARGET}")

include(FetchContent)
FetchContent_Declare(
        timer_lib
        GIT_REPOSITORY https://github.com/pr0g/timer_lib.git
        GIT_TAG v1.0
)
FetchContent_Declare(
        as-c-math
        GIT_REPOSITORY https://github.com/pr0g/as-c-math.git
        GIT_TAG 616fe946956561ef4884fc32c4eec2432fd952c8)

set(AS_MATH_ENABLE_TEST OFF CACHE INTERNAL "")
set(AS_MATH_ENABLE_COVERAGE OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(timer_lib as-c-math)
target_link_libraries(${TARGET} PRIVATE timer_lib as-c-math)



if (DEBUG)
    include(CMakeHelper.cmake)
    list_cmake_variables()
endif ()
