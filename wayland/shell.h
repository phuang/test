#ifndef __SHELL_H_
#define __SHELL_H_

#include <memory>

#include <wayland-client.h>

#include "proxy.h"
#include "shell_surface.h"

namespace wayland {

class Surface;

class Shell : public Proxy<struct wl_shell, Shell> {
 public:
  Shell(struct wl_shell* shell);
  ~Shell();

  std::unique_ptr<ShellSurface> GetShellSurface(
      Surface* surface, ShellSurface::Delegate* delegate);
};

}

#endif
