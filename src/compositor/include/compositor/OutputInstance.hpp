#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <sstream>
#include <string>

#include "utils/Traits.hpp"

#include "allocator/Allocator.hpp"
#include "renderer/Renderer.hpp"

extern "C" {
#include <wlr/types/wlr_output.h>
}

namespace compositor {

class OutputInstance : private traits::NonCopyable {
public:
    struct Data {
        OutputInstance *outputInstance;
        struct wl_listener destroyListener;
        struct wl_listener frameListener;
    };

    enum class OutputEvent { Destroy, Unknown };

    using OutputEventCallback = std::function<void(OutputEvent)>;

    explicit OutputInstance(wlr_output *output);
    ~OutputInstance();

    void commit();

    void attachRender(std::shared_ptr<renderer::Renderer> renderer,
                      std::shared_ptr<allocator::Allocator> allocator);

    void processFrame();

    void listenEvents(OutputEventCallback callback);

    [[nodiscard]] std::optional<std::string> name() const;

    void destroy();

    [[nodiscard]] std::string toString() const;

private:
    wlr_output *m_output = nullptr;
    Data m_data;

    std::shared_ptr<renderer::Renderer> m_renderer;
    std::shared_ptr<allocator::Allocator> m_allocator;

    OutputEventCallback m_callback;
};

} // namespace compositor
