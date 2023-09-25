
add_subdirectory(utils)
add_subdirectory(backend)
add_subdirectory(renderer)
add_subdirectory(allocator)
add_subdirectory(compositor)

app_add_executable(
    TARGET_NAME
        ${CMAKE_PROJECT_NAME}

    INCLUDE_DIRS

    SOURCES
        main.cpp

    DEPENDENCIES
        PRIVATE
            PkgConfig::wlroots
            PkgConfig::wayland-server
            Utils
            Compositor
)

protocol(${CMAKE_PROJECT_NAME} "stable/xdg-shell/xdg-shell.xml" "xdg-shell" false)
