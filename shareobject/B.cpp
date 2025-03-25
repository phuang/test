
#include "B.h"
#include "A.h"

#include <iostream>

namespace test {

B::B() {
  std::cout << "EEE B()" << std::endl;
  A a;
}

B::~B() = default;

}  // namespace test