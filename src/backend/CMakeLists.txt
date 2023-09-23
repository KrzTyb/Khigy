

app_add_library(
    TARGET_NAME
        Backend

    LIB_TYPE
        STATIC

    INCLUDE_DIRS
        PUBLIC
            include

    SOURCES
        src/Backend.cpp

    DEPENDENCIES
        PUBLIC
            Utils
            PkgConfig::wlroots
)
