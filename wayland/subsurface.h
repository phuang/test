#ifndef __SUBSURFACE_H_
#define __SUBSURFACE_H_

#include <wayland-client.h>

#include "proxy.h"
#include "surface.h"

namespace wayland {

class Surface;

class Subsurface : public Proxy<struct wl_subsurface, Subsurface> {
 public:
  explicit Subsurface(struct wl_subsurface* subsurface);
  ~Subsurface();

  void SetPosition(int32_t x, int32_t y) {
    wl_subsurface_set_position(id(), x, y);
  }

  void PlaceAbove(Surface* sibling) {
    wl_subsurface_place_above(id(), sibling->id());
  }

  void PlaceBelow(Surface* sibling) {
    wl_subsurface_place_below(id(), sibling->id());
  }

  void SetSync() {
    wl_subsurface_set_sync(id());
  }

  void SetDesync() {
    wl_subsurface_set_desync(id());
  }

};

}  // namespace wayland


#endif
