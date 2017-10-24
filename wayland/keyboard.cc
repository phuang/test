#include "keyboard.h"

#include <stdio.h>

namespace wayland {

const struct wl_keyboard_listener Keyboard::listener_ = {
    Keyboard::OnKeymapThunk,    Keyboard::OnEnterThunk,
    Keyboard::OnLeaveThunk,     Keyboard::OnKeyThunk,
    Keyboard::OnModifiersThunk, Keyboard::OnRepeatInfoThunk,
};

Keyboard::Keyboard(struct wl_keyboard* keyboard) : Proxy(keyboard) {
  wl_keyboard_add_listener(id(), &listener_, this);
}

Keyboard::~Keyboard() { wl_keyboard_release(id()); }

void Keyboard::OnKeymap(struct wl_keyboard* keyboard, uint32_t format,
                        int32_t fd, uint32_t size) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Keyboard::OnEnter(struct wl_keyboard* keyboard, uint32_t serial,
                       struct wl_surface* surface, struct wl_array* keys) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Keyboard::OnLeave(struct wl_keyboard* keyboard, uint32_t serial,
                       struct wl_surface* surface) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Keyboard::OnKey(struct wl_keyboard* keyboard, uint32_t serial,
                     uint32_t time, uint32_t key, uint32_t state) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Keyboard::OnModifiers(struct wl_keyboard* keyboard, uint32_t serial,
                           uint32_t mods_depressed, uint32_t mods_latched,
                           uint32_t mods_locked, uint32_t group) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

void Keyboard::OnRepeatInfo(struct wl_keyboard* wl_keyboard, int32_t rate,
                            int32_t delay) {
  fprintf(stderr, "%s this=%p\n", __PRETTY_FUNCTION__, this);
}

}  // namespace wayland
