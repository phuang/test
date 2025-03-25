#if defined(A_IMPLEMENTATION)
#define A_EXPORT __attribute__((visibility("default")))
#else
#define A_EXPORT
#endif

class A_EXPORT A {
 public:
  A();
  virtual ~A();
};