#ifndef __EGL_WINDOW_
#define __EGL_WINDOW_

#include <wayland-client.h>
#include <wayland-egl.h>

#include "display.h"
#include "surface.h"

namespace wayland {

class Surface;

class EGLWindow {
 public:
  EGLWindow(Surface* surface, int width, int height) : surface_(surface) {
    egl_window_ = wl_egl_window_create(surface_->id(), width, height);
    egl_surface_ = eglCreateWindowSurface(Display::current()->egl_display(),
                                          Display::current()->egl_config(),
                                          egl_window_, nullptr);
  }
  ~EGLWindow() {
    wl_egl_window_destroy(egl_window_);
  }
  void Resize(int width, int height, int dx, int dy) {
    wl_egl_window_resize(egl_window_, width, height, dx, dy);
  }
  void MakeCurrent() {
    eglMakeCurrent(Display::current()->egl_display(), egl_surface_,
                   egl_surface_, Display::current()->egl_context());
  }
  void SwapBuffers() {
    eglSwapBuffers(Display::current()->egl_display(), egl_surface_);
  }

 private:
  Surface* surface_;
  struct wl_egl_window* egl_window_ = nullptr;
  EGLSurface egl_surface_ = nullptr;
};

}  // namespace wayland

#endif
