#include <memory>
#include <iostream>

class TestClass {
 public:
  TestClass() {
    std::cout << "EEE " << __func__ << std::endl;
  }
  ~TestClass() {
    std::cout << "EEE " << __func__ << std::endl;
  }
};

int main() {
    auto p = std::make_shared<TestClass>();
    auto p1 = p;
    std::weak_ptr<TestClass> weak = p;
    p.reset();
    std::cout << "EEE reset p1 weak=" << weak.lock().get() << std::endl;
    p1.reset();
    std::cout << "EEE reset p2 weak=" << weak.lock().get() << std::endl;
    return 0;
}