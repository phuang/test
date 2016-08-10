#ifndef __REGISTRY_H_
#define __REGISTRY_H_

#include <wayland-client.h>

#include "proxy.h"
#include "surface.h"

namespace wayland {

class Surface;

class Registry : public Proxy<struct wl_registry, Registry> {
 public:
  class Delegate {
   public:
    virtual void OnGlobal(
        uint32_t id, const char* interface, uint32_t version) {}
    virtual void OnGlobalRemove(uint32_t id) {}
  };

  explicit Registry(struct wl_registry* registry,
                    Delegate* delegate);
  ~Registry();

  template<typename T>
  T* Bind(uint32_t name, const struct wl_interface* interface,
          uint32_t version) {
    void* ret = wl_registry_bind(id(), name, interface, version);
    return static_cast<T*>(ret);
  }

 private:
  void OnGlobal(struct wl_registry* registry, uint32_t id,
                const char* interface, uint32_t version);
  void OnGlobalRemove(struct wl_registry* registry, uint32_t id);
  static void OnGlobalThunk(void* data,
                            struct wl_registry* registry,
                            uint32_t id,
                            const char* interface,
                            uint32_t version) {
    static_cast<Registry*>(data)->OnGlobal(
        registry, id, interface, version);
  }
  static void OnGlobalRemoveThunk(void* data,
                                  struct wl_registry* registry,
                                  uint32_t id) {
    static_cast<Registry*>(data)->OnGlobalRemove(registry, id);
  }

  Delegate* delegate_;
  static const struct wl_registry_listener listener_;
};

}  // namespace wayland


#endif
