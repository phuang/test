// g++ -std=c++0x
#include <iostream>
#include <memory>
#include <vector>

class Data {
 public:
  Data() {
    static int c = 0;
    i = c++;
    std::cout << "New Data(" << i << ")" << std::endl;
  }
  ~Data() { std::cout << "Delete Data(" << i << ")" << std::endl; }

  void Hello() { std::cout << "Hello Data(" << i << ")" << std::endl; }
  int i;
};

typedef std::shared_ptr<Data> DataPtr;

int main() {
  DataPtr p1;
  p1.reset(new Data());

  DataPtr p2(new Data());

  p2 = p1;
  std::weak_ptr<Data> wp;
  wp = p1;

  p1->Hello();
  p2->Hello();

  std::cout << "p1 = NULL" << std::endl;
  p1.reset();
  std::cout << "p2 = NULL" << std::endl;
  p2.reset();
  std::cout << "exit" << std::endl;
  wp->Hello();

  std::vector<DataPtr> ve;

  ve.push_back(DataPtr(new Data()));
  ve.push_back(DataPtr(new Data()));
  ve.push_back(DataPtr(new Data()));
  ve.push_back(DataPtr(new Data()));
  ve.push_back(DataPtr(new Data()));
  ve.push_back(DataPtr(new Data()));
  ve.push_back(DataPtr(new Data()));

  ve.pop_back();
  ve.pop_back();
  ve.pop_back();

  ve.push_back(DataPtr(new Data()));
  ve.push_back(DataPtr(new Data()));

  for (int i = 0; i < ve.size(); i++) ve[i]->Hello();
}
