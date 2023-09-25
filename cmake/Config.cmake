
include(${CMAKE_CURRENT_LIST_DIR}/Options.cmake)

find_package(spdlog REQUIRED)

find_package(PkgConfig)
pkg_search_module(wlroots REQUIRED IMPORTED_TARGET wlroots)
pkg_search_module(wayland-server REQUIRED IMPORTED_TARGET wayland-server)
pkg_search_module(wayland-protocols REQUIRED IMPORTED_TARGET wayland-protocols)
pkg_search_module(wayland-scanner REQUIRED IMPORTED_TARGET wayland-scanner)

include(${CMAKE_CURRENT_LIST_DIR}/WaylandProtocols.cmake)

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
