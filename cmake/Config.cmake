
include(${CMAKE_CURRENT_LIST_DIR}/Options.cmake)

find_package(spdlog REQUIRED)

find_package(PkgConfig)
pkg_search_module(wlroots REQUIRED IMPORTED_TARGET wlroots)
pkg_search_module(wayland-server REQUIRED IMPORTED_TARGET wayland-server)

if(BUILD_TESTING MATCHES ON)
    enable_testing()
    find_package(GTest REQUIRED)
    include(CTest)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/Macros.cmake)

configure_file (
    "${PROJECT_SOURCE_DIR}/cmake/ProjectConfig.in"
    "${PROJECT_BINARY_DIR}/include/ProjectConfig.hpp"
)

include_directories(${PROJECT_BINARY_DIR}/include)
