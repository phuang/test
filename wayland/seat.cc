#include "seat.h"

#include <stdio.h>

#include "keyboard.h"
#include "pointer.h"

namespace wayland {

const struct wl_seat_listener Seat::listener_ = {
    Seat::OnCapabilitiesThunk,
    Seat::OnNameThunk,
};

Seat::Seat(struct wl_seat* seat) : Proxy(seat) {
  wl_seat_add_listener(id(), &listener_, this);
}

Seat::~Seat() {
#if defined(WL_SEAT_RELEASE)
  wl_seat_release(id());
#endif
}

void Seat::OnCapabilities(struct wl_seat* seat, uint32_t caps) {
  fprintf(stderr, "EEE %s\n", __func__);
  if ((caps && WL_SEAT_CAPABILITY_KEYBOARD) && !keyboard_) {
    keyboard_.reset(new Keyboard(wl_seat_get_keyboard(id())));
  }
  if ((caps & WL_SEAT_CAPABILITY_POINTER) && !pointer_) {
    pointer_.reset(new Pointer(wl_seat_get_pointer(id())));
  }
}

void Seat::OnName(struct wl_seat* seat, const char* name) {
  fprintf(stderr, "EEE %s name=%s\n", __func__, name);
}

}  // namespace wayland
