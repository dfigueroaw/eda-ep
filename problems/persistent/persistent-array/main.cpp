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

struct Node {
  Node *left, *right;
  int sum;

  Node(int val) : left(nullptr), right(nullptr), sum(val) {}
  Node(Node *l, Node *r) : left(l), right(r), sum(0) {
    if (l) {
      sum += l->sum;
    }
    if (r) {
      sum += r->sum;
    }
  }
};

Node *build(const vi &a, int tl, int tr) {
  if (tl == tr) {
    return new Node(a[tl]);
  }
  int tm = (tl + tr) / 2;
  return new Node(build(a, tl, tm), build(a, tm + 1, tr));
}

int get_sum(Node *v, int tl, int tr, int l, int r) {
  if (l > r) {
    return 0;
  }
  if (l == tl && r == tr) {
    return v->sum;
  }
  int tm = (tl + tr) / 2;
  return get_sum(v->left, tl, tm, l, min(r, tm)) +
         get_sum(v->right, tm + 1, tr, max(l, tm + 1), r);
}

Node *update(Node *v, int tl, int tr, int pos, int new_val) {
  if (tl == tr) {
    return new Node(new_val);
  }
  int tm = (tl + tr) / 2;
  if (pos <= tm) {
    return new Node(update(v->left, tl, tm, pos, new_val), v->right);
  } else {
    return new Node(v->left, update(v->right, tm + 1, tr, pos, new_val));
  }
} // Implementación inspirada en la de cp-algorithms.com.

void solve() {
  int n;
  cin >> n;
  vi a(n + 1);
  forr(i, 1, n + 1) { cin >> a[i]; }

  vector<Node *> fpst = {build(a, 1, n)};

  int m;
  cin >> m;
  while (m--) {
    string op;
    cin >> op;
    int i, j;
    cin >> i >> j;
    i--;
    if (op == "create") {
      int x;
      cin >> x;
      fpst.emplace_back(update(fpst[i], 1, n, j, x));
    } else {
      cout << get_sum(fpst[i], 1, n, j, j) << "\n";
    }
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
