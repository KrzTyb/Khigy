#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>

template <typename T>
concept Loggable = requires(T const &object) {
                       {
                           object.toString()
                           } -> std::convertible_to<std::string_view>;
                   };

template <Loggable T>
std::ostream &operator<<(std::ostream &outStream, const T &object) {
    outStream << object.toString();
    return outStream;
}

template <Loggable T>
struct fmt::formatter<T> : fmt::formatter<std::string_view> {
    template <typename Context>
    constexpr auto format(const T &object, Context &context) const {
        return fmt::formatter<std::string_view>::format(object.toString(),
                                                        context);
    }
};

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
