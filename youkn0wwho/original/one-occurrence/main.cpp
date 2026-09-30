#include <bits/stdc++.h>

using namespace std;

const int INF = int(1e9);
const int N = 500001;

typedef pair<int, int> pt;

#define x first
#define y second
#define mp make_pair

struct node {
    pt val;
    node *l;
    node *r;
    node() : val(mp(INF, 0)), l(NULL), r(NULL) {};
    node(node *l, node *r) {
        this->val = min(l->val, r->val);
        this->l = l;
        this->r = r;
    }
    node(int pos, int val) {
        this->val = mp(val, pos);
        this->l = NULL;
        this->r = NULL;
    }
};

typedef node *stree;

pt query(stree t, int l, int r, int L, int R) {
    if (L >= R)
        return mp(INF, 0);
    if (l == L && r == R)
        return t->val;
    int mid = (l + r) >> 1;
    pt q1 = query(t->l, l, mid, L, min(mid, R));
    pt q2 = query(t->r, mid, r, max(mid, L), R);
    return min(q1, q2);
}

stree update(stree t, int l, int r, int pos, int val) {
    if (l == r - 1)
        return new node(pos, val);
    else {
        int mid = (l + r) >> 1;
        if (pos < mid)
            return new node(update(t->l, l, mid, pos, val), t->r);
        else
            return new node(t->l, update(t->r, mid, r, pos, val));
    }
}

stree build(int l, int r) {
    if (l == r - 1)
        return new node(l, INF);
    else {
        int mid = (l + r) >> 1;
        return new node(build(l, mid), build(mid, r));
    }
}

int main() {
    int n;
    scanf("%d", &n);
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    vector<int> left(n);
    map<int, int> last;
    for (int i = 0; i < n; i++) {
        if (!last.count(a[i]))
            left[i] = -1;
        else
            left[i] = last[a[i]];
        last[a[i]] = i;
    }

    vector<stree> T(n + 1);
    T[0] = build(0, n);
    for (int i = 0; i < n; i++) {
        stree cur = T[i];
        if (left[i] != -1)
            cur = update(cur, 0, n, left[i], INF);
        cur = update(cur, 0, n, i, left[i]);
        T[i + 1] = cur;
    }

    int q;
    scanf("%d", &q);
    for (int i = 0; i < q; i++) {
        int l, r;
        scanf("%d %d", &l, &r);
        --l;
        pt ans = query(T[r], 0, n, l, r);
        if (ans.x < l)
            printf("%d\n", a[ans.y]);
        else
            printf("0\n");
    }
}
