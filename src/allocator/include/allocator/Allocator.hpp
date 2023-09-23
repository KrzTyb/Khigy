#pragma once

#include <memory>

extern "C" {
#include <wlr/backend.h>
#include <wlr/render/allocator.h>

#define static
#include <wlr/render/wlr_renderer.h>
#undef static
}

namespace allocator {

class Allocator {
public:
    static std::shared_ptr<Allocator> createDefault();

    virtual ~Allocator() = default;

    virtual void initialize(struct wlr_backend *backendHandle,
                            struct wlr_renderer *rendererHandle) = 0;

    [[nodiscard]] virtual struct wlr_allocator *rawPtr() const = 0;

protected:
    Allocator() = default;
};

} // namespace allocator
