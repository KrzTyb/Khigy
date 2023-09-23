
# Build with unit tests
option(ENABLE_TESTS "Builds tests" OFF)

# Print wlroots logs
option(WLROOTS_LOGGER "Log wlroots traces" OFF)
if (${WLROOTS_LOGGER})
set(WLROOTS_LOGGER 1)
else()
set(WLROOTS_LOGGER 0)
endif()
