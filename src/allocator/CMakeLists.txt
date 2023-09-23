

app_add_library(
    TARGET_NAME
        Allocator

    LIB_TYPE
        STATIC

    INCLUDE_DIRS
        PUBLIC
            include

    SOURCES
        src/Allocator.cpp

    DEPENDENCIES
        PUBLIC
            Utils
            PkgConfig::wlroots
)
