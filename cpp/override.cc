#include <iostream>

class A {
  public:
  virtual ~A() {
    std::cout << "A destructor" << std::endl;
  }

  virtual void foo() {
    std::cout << "A foo" << std::endl;
  }
  virtual void bar() {
    std::cout << "A bar" << std::endl;
  }
};

class B : public A{
  public:
  ~B() {
    std::cout << "B destructor" << std::endl;
  }
  void foo() {
    std::cout << "B foo" << std::endl;
  }
  void bar() override {
    std::cout << "B bar" << std::endl;
  }
};


int main() {
  A* a = new B();
  a->foo();
  a->bar();
  delete a;
  return 0;
}