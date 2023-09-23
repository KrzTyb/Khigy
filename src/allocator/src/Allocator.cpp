#include "allocator/Allocator.hpp"

#include "utils/Exceptions.hpp"

namespace allocator {

class DefaultAllocator : public Allocator {
public:
    DefaultAllocator();

    ~DefaultAllocator() override = default;

    void initialize(struct wlr_backend *backendHandle,
                    struct wlr_renderer *rendererHandle) override;

    [[nodiscard]] struct wlr_allocator *rawPtr() const override {
        return m_allocatorHandle;
    }

private:
    struct wlr_allocator *m_allocatorHandle;
};

std::shared_ptr<Allocator> Allocator::createDefault() {

    return std::make_shared<DefaultAllocator>();
}

DefaultAllocator::DefaultAllocator() = default;

void DefaultAllocator::initialize(struct wlr_backend *backendHandle,
                                  struct wlr_renderer *rendererHandle) {
    m_allocatorHandle = wlr_allocator_autocreate(backendHandle, rendererHandle);
    if (m_allocatorHandle == nullptr) {
        throw utils::CompositorError{"Failed to create allocator!"};
    }
}

} // namespace allocator
