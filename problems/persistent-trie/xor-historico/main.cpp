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
#define fore(x, v) for (auto x : v)

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
    int children[2] = {-1, -1};
    int cnt = 0;
};

vector<Node> nodes;

int clone(int node) {
    if (node == -1) {
        nodes.emplace_back(Node());
        return sz(nodes) - 1;
    }
    nodes.emplace_back(nodes[node]);
    return sz(nodes) - 1;
}

int add(int root, int x) {
    int newRoot = clone(root);
    nodes[newRoot].cnt++;
    int curOld = root;
    int curNew = newRoot;
    for (int i = 30; i >= 0; i--) {
        int bit = (x >> i) & 1;
        int nextOld = (curOld == -1 ? -1 : nodes[curOld].children[bit]);
        int nextNew = clone(nextOld);
        nodes[nextNew].cnt++;
        nodes[curNew].children[bit] = nextNew;
        curOld = nextOld;
        curNew = nextNew;
    }
    return newRoot;
}

void solve() {
    int n;
    cin >> n;

    vector<int> versions{-1};
    vector<int> pa{-1};

    forn(i, n) {
        int p, a;
        cin >> p >> a;
        versions.emplace_back(add(versions[p], a));
        pa.emplace_back(p);
    }

    int q;
    cin >> q;
    while (q--) {
        int u, w, x;
        cin >> u >> w >> x;
        dbg(u);
        dbg(w);
        dbg(x);

        int tw = versions[pa[w]];
        int tu = versions[u];

        int res = 0;

        for (int i = 30; i >= 0; i--) {
            bool has_bit = x & (1 << i);
            int next_tw = (tw == -1 ? -1 : nodes[tw].children[!has_bit]);
            int next_tu = (tu == -1 ? -1 : nodes[tu].children[!has_bit]);
            int cnt = (next_tu == -1 ? 0 : nodes[next_tu].cnt) -
                      (next_tw == -1 ? 0 : nodes[next_tw].cnt);
            if (cnt > 0) {
                tw = next_tw;
                tu = next_tu;
                res += 1 << i;
            } else {
                tw = (tw == -1 ? -1 : nodes[tw].children[has_bit]);
                tu = (tu == -1 ? -1 : nodes[tu].children[has_bit]);
            }
        }

        cout << res << "\n";
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
