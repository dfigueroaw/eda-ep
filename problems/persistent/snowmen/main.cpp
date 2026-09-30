#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vb = vector<bool>;
using vll = vector<ll>;
using mi = vector<vi>;
using mb = vector<vb>;
using mll = vector<vll>;

#define forr(i, a, b) for (int i = int(a); i < int(b); ++i)
#define forn(i, n) forr(i, 0, n)
#define all(x) begin(x), end(x)
#define rall(x) rbegin(x), rend(x)
#define sz(x) (int)(x).size()

#define MOD1 1000000007
#define MOD2 998244353

template <typename T> class FPStack {
  struct Node {
    T value;
    std::shared_ptr<const Node> next;

    Node(const T &value_, std::shared_ptr<const Node> next_)
        : value(value_), next(next_) {}
  };

  std::vector<std::shared_ptr<const Node>> versions;

  void check_version(const size_t version) const {
    if (version >= versions.size()) {
      throw std::out_of_range("invalid stack version");
    }
  }

public:
  FPStack() : versions(1, nullptr) {}

  size_t current_version() const { return versions.size() - 1; }

  size_t versions_count() const { return versions.size(); }

  bool empty(const size_t version) const {
    check_version(version);
    return versions[version] == nullptr;
  }

  bool empty() const { return empty(current_version()); }

  const T &top(const size_t version) const {
    check_version(version);
    if (empty(version)) {
      throw std::underflow_error("top from empty stack");
    }

    return versions[version]->value;
  }

  const T &top() const { return top(current_version()); }

  size_t push(const size_t version, const T &value) {
    check_version(version);
    versions.emplace_back(std::make_shared<Node>(value, versions[version]));
    return current_version();
  }

  size_t push(const T &value) { return push(current_version(), value); }

  bool pop(const size_t version) {
    check_version(version);
    if (empty(version)) {
      return false;
    }

    versions.emplace_back(versions[version]->next);
    return true;
  }

  bool pop() { return pop(current_version()); }
};

void solve() {
  int n;
  cin >> n;

  FPStack<ll> st{};
  ll ans = 0;

  forn(i, n) {
    ll t, m;
    cin >> t >> m;
    if (m != 0) {
      st.push(t, st.empty(t) ? m : st.top(t) + m);
    } else {
      st.pop(t);
    }
    if (!st.empty()) {
      ans += st.top();
    }
  }

  cout << ans << "\n";
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
