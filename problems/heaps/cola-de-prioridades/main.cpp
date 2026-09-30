#include <cstddef>
#include <iostream>
#include <vector>

template <typename T> class MaxHeap {
  std::vector<T> vec;

  size_t parent(const size_t idx) { return (idx - 1) / 2; }

  size_t left(const size_t idx) { return 2 * idx + 1; }

  size_t right(const size_t idx) { return 2 * idx + 2; }

  size_t size() { return vec.size(); }

  bool empty() { return size() == 0; }

  void percolate_down(const size_t idx) {
    size_t l = left(idx);
    size_t r = right(idx);
    size_t m = idx;
    if (l < size() && vec[l] > vec[m]) {
      m = l;
    }
    if (r < size() && vec[r] > vec[m]) {
      m = r;
    }
    if (m != idx) {
      std::swap(vec[idx], vec[m]);
      percolate_down(m);
    }
  }

  void percolate_up(const size_t idx) {
    if (idx == 0)
      return;
    size_t p = parent(idx);

    if (vec[p] < vec[idx]) {
      std::swap(vec[idx], vec[p]);
      percolate_up(p);
    }
  }

  void build_heap() {
    for (int i = parent(size() - 1); i >= 0; i--) {
      percolate_down(i);
    }
  }

public:
  MaxHeap() = default;

  MaxHeap(const std::vector<T> &v) : vec(v) { build_heap(); }

  void print() {
    for (int i = 0; i < size(); i++) {
      std::cout << i << ": " << vec[i] << std::endl;
    }
  }

  T top() { return vec[0]; }

  bool pop() {
    if (empty())
      return false;

    vec[0] = vec[size() - 1];
    vec.pop_back();
    percolate_down(0);
    return true;
  }

  void insert(const T &val) {
    vec.emplace_back(val);
    percolate_up(size() - 1);
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::vector<int> v = {};
  MaxHeap<int> mh(v);
  while (true) {
    std::string op;
    std::cin >> op;
    if (op == "insert") {
      int val;
      std::cin >> val;
      mh.insert(val);
    } else if (op == "extract") {
      int top = mh.top();
      mh.pop();
      std::cout << top << std::endl;
    } else {
      break;
    }
  }
}
