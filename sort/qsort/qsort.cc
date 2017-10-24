#include <iostream>
#include <string>
#include <vector>

namespace {

template <class T>
void qsort(std::vector<T>& array, int first, int last) {
  if (first == last) return;

  int i = first;
  int j = last;

  T pv = array[j];

  while (i != j) {
    while (i < j) {
      if (array[i] > pv) {
        array[j] = array[i];
        j--;
      } else {
        i++;
      }
    }
    while (j > i) {
      if (array[j] < pv) {
        array[i] = array[j];
        i++;
      } else {
        j--;
      }
    }
  }
  array[i] = pv;

  for (int c = first; c <= last; ++c) {
    std::cout << array[c] << " ";
  }

  std::cout << std::endl;

  if (i > first + 1) qsort(array, first, i - 1);
  if (i < last - 1) qsort(array, i + 1, last);
}

}  // namespace

int main() {
  std::vector<int> test;
  test.push_back(9);
  test.push_back(4);
  test.push_back(10);
  test.push_back(11);
  test.push_back(1);
  qsort<int>(test, 0, test.size() - 1);

  std::vector<std::string> words;

  while (!std::cin.eof()) {
    std::string word;
    std::cin >> word;
    words.push_back(word);
  }

  qsort<std::string>(words, 0, words.size() - 1);

  for (int i = 0; i < words.size(); ++i) {
    std::cout << words[i] << std::endl;
  }
}
