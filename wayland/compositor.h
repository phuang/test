#ifndef __COMPOSITOR_H_
#define __COMPOSITOR_H_

#include <memory>
#include <wayland-client.h>

#include "proxy.h"

namespace wayland {

class Region;
class Surface;

class Compositor : public Proxy<struct wl_compositor, Compositor> {
 public:
  explicit Compositor(struct wl_compositor* compositor);
  ~Compositor();

  std::unique_ptr<Surface> CreateSurface();
  std::unique_ptr<Region> CreateRegion();
};

}  // namespace wayland
#endif
