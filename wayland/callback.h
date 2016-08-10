#ifndef __CALLBACK_H_
#define __CALLBACK_H_

#include <wayland-client.h>

#include "proxy.h"

namespace wayland {

class Callback : public Proxy<struct wl_callback, Callback> {
 public:
  Callback(struct wl_callback* callback);
  ~Callback();

 protected:
  virtual void OnDone(struct wl_callback* callback,
                      uint32_t callback_data);

 private:

  static void OnDoneThunk(void* data,
                          struct wl_callback* callback,
                          uint32_t callback_data) {
    static_cast<Callback*>(data)->OnDone(callback, callback_data);
  }

  static const struct wl_callback_listener listener_;
};

}  // namespace wayland
#endif
