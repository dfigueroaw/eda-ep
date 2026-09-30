#include <bits/stdc++.h>
#define ll long long
#define pii pair<int, int>
#define fi first
#define se second
using namespace std;

const int N = 1e5 + 5;

int n, q;
int h[N];

struct Query {
    int l, r, w;
};

Query qr[N];

struct Node {
    int len, sum, pre, suf, best;
};

Node st[N << 2];

vector<int> pos[N];
vector<int> bucket[N];

int lo[N], hi[N];

Node MergeNode(Node a, Node b) {
    Node c;
    c.len = a.len + b.len;
    c.sum = a.sum + b.sum;
    c.pre = a.pre;
    if (a.pre == a.len)
        c.pre = a.len + b.pre;
    c.suf = b.suf;
    if (b.suf == b.len)
        c.suf = b.len + a.suf;
    c.best = max({a.best, b.best, a.suf + b.pre});
    return c;
}

void build(int id, int l, int r) {
    st[id] = {r - l + 1, 0, 0, 0, 0};
    if (l == r)
        return;
    int mid = (l + r) >> 1;
    build(id << 1, l, mid);
    build(id << 1 | 1, mid + 1, r);
}

void upd(int id, int l, int r, int p) {
    if (l == r) {
        st[id] = {1, 1, 1, 1, 1};
        return;
    }
    int mid = (l + r) >> 1;
    if (p <= mid)
        upd(id << 1, l, mid, p);
    else
        upd(id << 1 | 1, mid + 1, r, p);
    st[id] = MergeNode(st[id << 1], st[id << 1 | 1]);
}

Node get(int id, int l, int r, int u, int v) {
    if (l >= u && r <= v)
        return st[id];
    int mid = (l + r) >> 1;
    if (v <= mid)
        return get(id << 1, l, mid, u, v);
    if (u > mid)
        return get(id << 1 | 1, mid + 1, r, u, v);
    return MergeNode(get(id << 1, l, mid, u, v),
                     get(id << 1 | 1, mid + 1, r, u, v));
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
#define name "task"
    if (fopen(name ".inp", "r")) {
        freopen(name ".inp", "r", stdin);
        freopen(name ".out", "w", stdout);
    }
    cin >> n;
    vector<int> val;
    for (int i = 1; i <= n; i++) {
        cin >> h[i];
        val.push_back(h[i]);
    }
    sort(val.begin(), val.end(), greater<int>());
    val.erase(unique(val.begin(), val.end()), val.end());
    int k = val.size();
    for (int i = 1; i <= n; i++) {
        int id = lower_bound(val.begin(), val.end(), h[i], greater<int>()) -
                 val.begin();
        pos[id].push_back(i);
    }
    cin >> q;
    for (int i = 1; i <= q; i++) {
        cin >> qr[i].l >> qr[i].r >> qr[i].w;
        lo[i] = 0;
        hi[i] = k - 1;
    }
    while (1) {
        bool finish = 1;
        for (int i = 0; i < k; i++)
            bucket[i].clear();
        for (int i = 1; i <= q; i++) {
            if (lo[i] < hi[i]) {
                finish = 0;
                int mid = (lo[i] + hi[i]) / 2;
                bucket[mid].push_back(i);
            }
        }
        if (finish)
            break;
        build(1, 1, n);
        for (int mid = 0; mid < k; mid++) {
            for (int p : pos[mid])
                upd(1, 1, n, p);
            for (int id : bucket[mid]) {
                Node cur = get(1, 1, n, qr[id].l, qr[id].r);
                if (cur.best >= qr[id].w)
                    hi[id] = mid;
                else
                    lo[id] = mid + 1;
            }
        }
    }
    for (int i = 1; i <= q; i++)
        cout << val[lo[i]] << '\n';
}
