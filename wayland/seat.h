#ifndef __SEAT_H_
#define __SEAT_H_

#include <cstdint>
#include <memory>

#include <wayland-client.h>

#include "proxy.h"

namespace wayland {

class Keyboard;
class Pointer;

class Seat : public Proxy<struct wl_seat, Seat> {
 public:
  Seat(struct wl_seat* seat);
  ~Seat();

 private:
  // Seat event handlers:
  void OnCapabilities(struct wl_seat* seat,
                      uint32_t caps);
  void OnName(struct wl_seat* seat,
              const char* name);
  static void OnCapabilitiesThunk(void* data,
                                  struct wl_seat* seat,
                                  uint32_t caps) {
    static_cast<Seat*>(data)->OnCapabilities(seat, caps);
  }
  static void OnNameThunk(void* data,
                          struct wl_seat* seat,
                          const char* name) {
    static_cast<Seat*>(data)->OnName(seat, name);
  }

  std::unique_ptr<Keyboard> keyboard_;
  std::unique_ptr<Pointer> pointer_;

  static const struct wl_seat_listener listener_;
};

}  // namespace wayland

#endif
