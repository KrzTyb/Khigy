

function(_app_add_target)
    set(oneValueArgs TARGET_NAME)
    set(multiValueArgs INCLUDE_DIRS SOURCES DEPENDENCIES TESTS TARGET_TYPE LIB_TYPE)
    set(options "")
    cmake_parse_arguments(APP_ADD_TARGET_ARG "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

    if(${APP_ADD_TARGET_ARG_TARGET_TYPE} MATCHES EXECUTABLE)
        add_executable(${APP_ADD_TARGET_ARG_TARGET_NAME} ${APP_ADD_TARGET_ARG_SOURCES})
    elseif(${APP_ADD_TARGET_ARG_TARGET_TYPE} MATCHES LIBRARY)
        add_library(${APP_ADD_TARGET_ARG_TARGET_NAME} ${APP_ADD_TARGET_ARG_LIB_TYPE} ${APP_ADD_TARGET_ARG_SOURCES})
    else()
        message(FATAL_ERROR "Unknown TARGET_TYPE")
    endif()

    if(NOT "${APP_ADD_TARGET_ARG_INCLUDE_DIRS}" STREQUAL "")
        target_include_directories(${APP_ADD_TARGET_ARG_TARGET_NAME}
            ${APP_ADD_TARGET_ARG_INCLUDE_DIRS}
        )
    endif()

    target_link_libraries(${APP_ADD_TARGET_ARG_TARGET_NAME}
        ${APP_ADD_TARGET_ARG_DEPENDENCIES}
    )

    if(ENABLE_TESTS MATCHES ON AND NOT "${APP_ADD_TARGET_ARG_TESTS}" STREQUAL "")
        set(TEST_TARGET "Test${APP_ADD_TARGET_ARG_TARGET_NAME}")

        add_executable(${TEST_TARGET} ${APP_ADD_TARGET_ARG_TESTS})
        target_link_libraries(${TEST_TARGET} PRIVATE ${APP_ADD_TARGET_ARG_TARGET_NAME} GTest::gtest_main GTest::gmock)

        add_test(NAME ${TEST_TARGET}
            COMMAND qemu-aarch64 ${TEST_TARGET}
        )

    endif()
endfunction(_app_add_target)

function(app_add_library)
    _app_add_target(${ARGN} TARGET_TYPE LIBRARY)
endfunction(app_add_library)

function(app_add_executable)
    _app_add_target(${ARGN} TARGET_TYPE EXECUTABLE)
endfunction(app_add_executable)
