#ifndef __POINTER_H_
#define __POINTER_H_

#include <cstdint>

#include <wayland-client.h>

#include "proxy.h"

namespace wayland {

class Pointer : public Proxy<struct wl_pointer, Pointer> {
 public:
  Pointer(struct wl_pointer* pointer);
  ~Pointer();

 private:
  // Pointer events:
  void OnEnter(struct wl_pointer* pointer,
               uint32_t serial,
               struct wl_surface* surface,
               wl_fixed_t surface_x,
               wl_fixed_t surface_y);
  void OnLeave(struct wl_pointer* pointer,
               uint32_t serial,
               struct wl_surface* surface);
  void OnMotion(struct wl_pointer* pointer,
                uint32_t time,
                wl_fixed_t surface_x,
                wl_fixed_t surface_y);
  void OnButton(struct wl_pointer* pointer,
                uint32_t serial,
                uint32_t time,
                uint32_t button,
                uint32_t state);
  void OnAxis(struct wl_pointer* pointer,
              uint32_t time,
              uint32_t axis,
              wl_fixed_t value);
  void OnFrame(struct wl_pointer* pointer);
  void OnAxisSource(struct wl_pointer* pointer,
                    uint32_t axis_source);
  void OnAxisStop(struct wl_pointer* pointer,
                  uint32_t time,
                  uint32_t axis);
  static void OnEnterThunk(void* data,
                           struct wl_pointer* pointer,
                           uint32_t serial,
                           struct wl_surface* surface,
                           wl_fixed_t surface_x,
                           wl_fixed_t surface_y) {
    static_cast<Pointer*>(data)->OnEnter(
        pointer, serial, surface, surface_x, surface_y);
  }
  static void OnLeaveThunk(void* data,
                           struct wl_pointer* pointer,
                           uint32_t serial,
                           struct wl_surface* surface) {
    static_cast<Pointer*>(data)->OnLeave(pointer, serial, surface);
  }
  static void OnMotionThunk(void* data,
                            struct wl_pointer* pointer,
                            uint32_t time,
                            wl_fixed_t surface_x,
                            wl_fixed_t surface_y) {
    static_cast<Pointer*>(data)->OnMotion(pointer, time, surface_x, surface_y);
  }
  static void OnButtonThunk(void* data,
                            struct wl_pointer* pointer,
                            uint32_t serial,
                            uint32_t time,
                            uint32_t button,
                            uint32_t state) {
    static_cast<Pointer*>(data)->OnButton(pointer, serial, time, button, state);
  }
  static void OnAxisThunk(void* data,
                          struct wl_pointer* pointer,
                          uint32_t time,
                          uint32_t axis,
                          wl_fixed_t value) {
    static_cast<Pointer*>(data)->OnAxis(pointer, time, axis, value);
  }
  static void OnFrameThunk(void* data, struct wl_pointer* pointer) {
    static_cast<Pointer*>(data)->OnFrame(pointer);
  }
  static void OnAxisSourceThunk(void* data, struct wl_pointer* pointer,
                                uint32_t axis_source) {
    static_cast<Pointer*>(data)->OnAxisSource(pointer, axis_source);
  }
  static void OnAxisStopThunk(void* data, struct wl_pointer* pointer,
                              uint32_t time, uint32_t axis) {
    static_cast<Pointer*>(data)->OnAxisStop(pointer, time, axis);
  }

  static const struct wl_pointer_listener listener_;
};

}  // namespace wayland

#endif
