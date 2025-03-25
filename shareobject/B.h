#if defined(B_IMPLEMENTATION)
#define B_EXPORT __attribute__((visibility("default")))
#else
#define B_EXPORT
#endif

namespace test {
class B_EXPORT B {
 public:
  B();
  virtual ~B();
};
}