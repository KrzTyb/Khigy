
#include "compositor/Compositor.hpp"
#include "allocator/Allocator.hpp"
#include "renderer/Renderer.hpp"
#include "utils/Exceptions.hpp"
#include "utils/Logger.hpp"

#include <cstdlib>
#include <memory>

namespace compositor {

CompositorBuilder Compositor::createBuilder() { return CompositorBuilder{}; }

std::unique_ptr<Compositor> CompositorBuilder::build() {

    if ((m_backend == nullptr) && m_buildDefaultBackend) {
        m_backend = backend::Backend::createDefault();
    }

    if ((m_renderer == nullptr) && m_buildDefaultRenderer) {
        m_renderer = renderer::Renderer::createDefault();
    }

    if ((m_allocator == nullptr) && m_buildDefaultAllocator) {
        m_allocator = allocator::Allocator::createDefault();
    }

    if ((m_backend == nullptr) || (m_renderer == nullptr) ||
        (m_allocator == nullptr)) {
        throw std::logic_error(
            "Trying to build a compositor with missing components!");
    }

    auto compositor = std::unique_ptr<Compositor>(new Compositor{
        std::move(m_backend), std::move(m_renderer), std::move(m_allocator)});
    compositor->initialize();

    return compositor;
}

Compositor::Compositor(std::shared_ptr<backend::Backend> backend,
                       std::shared_ptr<renderer::Renderer> renderer,
                       std::shared_ptr<allocator::Allocator> allocator)
    : m_backend{std::move(backend)}, m_renderer{std::move(renderer)},
      m_allocator{std::move(allocator)} {}

Compositor::~Compositor() {
    wl_display_destroy_clients(m_displayHandle);
    wl_display_destroy(m_displayHandle);
}

void Compositor::initialize() {

    try {
        m_displayHandle = wl_display_create();
        if (m_displayHandle == nullptr) {
            throw utils::CompositorError{
                "Failed to create compositor display!"};
        }
        m_backend->initialize(m_displayHandle);

        m_renderer->initialize(m_backend->rawPtr());

        if (!wlr_renderer_init_wl_display(m_renderer->rawPtr(),
                                          m_displayHandle)) {
            throw utils::CompositorError{
                "wlr_renderer_init_wl_display failed!"};
        }

        m_allocator->initialize(m_backend->rawPtr(), m_renderer->rawPtr());

        m_compositorHandle =
            wlr_compositor_create(m_displayHandle, m_renderer->rawPtr());
        if (m_compositorHandle == nullptr) {
            throw utils::CompositorError{"wlr_compositor_create failed!"};
        }

    } catch (const utils::CompositorError &error) {
        LOG_ERROR("Failed to initialize compositor. What: {}", error.what());
        wl_display_destroy_clients(m_displayHandle);
        wl_display_destroy(m_displayHandle);
        exit(1);
    }
}

void Compositor::run() { wl_display_run(m_displayHandle); }

} // namespace compositor
