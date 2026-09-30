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
#define MOD3 1000000000

struct Node {
    Node *left, *right;
    ll sum;

    Node(ll val) : left(nullptr), right(nullptr), sum(val) {}
    Node(Node *l, Node *r) : left(l), right(r), sum(0) {
        if (l) {
            sum += l->sum;
        }
        if (r) {
            sum += r->sum;
        }
    }
};

Node *build(const vll &a, int tl, int tr) {
    if (tl == tr) {
        return new Node(a[tl]);
    }
    int tm = (tl + tr) / 2;
    return new Node(build(a, tl, tm), build(a, tm + 1, tr));
}

ll get_sum(Node *v, int tl, int tr, int l, int r) {
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

Node *update(Node *v, int tl, int tr, int pos, ll new_val) {
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
    ll n;
    cin >> n;
    vll a(n);
    ll l, m;
    cin >> a[0] >> l >> m;
    forr(i, 1, n) { a[i] = ((a[i - 1] * l) % MOD3 + m) % MOD3; }

    auto compress = [](vll &v) {
        auto u = v;
        sort(all(u));
        u.resize(unique(all(u)) - begin(u));
        fore(e, v) e = lower_bound(all(u), e) - begin(u);
        return u;
    }; // a[i] = soy el i-esimo menor
       // b[i] = el valor del i-esimo menor

    vll b = compress(a);

    vll tmp(sz(b));
    vector<Node *> fpst{build(tmp, 0, sz(b) - 1)};

    forn(i, n) {
        ll cur_val = get_sum(fpst[i], 0, sz(b) - 1, a[i], a[i]);
        fpst.emplace_back(update(fpst[i], 0, sz(b) - 1, a[i], cur_val + 1));
    }

    auto query = [&](ll i, ll j, ll k) {
        ll lo = 0;
        ll hi = sz(b) - 1;
        while (lo < hi) {
            ll mi = (hi + lo) / 2;
            ll n_lo = get_sum(fpst[j], 0, sz(b) - 1, 0, mi) -
                      get_sum(fpst[i - 1], 0, sz(b) - 1, 0, mi);

            if (n_lo < k) {
                lo = mi + 1;
            } else {
                hi = mi;
            }
        }

        return lo;
    };

    int q;
    cin >> q;
    ll ans = 0;
    while (q--) {
        ll G, xg, lx, mx, yg, ly, my, kg, lk, mk;
        cin >> G >> xg >> lx >> mx >> yg >> ly >> my >> kg >> lk >> mk;
        ll ig = min(xg, yg);
        ll jg = max(xg, yg);
        ans += b[query(ig, jg, kg)];
        forr(g, 1, G) {
            xg = ((xg - 1) * lx + mx) % n + 1;
            yg = ((yg - 1) * ly + my) % n + 1;
            ig = min(xg, yg);
            jg = max(xg, yg);
            kg = ((kg - 1) * lk + mk) % (jg - ig + 1) + 1;
            ans += b[query(ig, jg, kg)];
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
