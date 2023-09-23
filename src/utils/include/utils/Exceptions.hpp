#pragma once

#include <exception>
#include <stdexcept>

namespace utils {

class CompositorError : public std::exception {
public:
    explicit CompositorError(const char *message) : m_message(message) {}

    [[nodiscard]] const char *what() const noexcept override {
        return m_message;
    }

private:
    const char *m_message;
};

} // namespace utils
