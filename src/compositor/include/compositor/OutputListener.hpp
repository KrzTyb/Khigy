#pragma once

#include "compositor/OutputInstance.hpp"

#include <functional>
#include <memory>

#include <wayland-server-core.h>

extern "C" {
#include <wlr/backend.h>
}

namespace compositor {

class OutputListener {

public:
    using NewOutputCallback =
        std::function<void(std::shared_ptr<OutputInstance>)>;

    static std::unique_ptr<OutputListener> create();
    virtual ~OutputListener() = default;

    virtual void listen(struct wlr_backend *backend,
                        NewOutputCallback callback) = 0;

protected:
    OutputListener() = default;
};

} // namespace compositor
