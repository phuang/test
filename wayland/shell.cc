#include "shell.h"

#include "shell_surface.h"
#include "surface.h"

namespace wayland {

Shell::Shell(struct wl_shell* shell) : Proxy(shell) { }

Shell::~Shell() {
  wl_shell_destroy(id());
}

std::unique_ptr<ShellSurface> Shell::GetShellSurface(
    Surface* surface, ShellSurface::Delegate* delegate) {
  struct wl_shell_surface* shell_surface =
      wl_shell_get_shell_surface(id(), surface->id());
  if (!shell_surface)
    return nullptr;
  return std::unique_ptr<ShellSurface>(
      new ShellSurface(shell_surface, delegate));
}

}  // namespace wayland
