#include "shell_surface.h"

#include "display.h"
#include "seat.h"
#include "surface.h"

namespace wayland {

const struct wl_shell_surface_listener ShellSurface::listener_ = {
    OnPingThunk,
    OnConfigureThunk,
    OnPopupDoneThunk,
};

ShellSurface::ShellSurface(struct wl_shell_surface* shell_surface,
                           Delegate* delegate)
    : Proxy(shell_surface), delegate_(delegate) {
  wl_shell_surface_add_listener(id(), &listener_, this);
}

ShellSurface::~ShellSurface() { wl_shell_surface_destroy(id()); }

void ShellSurface::Pong(uint32_t serial) {
  wl_shell_surface_pong(id(), serial);
}

void ShellSurface::Move(Seat* seat, uint32_t serial) {
  wl_shell_surface_move(id(), seat->id(), serial);
}

void ShellSurface::Resize(Seat* seat, uint32_t serial, uint32_t edges) {
  wl_shell_surface_resize(id(), seat->id(), serial, edges);
}

void ShellSurface::SetToplevel() { wl_shell_surface_set_toplevel(id()); }

void ShellSurface::SetTransient(Surface* parent, int32_t x, int32_t y,
                                uint32_t flags) {
  wl_shell_surface_set_transient(id(), parent->id(), x, y, flags);
}

void ShellSurface::SetFullscreen(uint32_t method, uint32_t framerate,
                                 struct wl_output* output) {
  wl_shell_surface_set_fullscreen(id(), method, framerate, output);
}

void ShellSurface::SetPopup(Seat* seat, uint32_t serial, Surface* parent,
                            int32_t x, int32_t y, uint32_t flags) {
  wl_shell_surface_set_popup(id(), seat->id(), serial, parent->id(), x, y,
                             flags);
}

void ShellSurface::SetMaximized(struct wl_output* output) {
  wl_shell_surface_set_maximized(id(), output);
}

void ShellSurface::SetTitle(const char* title) {
  wl_shell_surface_set_title(id(), title);
}

void ShellSurface::SetClass(const char* class_) {
  wl_shell_surface_set_class(id(), class_);
}

void ShellSurface::OnPing(struct wl_shell_surface* shell_surface,
                          uint32_t serial) {
  Pong(serial);
}

void ShellSurface::OnConfigure(struct wl_shell_surface* shell_surface,
                               uint32_t edges, int32_t width, int32_t height) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
  if (delegate_) delegate_->OnConfigure(edges, width, height);
}

void ShellSurface::OnPopupDone(struct wl_shell_surface* shell_surface) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
  if (delegate_) delegate_->OnPopupDone();
}

}  // namespace wayland
