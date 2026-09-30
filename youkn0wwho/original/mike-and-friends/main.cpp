#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;
#define Foreach(i, c)                                                          \
    for (__typeof((c).begin()) i = (c).begin(); i != (c).end(); ++i)
#define For(i, a, b) for (int(i) = (a); (i) < (b); ++(i))
#define rof(i, a, b) for (int(i) = (a); (i) > (b); --(i))
#define rep(i, c) for (auto &(i) : (c))
#define x first
#define y second
#define pb push_back
#define PB pop_back()
#define iOS ios_base::sync_with_stdio(false)
#define sqr(a) (((a) * (a)))
#define all(a) a.begin(), a.end()
#define error(x) cerr << #x << " = " << (x) << endl
#define Error(a, b)                                                            \
    cerr << "( " << #a << " , " << #b << " ) = ( " << (a) << " , " << (b)      \
         << " )\n";
#define errop(a) cerr << #a << " = ( " << ((a).x) << " , " << ((a).y) << " )\n";
#define coud(a, b) cout << fixed << setprecision((b)) << (a)
#define L(x) ((x) << 1)
#define R(x) (((x) << 1) + 1)
#define umap unordered_map
#define double long double
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef complex<double> point;
template <typename T>
using os =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <class T> inline void smax(T &x, T y) { x = max((x), (y)); }
template <class T> inline void smin(T &x, T y) { x = min((x), (y)); }
const int maxn = 5e5 + 100;
int TRIE[maxn][26];
string s[maxn];
int f[maxn], aut[maxn][26], root, nx = 1;
vi tof[maxn];
vi adj[maxn];
int end[maxn];
inline void build(int x) {
    int v = root;
    rep(ch, s[x]) {
        int c = ch - 'a';
        if (TRIE[v][c] == -1) {
            TRIE[v][c] = nx;
            ++nx;
        }
        v = TRIE[v][c];
    }
    ::end[x] = v;
}
inline void ahoc() {
    f[root] = root;
    queue<int> q;
    q.push(root);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        For(c, 0, 26) {
            if (TRIE[v][c] != -1) {
                aut[v][c] = TRIE[v][c];
                if (v != root)
                    f[aut[v][c]] = aut[f[v]][c];
                else
                    f[aut[v][c]] = root;
                q.push(TRIE[v][c]);
            } else {
                if (v == root)
                    aut[v][c] = root;
                else
                    aut[v][c] = aut[f[v]][c];
            }
        }
    }
}
inline void go(int x) {
    int v = root;
    rep(ch, s[x]) {
        int c = ch - 'a';
        v = aut[v][c];
        tof[v].pb(x);
    }
}
int par[maxn];
int st[maxn], ft[maxn];
int a[maxn], nex = 0;
inline void dfs(int v) {
    st[v] = nex;
    rep(x, tof[v]) a[nex++] = x;
    rep(u, adj[v]) dfs(u);
    ft[v] = nex;
}
struct query {
    int l, r, k, ind, sgn;
    query() { l = r = k = 0; }
    query(int L, int R, int K, int IND, int SGN) {
        l = L, r = R, k = K, ind = IND, sgn = SGN;
    }
    bool operator<(const query &a) const {
        if (k != a.k)
            return k < a.k;
        return pii(l, r) < pii(a.l, a.r);
    }
};
pii p[maxn];
int fen[maxn];
inline void add(int x) {
    ++x;
    for (int i = x; i < maxn; i += i & -i)
        fen[i]++;
}
inline int ask(int x) {
    ++x;
    int res = 0;
    for (int i = x; i > 0; i -= i & -i)
        res += fen[i];
    return res;
}
inline int ask(int l, int r) { return max(0, ask(r) - ask(l - 1)); }
int answ[maxn];
vector<query> quer;
int lq[maxn], rq[maxn], kq[maxn];
char ch[maxn];
int main() {
    memset(TRIE, -1, sizeof TRIE);
    memset(par, -1, sizeof par);
    int n, q;
    scanf("%d %d", &n, &q);
    For(i, 0, n) {
        scanf("%s", ch);
        s[i] = (string)ch;
        build(i);
    }
    ahoc();
    For(i, 0, n) go(i);
    For(i, 1, nx) {
        par[i] = f[i];
        adj[par[i]].pb(i);
    }
    dfs(root);
    For(i, 0, q) {
        scanf("%d %d %d", lq + i, rq + i, kq + i);
        --kq[i];
        kq[i] = ::end[kq[i]];
        lq[i]--;
        rq[i]--;
        quer.pb(query(st[kq[i]], ft[kq[i]] - 1, lq[i] - 1, i, -1));
        quer.pb(query(st[kq[i]], ft[kq[i]] - 1, rq[i], i, 1));
    }
    sort(all(quer));
    For(i, 0, nex) p[i] = {a[i], i};
    sort(p, p + nex);
    int po = 0;
    rep(q, quer) {
        int l = q.l;
        int r = q.r;
        int k = q.k;
        while (po < nex && p[po].x <= k) {
            add(p[po].y);
            po++;
        }
        int ans = ask(l, r);
        int ind = q.ind;
        answ[ind] += q.sgn * ans;
    }
    For(i, 0, q) printf("%d\n", answ[i]);
}
