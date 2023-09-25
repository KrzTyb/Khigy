
#include "compositor/Compositor.hpp"
#include "allocator/Allocator.hpp"
#include "compositor/OutputInstance.hpp"
#include "compositor/OutputListener.hpp"
#include "renderer/Renderer.hpp"
#include "utils/Exceptions.hpp"
#include "utils/Logger.hpp"

#include <csignal>
#include <cstdlib>
#include <memory>
#include <wayland-server-core.h>

namespace {

int handleSignal(int signalCode, void *data) {
    if (signalCode == SIGTERM || signalCode == SIGINT) {
        LOG_INFO("Received signal {}", signalCode);
        auto *displayHandle = reinterpret_cast<struct wl_display *>(data);
        wl_display_destroy_clients(displayHandle);
        wl_display_terminate(displayHandle);
    }
    return 0;
}

} // namespace

namespace compositor {

CompositorBuilder Compositor::createBuilder() { return CompositorBuilder{}; }

std::shared_ptr<Compositor> CompositorBuilder::build() {

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

    auto compositor = std::shared_ptr<Compositor>(new Compositor{
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

        auto *eventLoop = wl_display_get_event_loop(m_displayHandle);

        // Register signals
        wl_event_loop_add_signal(eventLoop, SIGINT, handleSignal,
                                 m_displayHandle);
        wl_event_loop_add_signal(eventLoop, SIGTERM, handleSignal,
                                 m_displayHandle);

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

        m_subcompositorHandle = wlr_subcompositor_create(m_displayHandle);
        if (m_subcompositorHandle == nullptr) {
            throw utils::CompositorError{"wlr_subcompositor_create failed!"};
        }

        auto *data_device_manager =
            wlr_data_device_manager_create(m_displayHandle);
        if (data_device_manager == nullptr) {
            throw utils::CompositorError{
                "wlr_data_device_manager_create failed!"};
        }

        m_outputListener = OutputListener::create();
        m_outputListener->listen(m_backend->rawPtr(), [compositorWeakPtr =
                                                           weak_from_this()](
                                                          auto objectInstance) {
            if (auto compositorPtr = compositorWeakPtr.lock();
                (objectInstance != nullptr) && (compositorPtr != nullptr)) {
                LOG_INFO("Created output: {}", *objectInstance);

                const auto instanceName = objectInstance->name();
                if (instanceName) {

                    objectInstance->attachRender(compositorPtr->renderer(),
                                                 compositorPtr->allocator());

                    objectInstance->listenEvents(
                        [name = instanceName.value(),
                         compositorWeakPtr](auto event) {
                            if (auto compositorPtr = compositorWeakPtr.lock();
                                compositorPtr != nullptr) {
                                compositorPtr->processOutputEvent(event, name);
                            }
                        });
                    objectInstance->commit();
                    compositorPtr->outputInstances()[instanceName.value()] =
                        std::move(objectInstance);
                }
            }
        });

    } catch (const utils::CompositorError &error) {
        LOG_ERROR("Failed to initialize compositor. What: {}", error.what());
        wl_display_destroy(m_displayHandle);
        exit(1);
    }
}

void Compositor::processOutputEvent(OutputInstance::OutputEvent event,
                                    const std::string &outputName) {
    if (event == OutputInstance::OutputEvent::Destroy) {
        LOG_INFO("Destroyed output: {}", outputName);
        m_outputs.erase(outputName);
    }
}

void Compositor::run() {

    if (!m_backend->start()) {
        LOG_ERROR("Failed to start backend!");
        wl_display_destroy(m_displayHandle);
        exit(1);
    }

    wl_display_run(m_displayHandle);
}

} // namespace compositor
