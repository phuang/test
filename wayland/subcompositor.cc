#include "subcompositor.h"

#include "subsurface.h"
#include "surface.h"

namespace wayland {

Subcompositor::Subcompositor(struct wl_subcompositor* subcompositor)
  : Proxy(subcompositor) {
}

Subcompositor::~Subcompositor() {
  wl_subcompositor_destroy(id());
}

std::unique_ptr<Subsurface> Subcompositor::GetSubsurface(
    Surface* surface, Surface* parent) {
  struct wl_subsurface* subsurface = wl_subcompositor_get_subsurface(
      id(), surface->id(), parent->id());
  if (!subsurface)
    return nullptr;
  return std::unique_ptr<Subsurface>(new Subsurface(subsurface));
}

}  // namespace wayland
