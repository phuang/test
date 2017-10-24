#include <boost/bind.hpp>
#include <iostream>

typedef int (*Func)(int);

template <class Function>
void test_func(Function func) {
  int ret = func(100);
  std::cout << "ret = " << ret << std::endl;
}

int inc(int i) { return i + 1; }

int add(int i, int b) { return i + b; }

int main(int argc, char **argv) {
  test_func(inc);
  test_func(boost::bind(inc, _1));
  test_func(boost::bind(add, _1, 9));
}
