#include <vector>
#include <stdio.h>

int main() {
  std::vector<int> ints = {1,2,3,4,5,6,7,8,9,0};
  for (auto i : ints)
    fprintf(stderr, "EEE i=%d\n", i);
  std::move(ints);
  fprintf(stderr, "EEE ints.size()=%zu\n", ints.size());

  return 0;
}
