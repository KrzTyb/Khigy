

#include "utils/Logger.hpp"

#include "compositor/OutputInstance.hpp"
#include <array>

namespace {

void removeOutputHandler(struct wl_listener *listener, void *data);

void frameHandler(struct wl_listener *listener, void *data);

} // namespace

namespace compositor {

OutputInstance::OutputInstance(wlr_output *output) : m_output{output} {
    if (m_output == nullptr) {
        return;
    }
    m_data.outputInstance = this;

    wl_signal_add(&m_output->events.destroy, &m_data.destroyListener);
    m_data.destroyListener.notify = removeOutputHandler;

    wl_signal_add(&m_output->events.frame, &m_data.frameListener);
    m_data.frameListener.notify = frameHandler;

    struct wlr_output_mode *mode = wlr_output_preferred_mode(m_output);
    if (mode != nullptr) {
        wlr_output_set_mode(m_output, mode);
    }
}

OutputInstance::~OutputInstance() {
    if (m_output != nullptr) {
        wl_list_remove(&m_data.frameListener.link);
        wl_list_remove(&m_data.destroyListener.link);
    }
}

void OutputInstance::commit() {
    if (m_output != nullptr) {
        wlr_output_commit(m_output);
    }
}

void OutputInstance::attachRender(
    std::shared_ptr<renderer::Renderer> renderer,
    std::shared_ptr<allocator::Allocator> allocator) {

    m_renderer = std::move(renderer);
    m_allocator = std::move(allocator);

    if ((m_output != nullptr) && (m_renderer != nullptr) &&
        (m_allocator != nullptr)) {

        if (!wlr_output_init_render(m_output, m_allocator->rawPtr(),
                                    m_renderer->rawPtr())) {
            LOG_ERROR("Failed to initialize output rendering!");
        }
    } else {
        LOG_WARN("Null components");
    }
}

void OutputInstance::processFrame() {
    if ((m_output == nullptr) || (m_renderer == nullptr)) {
        return;
    }

    // TODO: Implement me
}

void OutputInstance::listenEvents(OutputEventCallback callback) {
    m_callback = std::move(callback);
}

std::optional<std::string> OutputInstance::name() const {

    std::optional<std::string> outputName;

    if (m_output != nullptr) {
        outputName = m_output->name;
    }

    return outputName;
}

void OutputInstance::destroy() {
    if (m_output != nullptr) {
        wl_list_remove(&m_data.frameListener.link);
        wl_list_remove(&m_data.destroyListener.link);
    }

    m_output = nullptr;

    if (m_callback) {
        m_callback(OutputEvent::Destroy);
    }
}

std::string OutputInstance::toString() const {
    auto stream = std::stringstream{};
    if (m_output != nullptr) {
        stream << "OutputInstance { name: " << m_output->name
               << ", width: " << m_output->width
               << ", height: " << m_output->height
               << ", enabled: " << m_output->enabled << " }";
    } else {
        stream << "OutputInstance { Invalid }";
    }

    return stream.str();
}

} // namespace compositor

namespace {

void removeOutputHandler(struct wl_listener *listener, void * /*data*/) {

    compositor::OutputInstance::Data *outputInstanceData;
    outputInstanceData =
        wl_container_of(listener, outputInstanceData, destroyListener);
    outputInstanceData->outputInstance->destroy();
}

void frameHandler(struct wl_listener *listener, void * /*data*/) {
    compositor::OutputInstance::Data *outputInstanceData;
    outputInstanceData =
        wl_container_of(listener, outputInstanceData, frameListener);

    outputInstanceData->outputInstance->processFrame();
}

} // namespace
