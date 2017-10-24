#include "display.h"

#include <cassert>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string.h>
#include <wayland-egl.h>

#include "compositor.h"
#include "registry.h"
#include "seat.h"
#include "shell.h"
#include "subcompositor.h"

namespace wayland {

namespace {
Display* g_instance_ = nullptr;
}

const struct wl_shm_listener Display::shm_listener_ = {
    Display::OnShmFormatThunk,
};

Display::Display() : Proxy(wl_display_connect(nullptr)) {
  assert(!g_instance_);
  g_instance_ = this;

  if (!id()) {
    fprintf(stderr, "can't connect to display\n");
    exit(EXIT_FAILURE);
  }

  registry_.reset(new Registry(wl_display_get_registry(id()), this));

  wl_display_dispatch(id());
  wl_display_roundtrip(id());

  InitEGL();
}

Display::~Display() {
  assert(g_instance_ == this);
  g_instance_ = nullptr;

  if (shm_) wl_shm_destroy(shm_);

  if (egl_context_) eglDestroyContext(egl_display_, egl_context_);

  if (egl_display_) eglTerminate(egl_display_);
}

// static
Display* Display::current() { return g_instance_; }

void Display::InitEGL() {
  EGLint major, minor;
  EGLint n;

  static const EGLint argb_cfg_attribs[] = {EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                            EGL_RED_SIZE,     1,
                                            EGL_GREEN_SIZE,   1,
                                            EGL_BLUE_SIZE,    1,
                                            EGL_ALPHA_SIZE,   1,
                                            EGL_DEPTH_SIZE,   1,
                                            EGL_NONE};

  EGLint* context_attribs = NULL;
  EGLint api = EGL_OPENGL_ES_API;

  egl_display_ = eglGetDisplay(id());
  if (!eglInitialize(egl_display_, &major, &minor)) {
    fprintf(stderr, "failed to initialize EGL\n");
    exit(EXIT_FAILURE);
  }

  if (!eglBindAPI(api)) {
    fprintf(stderr, "failed to bind EGL client API\n");
    exit(EXIT_FAILURE);
  }

  if (!eglChooseConfig(egl_display_, argb_cfg_attribs, &egl_config_, 1, &n) ||
      n != 1) {
    fprintf(stderr, "failed to choose argb EGL config\n");
    exit(EXIT_FAILURE);
  }

  egl_context_ = eglCreateContext(egl_display_, egl_config_, EGL_NO_CONTEXT,
                                  context_attribs);
  if (!egl_context_) {
    fprintf(stderr, "failed to create EGL context\n");
    exit(EXIT_FAILURE);
  }
}

void Display::Run() {
  while (1) {
    int ret = wl_display_dispatch(id());
    if (ret == -1) break;
#if 0
    int ret = wl_display_flush(id());
    struct epoll_event ep[16];
    if (ret < 0 && errno == EAGAIN) {
      ep[0].events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
      ep[0].data.ptr =  nullptr;
      epoll_ctrl(epoll_fd_, EPOLL_CTL_MOD, display_fd_, &ep[0]);
    } else if (ret < 0) {
      break;
    }
    int count =  epoll_wait(epoll_fd_, ep, 16, -1);
    for (int i = 0; i < count; ++i) {
    }
#endif
  }
}

void Display::OnGlobal(uint32_t id, const char* interface, uint32_t version) {
  printf("Got a registry event for id %d interface %s\n", id, interface);
  if (strcmp(interface, "wl_compositor") == 0) {
    auto compositor =
        registry_->Bind<struct wl_compositor>(id, &wl_compositor_interface, 3);
    compositor_.reset(new Compositor(compositor));
  } else if (strcmp(interface, "wl_output") == 0) {
    auto output =
        registry_->Bind<struct wl_output>(id, &wl_output_interface, 2);
  } else if (strcmp(interface, "wl_seat") == 0) {
    seat_version_ = version;
    auto seat = registry_->Bind<struct wl_seat>(id, &wl_seat_interface,
                                                std::max(seat_version_, 3u));
    std::unique_ptr<Seat> input(new Seat(seat));
    seats_.push_back(std::move(input));
  } else if (strcmp(interface, "wl_shell") == 0) {
    auto shell = registry_->Bind<struct wl_shell>(id, &wl_shell_interface, 1);
    shell_.reset(new Shell(shell));
  } else if (strcmp(interface, "wl_shm") == 0) {
    auto shm_ = registry_->Bind<struct wl_shm>(id, &wl_shm_interface, 1);
    wl_shm_add_listener(shm_, &shm_listener_, this);
  } else if (strcmp(interface, "workspace_manager") == 0) {
  } else if (strcmp(interface, "wl_subcompositor") == 0) {
    auto subcompositor = registry_->Bind<struct wl_subcompositor>(
        id, &wl_subcompositor_interface, 1);
    subcompositor_.reset(new Subcompositor(subcompositor));
  }
}

void Display::OnGlobalRemove(uint32_t id) {
  printf("Got a registry remover event for id %d\n", id);
}

void Display::OnShmFormat(struct wl_shm* shm, uint32_t format) {
  fprintf(stderr, "%s format=%u\n", __func__, format);
}

}  // namespace wayland
