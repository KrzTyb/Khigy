

find_program(WaylandScanner NAMES wayland-scanner)
message(STATUS "Found WaylandScanner at ${WaylandScanner}")
execute_process(
    COMMAND pkg-config --variable=pkgdatadir wayland-protocols
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE WAYLAND_PROTOCOLS_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Found wayland-protocols at ${WAYLAND_PROTOCOLS_DIR}")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/protocols)
include_directories(${CMAKE_BINARY_DIR}/protocols)

function(protocol target protoPath protoName external)
    if (external)
        execute_process(
            COMMAND ${WaylandScanner} server-header ${protoPath} protocols/${protoName}-protocol.h
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        execute_process(
            COMMAND ${WaylandScanner} private-code ${protoPath} protocols/${protoName}-protocol.c
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})  
        target_sources(${target} PRIVATE ${CMAKE_BINARY_DIR}/protocols/${protoName}-protocol.c)
    else()
        execute_process(
            COMMAND ${WaylandScanner} server-header ${WAYLAND_PROTOCOLS_DIR}/${protoPath} protocols/${protoName}-protocol.h
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        execute_process(
            COMMAND ${WaylandScanner} private-code ${WAYLAND_PROTOCOLS_DIR}/${protoPath} protocols/${protoName}-protocol.c
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})  
        target_sources(${target} PRIVATE ${CMAKE_BINARY_DIR}/protocols/${protoName}-protocol.c)
    endif()
endfunction()

