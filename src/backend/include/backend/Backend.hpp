#pragma once

#include <memory>
#include <wayland-server-core.h>

extern "C" {
#include <wlr/backend.h>
}

namespace backend {

class Backend {
public:
    static std::shared_ptr<Backend> createDefault();

    virtual ~Backend() = default;

    virtual void initialize(struct wl_display *displayHandle) = 0;

    [[nodiscard]] virtual struct wlr_backend *rawPtr() const = 0;

protected:
    Backend() = default;
};

} // namespace backend
