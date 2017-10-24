#include "callback.h"

namespace wayland {

const struct wl_callback_listener Callback::listener_ = {
    Callback::OnDoneThunk,
};

Callback::Callback(struct wl_callback* callback) : Proxy(callback) {
  wl_callback_add_listener(id(), &listener_, this);
}

Callback::~Callback() {}

void Callback::OnDone(struct wl_callback* callback, uint32_t callback_data) {}

}  // namespace wayland
