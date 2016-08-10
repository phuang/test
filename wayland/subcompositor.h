#ifndef __SUBCOMPOSITOR_H_
#define __SUBCOMPOSITOR_H_

#include <memory>
#include <wayland-client.h>

#include "proxy.h"

namespace wayland {

class Subsurface;
class Surface;

class Subcompositor : public Proxy<struct wl_subcompositor, Subcompositor> {
 public:
  explicit Subcompositor(struct wl_subcompositor* subcompositor);
  ~Subcompositor();

  std::unique_ptr<Subsurface> GetSubsurface(Surface* surface, Surface* parent);
};

}  // namespace wayland
#endif
