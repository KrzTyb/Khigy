#include "backend/Backend.hpp"

#include <memory>

#include "utils/Exceptions.hpp"

namespace backend {

class DefaultBackend : public Backend {
public:
    DefaultBackend();

    ~DefaultBackend() override = default;

    void initialize(struct wl_display *displayHandle) override;

    bool start() override;

    [[nodiscard]] struct wlr_backend *rawPtr() const override {
        return m_backendHandle;
    }

private:
    struct wlr_backend *m_backendHandle = nullptr;
};

std::shared_ptr<Backend> Backend::createDefault() {

    return std::make_shared<DefaultBackend>();
}

DefaultBackend::DefaultBackend() = default;

void DefaultBackend::initialize(struct wl_display *displayHandle) {
    m_backendHandle = wlr_backend_autocreate(displayHandle);
    if (m_backendHandle == nullptr) {
        throw utils::CompositorError{"Failed to create backend!"};
    }
}

bool DefaultBackend::start() { return wlr_backend_start(m_backendHandle); }

} // namespace backend
