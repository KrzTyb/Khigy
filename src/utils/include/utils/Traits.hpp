#pragma once

#include <chrono>
#include <type_traits>

namespace traits {

class NonCopyable {
public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default; // Protected non-virtual destructor
};

} // namespace traits

template <class T>
concept ConvertibleToDuration =
    std::is_convertible_v<T, std::chrono::milliseconds>;

template <typename T>
concept CallableForTask = std::is_invocable_v<T>;
