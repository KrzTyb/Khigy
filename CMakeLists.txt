cmake_minimum_required(VERSION 3.24)

project(Khigy VERSION 1.0.0
    DESCRIPTION "Khigy Wayland compositor"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
    WLR_USE_UNSTABLE=1 # Stability is not provided with future wlroots versions
)

include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)


add_subdirectory(src)
