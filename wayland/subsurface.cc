#include "subsurface.h"

namespace wayland {

Subsurface::Subsurface(struct wl_subsurface* subsurface) : Proxy(subsurface) {}

Subsurface::~Subsurface() { wl_subsurface_destroy(id()); }

}  // namespace wayland
