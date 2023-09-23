#pragma once

#include <atomic>
#include <memory>

#include "allocator/Allocator.hpp"
#include "backend/Backend.hpp"
#include "renderer/Renderer.hpp"
#include "utils/Traits.hpp"

#include <wayland-server-core.h>
extern "C" {
#include <wlr/types/wlr_compositor.h>
}

namespace compositor {

class Compositor;

class CompositorBuilder : private traits::NonCopyable {
public:
    CompositorBuilder() = default;
    ~CompositorBuilder() = default;

    CompositorBuilder &withDefaultBackend() {
        m_buildDefaultBackend = true;
        return *this;
    }

    CompositorBuilder &withDefaultRenderer() {
        m_buildDefaultRenderer = true;
        return *this;
    }

    CompositorBuilder &withDefaultAllocator() {
        m_buildDefaultAllocator = true;
        return *this;
    }

    [[nodiscard]] std::unique_ptr<Compositor> build();

private:
    bool m_buildDefaultBackend = false;
    bool m_buildDefaultRenderer = false;
    bool m_buildDefaultAllocator = false;

    std::shared_ptr<backend::Backend> m_backend = nullptr;
    std::shared_ptr<renderer::Renderer> m_renderer = nullptr;
    std::shared_ptr<allocator::Allocator> m_allocator = nullptr;
};

class Compositor : private traits::NonCopyable {
public:
    static CompositorBuilder createBuilder();

    ~Compositor();

    void run();

private:
    friend class CompositorBuilder;
    Compositor(std::shared_ptr<backend::Backend> backend,
               std::shared_ptr<renderer::Renderer> renderer,
               std::shared_ptr<allocator::Allocator> allocator);
    void initialize();

    struct wl_display *m_displayHandle = nullptr;
    struct wlr_compositor *m_compositorHandle = nullptr;
    std::shared_ptr<backend::Backend> m_backend;
    std::shared_ptr<renderer::Renderer> m_renderer;
    std::shared_ptr<allocator::Allocator> m_allocator;
};

} // namespace compositor
