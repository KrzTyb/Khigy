
#include <csignal>
#include <string>

#include "ProjectConfig.hpp"
#include "compositor/Compositor.hpp"
#include "utils/Logger.hpp"

extern "C" {
#include <wlr/util/log.h>
#include <wlr/version.h>
}

#if ENABLE_WLROOTS_LOGS
#define WLR_LOG_LEVEL WLR_INFO
#else
#define WLR_LOG_LEVEL WLR_ERROR
#endif

void wlr_log_callback(enum wlr_log_importance importance, const char *fmt,
                      va_list args) {
    if (importance > WLR_LOG_LEVEL) {
        return;
    }

    std::string message;
    va_list ap_copy;
    va_copy(ap_copy, args);
    size_t len = vsnprintf(nullptr, 0, fmt, ap_copy);
    message.resize(len + 1); // need space for NUL
    vsnprintf(message.data(), len + 1, fmt, args);
    message.resize(len); // remove the NUL

    switch (importance) {
    case WLR_ERROR:
        spdlog::error(message);
        break;
    case WLR_INFO:
        spdlog::info(message);
        break;
    case WLR_DEBUG:
        spdlog::debug(message);
        break;
    case WLR_SILENT:
        [[fallthrough]];
    default:
        break;
    }
}

void handleCriticalSignal(int sigNumber) {
    LOG_ERROR("Received signal: {}!", sigNumber);
    // TODO: Implement crash reporter
}

int main(int /*argc*/, char * /*argv*/[]) {
    logger::init();

    signal(SIGSEGV, handleCriticalSignal);
    signal(SIGABRT, handleCriticalSignal);

    if (std::getenv("XDG_RUNTIME_DIR") == nullptr) {
        LOG_ERROR("Missing XDG_RUNTIME_DIR environment!");
        return 1;
    }

    wlr_log_init(WLR_LOG_LEVEL, wlr_log_callback);

    LOG_INFO("wlroots version: {}", WLR_VERSION_STR);

    // Configure and build Khigy
    auto khigyCompositor = compositor::Compositor::createBuilder()
                               .withDefaultBackend()
                               .withDefaultRenderer()
                               .withDefaultAllocator()
                               .build();

    // Run Khigy
    khigyCompositor->run();

    logger::deinit();
    return 0;
}
