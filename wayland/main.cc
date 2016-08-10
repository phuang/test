#include <GLES2/gl2.h>

#include "compositor.h"
#include "display.h"
#include "egl_window.h"
#include "shell.h"
#include "shell_surface.h"
#include "subcompositor.h"
#include "subsurface.h"
#include "surface.h"

void draw(int r, int g, int b, int a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}

int
main(int argc, char** argv) {
  std::unique_ptr<wayland::Display> display(new wayland::Display());
 
  // Create the toplevel surface
  auto surface = display->compositor()->CreateSurface();
  auto shell_surface = display->shell()->GetShellSurface(
      surface.get(), nullptr);
  shell_surface->SetToplevel();
  wayland::EGLWindow egl_window(surface.get(), 200, 200);
  egl_window.MakeCurrent();
  draw(1, 0, 0, 1);
  egl_window.SwapBuffers();

  auto surface2 = display->compositor()->CreateSurface();

  auto subsurface = display->subcompositor()->GetSubsurface(surface2.get(),
                                                            surface.get());
  subsurface->SetPosition(100, 100);
  wayland::EGLWindow egl_window2(surface2.get(), 50, 50);
  egl_window2.MakeCurrent();
  draw(0, 0, 1, 1);
  egl_window2.SwapBuffers();
  surface->Commit();

  display->Run();
}
