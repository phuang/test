#ifndef __SURFACE_H_
#define __SURFACE_H_

#include <wayland-client.h>
#include <wayland-egl.h>

#include "proxy.h"

namespace wayland {

class Surface : public Proxy<struct wl_surface, Surface> {
 public:
  explicit Surface(struct wl_surface* surface);
  ~Surface();

  void Attach(struct wl_buffer* buffer, int32_t x, int32_t y) {
    wl_surface_attach(id(), buffer, x, y);
  }
  void Damage(int32_t x, int32_t y, int32_t width, int32_t height) {
    wl_surface_damage(id(), x, y, width, height);
  }
  struct wl_callback* Frame() {
    return wl_surface_frame(id());
  }
  void SetOpaqueRegion(struct wl_region* region) {
    wl_surface_set_opaque_region(id(), region);
  }
  void SetInputRegion(struct wl_region* region) {
    wl_surface_set_input_region(id(), region);
  }
  void Commit() {
    wl_surface_commit(id());
  }
  void SetBufferTransform(int32_t transform) {
    wl_surface_set_buffer_transform(id(), transform);
  }
  void SetBufferScale(int32_t scale) {
    wl_surface_set_buffer_scale(id(), scale);
  }
#if defined(WL_SURFACE_DAMAGE_BUFFER)
  void DamageBuffer(int32_t x, int32_t y, int32_t width, int32_t height) {
    wl_surface_damage_buffer(id(), x, y, width, height);
  }
#endif
  
 private:
  void OnEntry(struct wl_surface* surface,
               struct wl_output* output);
  void OnLeave(struct wl_surface* surface,
               struct wl_output* output);
  static void OnEntryThunk(void* data,
                           struct wl_surface* surface,
                           struct wl_output* output) {
    static_cast<Surface*>(data)->OnEntry(surface, output);
  }
  static void OnLeaveThunk(void* data,
                           struct wl_surface* surface,
                           struct wl_output* output) {
    static_cast<Surface*>(data)->OnLeave(surface, output);
  }

  struct wl_egl_window* egl_window_;

  const static wl_surface_listener surface_listener_;
};

}

#endif
