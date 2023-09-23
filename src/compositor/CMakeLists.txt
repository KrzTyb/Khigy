
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

    DEPENDENCIES
        PUBLIC
            Utils
            Backend
            Renderer
            Allocator
            PkgConfig::wlroots
            PkgConfig::wayland-server
)
