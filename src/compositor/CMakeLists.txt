
app_add_library(
    TARGET_NAME
        Compositor

    LIB_TYPE
        STATIC

    INCLUDE_DIRS
        PUBLIC
            include

    SOURCES
        src/Compositor.cpp
        src/OutputListener.cpp
        src/OutputInstance.cpp

    DEPENDENCIES
        PUBLIC
            Utils
            Backend
            Renderer
            Allocator
            PkgConfig::wlroots
            PkgConfig::wayland-server
            PkgConfig::wayland-protocols
)
