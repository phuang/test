#ifndef __SHELL_SURFACE_H_
#define __SHELL_SURFACE_H_

#include <wayland-egl.h>

#include "proxy.h"

namespace wayland {

class Display;
class Seat;
class Surface;

class ShellSurface : public Proxy<struct wl_shell_surface, ShellSurface> {
 public:
  class Delegate {
   public:
    virtual void OnConfigure(uint32_t edges, int32_t width, int32_t height) {}
    virtual void OnPopupDone();
  };

  ShellSurface(struct wl_shell_surface* shell_surface,
               Delegate* delegate);
  ~ShellSurface();

  void Pong(uint32_t serial);
  void Move(Seat* seat, uint32_t serial);
  void Resize(Seat* seat, uint32_t serial, uint32_t edges);
  void SetToplevel();
  void SetTransient(Surface* parent, int32_t x, int32_t y, uint32_t flags);
  void SetFullscreen(uint32_t method, uint32_t framerate,
                     struct wl_output* output);
  void SetPopup(Seat* seat, uint32_t serial, Surface* parent, int32_t x,
                int32_t y, uint32_t flags);
  void SetMaximized(struct wl_output* output);
  void SetTitle(const char* title);
  void SetClass(const char* class_);

 private:
  // Surface events:
  void OnPing(struct wl_shell_surface* shell_surface,
              uint32_t serial);
  void OnConfigure(struct wl_shell_surface* shell_surface,
                   uint32_t edges,
                   int32_t width,
                   int32_t height);
  void OnPopupDone(struct wl_shell_surface* shell_surface);
  static void OnPingThunk(void* data,
                          struct wl_shell_surface* shell_surface,
                          uint32_t serial) {
    static_cast<ShellSurface*>(data)->OnPing(shell_surface, serial);
  }
  static void OnConfigureThunk(void* data,
                               struct wl_shell_surface* shell_surface,
                               uint32_t edges,
                               int32_t width,
                               int32_t height) {
    static_cast<ShellSurface*>(data)->OnConfigure(shell_surface,
                                                  edges,
                                                  width,
                                                  height);
  }
  static void OnPopupDoneThunk(void* data,
                               struct wl_shell_surface* shell_surface) {
    static_cast<ShellSurface*>(data)->OnPopupDone(shell_surface);
  }

  Delegate* delegate_;

  static const struct wl_shell_surface_listener listener_;
};

}  // namespace wayland

#endif
