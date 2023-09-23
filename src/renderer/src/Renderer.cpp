#include "renderer/Renderer.hpp"

#include "utils/Exceptions.hpp"

namespace renderer {

class DefaultRenderer : public Renderer {
public:
    DefaultRenderer();

    ~DefaultRenderer() override = default;

    void initialize(struct wlr_backend *backendHandle) override;

    [[nodiscard]] struct wlr_renderer *rawPtr() const override {
        return m_rendererHandle;
    }

private:
    struct wlr_renderer *m_rendererHandle;
};

std::shared_ptr<Renderer> Renderer::createDefault() {

    return std::make_shared<DefaultRenderer>();
}

DefaultRenderer::DefaultRenderer() = default;

void DefaultRenderer::initialize(struct wlr_backend *backendHandle) {
    m_rendererHandle = wlr_renderer_autocreate(backendHandle);
    if (m_rendererHandle == nullptr) {
        throw utils::CompositorError{"Failed to create renderer!"};
    }
}

} // namespace renderer
