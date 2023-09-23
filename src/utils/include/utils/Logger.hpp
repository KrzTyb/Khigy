#pragma once

#include <sstream>
#include <string>
#include <utility>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>

namespace logger {

void init();

void deinit();

} // namespace logger

#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)

#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)

#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)

#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)

#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
