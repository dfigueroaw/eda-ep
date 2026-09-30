#include <bits/stdc++.h>
#include <functional>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

#define all(x) begin(x), end(x)
#define rall(x) rbegin(x), rend(x)

template <typename T, typename C = std::less<T>> class Heap {
  std::vector<T> vec;
  C compare;

  size_t parent(const size_t idx) { return (idx - 1) / 2; }

  size_t left(const size_t idx) { return 2 * idx + 1; }

  size_t right(const size_t idx) { return 2 * idx + 2; }

  void percolate_down(const size_t idx) {
    size_t l = left(idx);
    size_t r = right(idx);
    size_t m = idx;

    if (l < size() && compare(vec[m], vec[l])) {
      m = l;
    }

    if (r < size() && compare(vec[m], vec[r])) {
      m = r;
    }

    if (m != idx) {
      std::swap(vec[idx], vec[m]);
      percolate_down(m);
    }
  }

  void percolate_up(const size_t idx) {
    if (idx == 0) {
      return;
    }

    size_t p = parent(idx);

    if (compare(vec[p], vec[idx])) {
      std::swap(vec[idx], vec[p]);
      percolate_up(p);
    }
  }

  void build_heap() {
    if (size() <= 1) {
      return;
    }

    for (size_t i = parent(size() - 1); i >= 0; i--) {
      percolate_down(i);
    }
  }

public:
  Heap() = default;

  Heap(const std::vector<T> &v, C comp = C()) : vec(v), compare(comp) {
    build_heap();
  }

  void print() {
    for (size_t i = 0; i < size(); i++) {
      std::cout << i << ": " << vec[i] << std::endl;
    }
  }

  T top() { return vec[0]; }

  size_t size() { return vec.size(); }

  bool empty() { return size() == 0; }

  bool pop() {
    if (empty()) {
      return false;
    }

    vec[0] = vec[size() - 1];
    vec.pop_back();
    percolate_down(0);
    return true;
  }

  void push(const T &val) {
    vec.emplace_back(val);
    percolate_up(size() - 1);
  }
};

void solve() {
  int n, m;
  cin >> n >> m;
  vector<unordered_map<int, long long>> g(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    long long w;
    cin >> a >> b >> w;
    a--;
    b--;
    if (g[a].contains(b)) {
      g[a][b] = min(g[a][b], w);
      g[b][a] = min(g[b][a], w);
    } else {
      g[a][b] = w;
      g[b][a] = w;
    }
  }

  vector<long long> d(n, 1e18);
  vector<int> p(n, -1);
  d[0] = 0;

  Heap<pair<long long, int>, std::greater<>> pq;
  pq.push({0, 0});

  while (!pq.empty()) {
    pair<long long, int> current = pq.top();
    pq.pop();
    if (current.first != d[current.second]) {
      continue;
    }
    for (auto &[u, w] : g[current.second]) {
      if (d[u] > current.first + w) {
        d[u] = current.first + w;
        p[u] = current.second;
        pq.push({d[u], u});
      }
    }
  }

  if (p[n - 1] == -1) {
    cout << "-1\n";
  } else {
    vector<int> out;
    int cur = n - 1;
    while (cur != -1) {
      out.emplace_back(cur);
      cur = p[cur];
    }
    for (int i : views::reverse(out)) {
      cout << i + 1 << " ";
    }
    cout << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int tc = 1;
  // cin >> tc;
  while (tc--)
    solve();
  return 0;
}
