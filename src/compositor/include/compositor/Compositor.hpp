#pragma once

#include <atomic>
#include <memory>
#include <unordered_map>

#include "allocator/Allocator.hpp"
#include "backend/Backend.hpp"
#include "compositor/OutputInstance.hpp"
#include "compositor/OutputListener.hpp"
#include "renderer/Renderer.hpp"
#include "utils/Traits.hpp"

#include <wayland-server-core.h>
extern "C" {
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_subcompositor.h>
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

    [[nodiscard]] std::shared_ptr<Compositor> build();

private:
    bool m_buildDefaultBackend = false;
    bool m_buildDefaultRenderer = false;
    bool m_buildDefaultAllocator = false;

    std::shared_ptr<backend::Backend> m_backend = nullptr;
    std::shared_ptr<renderer::Renderer> m_renderer = nullptr;
    std::shared_ptr<allocator::Allocator> m_allocator = nullptr;
};

class Compositor : private traits::NonCopyable,
                   public std::enable_shared_from_this<Compositor> {
public:
    static CompositorBuilder createBuilder();

    ~Compositor();

    void run();

    auto &outputInstances() { return m_outputs; }

    auto &backend() { return m_backend; }

    auto &renderer() { return m_renderer; }

    auto &allocator() { return m_allocator; }

private:
    friend class CompositorBuilder;
    Compositor(std::shared_ptr<backend::Backend> backend,
               std::shared_ptr<renderer::Renderer> renderer,
               std::shared_ptr<allocator::Allocator> allocator);
    void initialize();

    void processOutputEvent(OutputInstance::OutputEvent event,
                            const std::string &outputName);

    struct wl_display *m_displayHandle = nullptr;
    struct wlr_compositor *m_compositorHandle = nullptr;
    struct wlr_subcompositor *m_subcompositorHandle = nullptr;

    struct wl_listener m_newOutputListener;

    std::shared_ptr<backend::Backend> m_backend;
    std::shared_ptr<renderer::Renderer> m_renderer;
    std::shared_ptr<allocator::Allocator> m_allocator;

    std::unique_ptr<OutputListener> m_outputListener;

    std::unordered_map<std::string, std::shared_ptr<OutputInstance>> m_outputs;
};

} // namespace compositor
