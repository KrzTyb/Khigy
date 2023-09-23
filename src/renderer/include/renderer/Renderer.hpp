#pragma once

#include <memory>

extern "C" {
#include <wlr/backend.h>

#define static
#include <wlr/render/wlr_renderer.h>
#undef static
}

namespace renderer {

class Renderer {
public:
    static std::shared_ptr<Renderer> createDefault();

    virtual ~Renderer() = default;

    virtual void initialize(struct wlr_backend *backendHandle) = 0;

    [[nodiscard]] virtual struct wlr_renderer *rawPtr() const = 0;

protected:
    Renderer() = default;
};

} // namespace renderer
