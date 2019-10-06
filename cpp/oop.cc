#include <stdio.h>

class A {
 public:
  A() {
    fprintf(stderr, "EEEE A()\n");
  }
  ~A() {
    fprintf(stderr, "EEEE ~A()\n");
  }
};

A CreateA() {
  return A();
}

// Test program
int main() {
  A a = CreateA();
  return 0;
}
