#include <iostream>

class A {
 public:
  A() {
    std::cout << "A" << std::endl;
  }
};

class B {
 public:
  B() {
    std::cout << "B" << std::endl;
  }
};

union C {
  A a;
  B b;
  C() {
    new (&a)A();
    new (&a)B();
  }
};

int main() {
  C c;
}
