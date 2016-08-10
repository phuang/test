#include "pointer.h"

#include <stdio.h>

namespace wayland {

const struct wl_pointer_listener Pointer::listener_ = {
  Pointer::OnEnterThunk,
  Pointer::OnLeaveThunk,
  Pointer::OnMotionThunk,
  Pointer::OnButtonThunk,
  Pointer::OnAxisThunk,
  Pointer::OnFrameThunk,
  Pointer::OnAxisSourceThunk,
  Pointer::OnAxisStopThunk,
};

Pointer::Pointer(struct wl_pointer* pointer)
  : Proxy(pointer) {
    wl_pointer_add_listener(id(), &listener_, this);
}

Pointer::~Pointer() {
  wl_pointer_release(id());
}

void Pointer::OnEnter(struct wl_pointer* pointer,
                      uint32_t serial,
                      struct wl_surface* surface,
                      wl_fixed_t surface_x,
                      wl_fixed_t surface_y) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Pointer::OnLeave(struct wl_pointer* pointer,
                      uint32_t serial,
                      struct wl_surface* surface) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Pointer::OnMotion(struct wl_pointer* pointer,
                       uint32_t time,
                       wl_fixed_t surface_x,
                       wl_fixed_t surface_y) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Pointer::OnButton(struct wl_pointer* pointer,
                       uint32_t serial,
                       uint32_t time,
                       uint32_t button,
                       uint32_t state) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Pointer::OnAxis(struct wl_pointer* pointer,
                     uint32_t time,
                     uint32_t axis,
                     wl_fixed_t value) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Pointer::OnFrame(struct wl_pointer* pointer) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Pointer::OnAxisSource(struct wl_pointer* pointer, uint32_t axis_source) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Pointer::OnAxisStop(struct wl_pointer* pointer, uint32_t time,
                         uint32_t axis) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

}   // namespace wayland
