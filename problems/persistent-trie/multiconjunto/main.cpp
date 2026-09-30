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

int remove(int root, int x) {
    int newRoot = clone(root);
    nodes[newRoot].cnt--;
    int curOld = root;
    int curNew = newRoot;
    for (int i = 30; i >= 0; i--) {
        int bit = (x >> i) & 1;
        int nextOld = nodes[curOld].children[bit];
        int nextNew = clone(nextOld);
        nodes[nextNew].cnt--;
        nodes[curNew].children[bit] = nextNew;
        curOld = nextOld;
        curNew = nextNew;
    }
    return newRoot;
}

int kth(int root, int k) {
    int cur = root;
    int res = 0;
    for (int i = 30; i >= 0; i--) {
        int left = nodes[cur].children[0];
        int leftCnt = (left == -1 ? 0 : nodes[left].cnt);
        if (k <= leftCnt) {
            cur = left;
        } else {
            k -= leftCnt;
            cur = nodes[cur].children[1];
            res |= (1 << i);
        }
    }

    return res;
}

void solve() {
    int q;
    cin >> q;
    vi versions{-1};
    while (q--) {
        int t, v, y;
        cin >> t >> v >> y;
        dbg(q);
        dbg(t);
        dbg(v);
        dbg(y);
        if (t == 1) {
            versions.emplace_back(add(versions[v], y));
        } else if (t == 2) {
            versions.emplace_back(remove(versions[v], y));
        } else {
            cout << kth(versions[v], y) << "\n";
            versions.emplace_back(versions[v]);
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
