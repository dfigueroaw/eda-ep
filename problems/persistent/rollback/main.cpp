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
    int n, m;
    cin >> n >> m;
    vi a(n + 1, 1);
    vector<Node *> fpst{build(a, 1, n)};
    forr(i, 1, n + 1) { cin >> a[i]; }

    unordered_map<int, int> um;
    forr(i, 1, n + 1) {
        if (um.contains(a[i])) {
            fpst.emplace_back(update(fpst[i - 1], 1, n, um[a[i]], 0));
        } else {
            fpst.emplace_back(fpst[i - 1]);
        }
        um[a[i]] = i;
    }

    int q;
    cin >> q;
    int p = 0;
    while (q--) {
        int x, y;
        cin >> x >> y;
        int l = (x + p) % n + 1;
        int k = (y + p) % m + 1;

        if (get_sum(fpst[n], 1, n, l, n) < k) {
            cout << "0\n";
            p = 0;
        } else {
            int lo = l;
            int hi = n;
            while (lo != hi) {
                int mi = (lo + hi) / 2;
                int cnt = get_sum(fpst[mi], 1, n, l, mi);
                if (cnt < k) {
                    lo = mi + 1;
                } else {
                    hi = mi;
                }
            }
            cout << lo << "\n";
            p = lo;
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
