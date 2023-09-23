
app_add_library(
    TARGET_NAME
        Utils

    LIB_TYPE
        STATIC

    INCLUDE_DIRS
        PUBLIC
            include

    SOURCES
        src/Logger.cpp

    DEPENDENCIES
        PUBLIC
            spdlog::spdlog
)
