#include <bits/stdc++.h>
#include <cassert>
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

struct ModInt {
    ll x;
    ModInt() : x(0LL) {}
    ModInt(ll xx) : x(xx) {}
    ModInt operator+(ModInt b) { return ModInt((x + b.x) % MOD1); }
    ModInt operator-(ModInt b) { return ModInt((x - b.x + MOD1) % MOD1); }
    ModInt operator*(ModInt b) { return ModInt((x * b.x) % MOD1); }
    bool operator==(ModInt b) { return x == b.x; }
    friend ostream &operator<<(ostream &os, const ModInt &a) {
        return os << a.x;
    }
};

constexpr int N = 1e5 + 30;
ModInt pw[N];

struct Node {
    Node *left, *right;
    int sum, size;
    ModInt val;

    Node(int sum = 0, int size = 1, ModInt val = 0)
        : left(nullptr), right(nullptr), sum(sum), size(size), val(val) {}

    Node(Node *l, Node *r) : left(l), right(r) {
        sum = l->sum + r->sum;
        size = l->size + r->size;
        val = l->val + pw[l->size] * r->val;
    }
};

Node *build(int tl, int tr) {
    if (tl == tr) {
        return new Node();
    }
    int tm = (tl + tr) / 2;
    return new Node(build(tl, tm), build(tm + 1, tr));
}

Node *revise(Node *p, int l, int r, int x) {
    if (l == r) {
        return new Node(p->sum + 1, 1, p->val + 1);
    }
    int mid = (l + r) / 2;
    if (x <= mid) {
        return new Node(revise(p->left, l, mid, x), p->right);
    }
    return new Node(p->left, revise(p->right, mid + 1, r, x));
} // Sumar 1 al valor en la posición i de p

Node *assign(Node *p, Node *q, int l, int r, int L, int R) {
    if (L <= l && r <= R) {
        return q;
    }
    int mid = (l + r) / 2;
    Node *nl = p->left, *nr = p->right;
    if (L <= mid) {
        nl = assign(p->left, q->left, l, mid, L, R);
    }
    if (R > mid) {
        nr = assign(p->right, q->right, mid + 1, r, L, R);
    }
    return new Node(nl, nr);
} // Asignar al rango [L, R] de p el rango [L, R] de q

int query(Node *u, int l, int r, int L, int R) {
    if (L <= l && r <= R) {
        return u->sum;
    }
    int mid = (l + r) / 2;
    int ans = 0;
    if (L <= mid) {
        ans += query(u->left, l, mid, L, R);
    }
    if (R > mid) {
        ans += query(u->right, mid + 1, r, L, R);
    }
    return ans;
} // Suma de los nodos en el rango [L, R]

int binary(Node *u, int l, int r, int x, int c) {
    if (l == r) {
        return l;
    }
    int mid = (l + r) / 2;
    if (u->left->sum >= mid - x + 1 + c) {
        return binary(u->right, mid + 1, r, x, c - u->left->sum);
    }
    return binary(u->left, l, mid, x, c);
} // Sumar 2^x , antes de la posición x hay c posiciones con 1

bool compare(Node *p, Node *q, int l, int r) {
    if (l == r) {
        return p->sum > q->sum;
    }
    int mid = (l + r) / 2;
    if (p->right->val == q->right->val) {
        return compare(p->left, q->left, l, mid);
    } // Funciona probabilísticamente, sin embargo no es siempre válido
    return compare(p->right, q->right, mid + 1, r);
} // Comprueba si p > q

void solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> g(n);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w, u--, v--;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    int start, end;
    cin >> start >> end, start--, end--;

    pw[0] = 1;
    for (int i = 1; i < N; i++)
        pw[i] = pw[i - 1] * 2;

    Node *zero = build(0, N - 1);

    auto plus = [&](Node *p, int w) {
        int c = w > 0 ? query(p, 0, N - 1, 0, w - 1) : 0;
        int i = binary(p, 0, N - 1, w, c);
        Node *u;
        u = i > w ? assign(p, zero, 0, N - 1, w, i - 1) : p;
        return revise(u, 0, N - 1, i);
    };

    struct State {
        int u;
        Node *rt;
        bool operator<(State o) const { return compare(rt, o.rt, 0, N - 1); }
    };

    priority_queue<State> pq;

    vector<Node *> root(n, nullptr);
    root[start] = zero;

    pq.push({start, root[start]});

    vector<int> pre(n, -1);
    vector<bool> visited(n);

    while (!pq.empty()) {
        int u = pq.top().u;
        pq.pop();

        if (visited[u]) {
            continue;
        }

        visited[u] = true;

        if (u == end) {
            break;
        }

        for (auto [v, w] : g[u]) {
            Node *next = plus(root[u], w);

            if (!root[v] || compare(root[v], next, 0, N - 1)) {
                root[v] = next;
                pre[v] = u;

                if (!visited[v]) {
                    pq.push({v, root[v]});
                }
            }
        }
    }

    if (!visited[end]) {
        cout << -1 << '\n';
        return;
    }

    cout << root[end]->val << '\n';

    vector<int> path{end};
    int u = end;

    while (u != start) {
        u = pre[u];
        path.push_back(u);
    }

    cout << path.size() << '\n';

    for (int i = path.size() - 1; i >= 0; i--)
        cout << path[i] + 1 << ' ';

    cout << '\n';
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
