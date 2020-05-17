#include <chrono>
#include <iostream>
#include <random>

std::default_random_engine generator;
#if 0
void throwexpect0() {
  if (0 == generator() % 4)
    throw std::runtime_error("run time error throwexpect0");
}

void throwexpect1() {
  if (1 == generator() % 4)
    throw std::runtime_error("run time error throwexpect1");
}

void throwexpect2() {
  if (2 == generator() % 4)
    throw std::runtime_error("run time error throwexpect2");
}


void except_test(int i) {
  try {
    throwexpect0();
    throwexpect1();
    throwexpect2();
  } catch(const std::exception& e) {
    std::cout << "error=" << e.what() << std::endl;
  }
}
#else
extern bool throwexpect0() {
  return (0 == generator() % 4);
}

extern bool throwexpect1() {
  return (1 == generator() % 4);
}

extern bool throwexpect2() {
  return (2 == generator() % 4);
}


extern void except_test(int i) {
  if (!throwexpect0()) {
    std::cout << "error=run time error throwexpect0" << std::endl;
    return;
  }
  if (!throwexpect1()) {
    std::cout << "error=run time error throwexpect1" << std::endl;
    return;
  }
  if (!throwexpect2()) {
    std::cout << "error=run time error throwexpect2" << std::endl;
    return;
  }
}
#endif

int main(int argc, char**argv) {
  generator.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  except_test(argc);
  return 0;
}
