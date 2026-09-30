#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using lld = long double;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vb = vector<bool>;
using vll = vector<ll>;

#define forr(i, a, b) for (int i = int(a); i < int(b); ++i)
#define forn(i, n) forr(i, 0, n)
#define dforr(i, a, b) for (int i = int(b) - 1; i >= int(a); --i)
#define dforn(i, n) dforr(i, 0, n)
#define fore(x, v) for (auto &x : v)

#define all(x) begin(x), end(x)
#define rall(x) rbegin(x), rend(x)
#define sz(x) (int)(x).size()

#define fi first
#define se second

#define dbg(x) cerr << #x << " = " << x << "\n"
#define vdbg(c)                                                                \
    cerr << #c << " = ";                                                       \
    fore(e, c) cerr << e << " ";                                               \
    cerr << "\n"

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

Node *build(const vector<int> &a, int tl, int tr) {
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
}

void solve() {
    int n;
    cin >> n;
    vi a(n);
    forn(i, n) cin >> a[i];

    auto compress = [](vi &v) {
        auto u = v;
        sort(all(u));
        u.resize(unique(all(u)) - begin(u));
        fore(e, v) e = lower_bound(all(u), e) - begin(u);
        return u;
    }; // a[i] = soy el i-esimo menor
       // b[i] = el valor del i-esimo menor

    vi b = compress(a);

    vector<vi> c(sz(b));
    forn(i, n) { c[a[i]].emplace_back(i); }

    vi tmp(n);
    vector<Node *> fpst{build(tmp, 0, n - 1)};

    forn(i, sz(b)) {
        fpst.emplace_back(fpst[i]);
        forn(j, sz(c[i])) {
            fpst[i + 1] = update(fpst[i + 1], 0, n - 1, c[i][j], 1);
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        l--;
        r--;
        int idx = upper_bound(all(b), k) - begin(b);
        cout << r - l + 1 - get_sum(fpst[idx], 0, n - 1, l, r) << "\n";
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
