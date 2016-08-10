#include "registry.h"

namespace wayland {

const struct wl_registry_listener Registry::listener_ = {
  Registry::OnGlobalThunk,
  Registry::OnGlobalRemoveThunk,
};

Registry::Registry(struct wl_registry* registry,
                   Delegate* delegate)
  : Proxy(registry),
    delegate_(delegate) {
  wl_registry_add_listener(id(), &listener_, this);
}

Registry::~Registry() {
  wl_registry_destroy(id());
}

void Registry::OnGlobal(struct wl_registry* registry, uint32_t id,
                        const char* interface, uint32_t version) {
  delegate_->OnGlobal(id, interface, version);
}

void Registry::OnGlobalRemove(struct wl_registry* registry, uint32_t id) {
  delegate_->OnGlobalRemove(id);
}

}  // namespace wayland
