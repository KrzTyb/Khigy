#include "utils/Logger.hpp"

#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace logger {

void init() {
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(
        std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
    auto logger =
        std::make_shared<spdlog::logger>("Khigy", sinks.begin(), sinks.end());

    logger->set_level(spdlog::level::trace);
    spdlog::set_default_logger(logger);

    LOG_DEBUG("Logger inited");
}

void deinit() { spdlog::shutdown(); }

} // namespace logger
