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
    int child[26]{};
    int cnt = 0;
};

vector<Node> nodes{Node()};

int clone(int u) {
    nodes.emplace_back(nodes[u]);
    return sz(nodes) - 1;
}

int insert(int root, const string &word) {
    int newRoot = clone(root);
    nodes[newRoot].cnt++;
    int oldNode = root;
    int newNode = newRoot;
    for (char c : word) {
        int idx = c - 'a';
        int oldChild = nodes[oldNode].child[idx];
        int newChild = clone(oldChild);
        nodes[newNode].child[idx] = newChild;
        oldNode = oldChild;
        newNode = newChild;
        nodes[newNode].cnt++;
    }
    return newRoot;
}

int countPrefix(int root, const string &prefix) {
    int u = root;
    for (char c : prefix) {
        int idx = c - 'a';
        u = nodes[u].child[idx];
        if (u == 0) {
            return 0;
        }
    }
    return nodes[u].cnt;
}

void solve() {
    int q;
    cin >> q;

    vector<int> versions{0};

    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            string s;
            cin >> s;
            versions.emplace_back(insert(versions[sz(versions) - 1], s));
        } else if (op == 2) {
            int t;
            cin >> t;
            versions.emplace_back(versions[t]);
        } else {
            string p;
            cin >> p;
            versions.emplace_back(versions[sz(versions) - 1]);
            cout << countPrefix(versions[sz(versions) - 1], p) << "\n";
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
