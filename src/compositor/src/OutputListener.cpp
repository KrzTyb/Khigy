
#include "compositor/OutputListener.hpp"

extern "C" {
#include <wlr/backend.h>
#include <wlr/types/wlr_output.h>
}

namespace {

void newOutputHandler(struct wl_listener *listener, void *data);

} // namespace

namespace compositor {

class OutputListenerImpl : public OutputListener {
public:
    struct Data {
        OutputListenerImpl *listenerInstance;
        struct wl_listener outputListener;
    };

    OutputListenerImpl();
    ~OutputListenerImpl() override;

    void listen(struct wlr_backend *backend,
                NewOutputCallback callback) override;

    void createOutput(wlr_output *newOutput);

private:
    bool m_listening = false;
    Data m_data;
    NewOutputCallback m_callback;
};

std::unique_ptr<OutputListener> OutputListener::create() {
    return std::make_unique<OutputListenerImpl>();
}

OutputListenerImpl::OutputListenerImpl() {
    m_data.listenerInstance = this;
    m_data.outputListener.notify = newOutputHandler;
}

OutputListenerImpl::~OutputListenerImpl() {
    if (m_listening) {
        wl_list_remove(&m_data.outputListener.link);
    }
    m_listening = false;
}

void OutputListenerImpl::listen(struct wlr_backend *backend,
                                NewOutputCallback callback) {

    if (m_listening) {
        wl_list_remove(&m_data.outputListener.link);
    }
    m_listening = true;
    m_callback = callback;
    wl_signal_add(&backend->events.new_output, &m_data.outputListener);
}

void OutputListenerImpl::createOutput(wlr_output *newOutput) {
    auto outputInstance = std::make_shared<OutputInstance>(newOutput);

    if (m_callback) {
        m_callback(std::move(outputInstance));
    }
}

} // namespace compositor

namespace {

void newOutputHandler(struct wl_listener *listener, void *data) {
    auto *newOutput = reinterpret_cast<wlr_output *>(data);

    compositor::OutputListenerImpl::Data *listenerDataPtr;
    listenerDataPtr =
        wl_container_of(listener, listenerDataPtr, outputListener);
    listenerDataPtr->listenerInstance->createOutput(newOutput);
}

} // namespace
