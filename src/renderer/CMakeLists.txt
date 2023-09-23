

app_add_library(
    TARGET_NAME
        Renderer

    LIB_TYPE
        STATIC

    INCLUDE_DIRS
        PUBLIC
            include

    SOURCES
        src/Renderer.cpp

    DEPENDENCIES
        PUBLIC
            Utils
            PkgConfig::wlroots
)
