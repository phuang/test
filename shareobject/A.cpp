
#include "A.h"

#include <iostream>

namespace test {

A::A() {
  std::cout << "EEE A()" << std::endl;
}

A::~A() = default;

}  // namespace test