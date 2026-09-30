#include <bits/stdc++.h>
using namespace std;

#define FOR(i, a, b) for (int i = a, _b = (b); i <= _b; i++)
#define FORD(i, b, a) for (int i = b, _a = (a); i >= a; i--)
#define REP(i, a) for (int i = 0, _a = (a); i < _a; i++)

#define fi first
#define se second
#define ll long long
#define pli pair<ll, int>
#define pii pair<int, int>
#define pdi pair<double, int>

const int MOD[] = {(int)1e9 + 7, (int)1e9 + 5277}, inf = 2e9;
const ll INF = 2e18;

const int LIM = 101000, lim = 2010;
const int BASE = 256, NMOD = 2;
int mod = (int)1e9 + 7, s;

int tot, rt[LIM];

struct PersistSeg {
    struct node {
        int ls, rs, sum;
    } nd[LIM << 6];

#define ls(k) nd[k].ls
#define rs(k) nd[k].rs

    int newNode(int p) {
        nd[++tot] = nd[p];
        return tot;
    }
    void pushUp(int p) { nd[p].sum = nd[ls(p)].sum + nd[rs(p)].sum; }
    void upd(int &p, int l, int r, int pos, int val) {
        if (pos > r || pos < l)
            return;
        p = newNode(p);
        if (l == r) {
            nd[p].sum += val;
            return;
        }
        int m = l + r >> 1;
        upd(ls(p), l, m, pos, val);
        upd(rs(p), m + 1, r, pos, val);
        pushUp(p);
    }
    int get(int p, int l, int r, int u, int v) {
        if (u > r || v < l)
            return 0;
        if (u <= l && r <= v)
            return nd[p].sum;

        int m = l + r >> 1;
        int a = get(ls(p), l, m, u, v);
        int b = get(rs(p), m + 1, r, u, v);
        return a + b;
    }
    int getPos(int p, int l, int r, int k) {
        if (l == r)
            return l;
        int m = l + r >> 1;

        int cnt = nd[rs(p)].sum;
        if (cnt > k)
            return getPos(rs(p), m + 1, r, k);
        return getPos(ls(p), l, m, k - cnt);
    }
} seg;

int n, res, a[LIM], last[LIM];

int f(int r, int k) {
    res++;
    int pos = seg.getPos(rt[r], 1, n, k);
    if (seg.get(rt[r], 1, n, pos, r) <= k)
        pos = 0;
    return pos;
}

void solve() {

    cin >> n;
    FOR(i, 1, n) cin >> a[i];

    FOR(i, 1, n) {
        rt[i] = rt[i - 1];
        if (last[a[i]])
            seg.upd(rt[i], 1, n, last[a[i]], -1);
        last[a[i]] = i;
        seg.upd(rt[i], 1, n, last[a[i]], 1);
    }

    FOR(k, 1, n) {
        int r = n;
        res = 0;
        while (r)
            r = f(r, k);
        cout << res << " ";
    }
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    solve();
}
