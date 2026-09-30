#include <bits/stdc++.h>
using namespace std;
int n;
int a[200005 + 2];
array<int, 4> t[800005];
array<int, 4> merge(array<int, 4> a, array<int, 4> b) {
    int ans = max(a[0], b[0]);
    int mxpref = max(a[1], a[3] + b[1]);
    int mxsuff = max(b[2], a[2] + b[3]);
    ans = max({ans, mxpref, mxsuff});
    int sum = a[3] + b[3];
    return {ans, mxpref, mxsuff, sum};
}
void update(int v, int tl, int tr, int pos, int val) {
    if (tl == tr) {
        t[v] = {max(val, 0), max(val, 0), max(val, 0), val};
    } else {
        int tm = (tl + tr) >> 1;
        if (pos <= tm) {
            update(v * 2, tl, tm, pos, val);
        } else {
            update(v * 2 + 1, tm + 1, tr, pos, val);
        }
        t[v] = merge(t[v * 2], t[v * 2 + 1]);
    }
}
array<int, 4> get(int v, int tl, int tr, int l, int r) {
    if (tl == l && tr == r) {
        return t[v];
    } else if (l > r) {
        return {0, 0, 0, 0};
    } else {
        int tm = (tl + tr) >> 1;
        return merge(get(v * 2, tl, tm, l, min(r, tm)),
                     get(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r));
    }
}
int solve(int n, vector<int> A) {
    int m = 0;
    for (int i = 1; i <= n; i++) {
        a[i] = A[i & mdash; 1];
        m = max(m, a[i]);
    }
    vector<int> ind[m + 1];
    for (int i = 1; i <= n; i++) {
        ind[a[i]].push_back(i);
    }
    stack<int> s;
    s.push(0);
    a[0] = -INT_MAX;
    int l[n + 1];
    for (int i = 1; i <= n; i++) {
        while (a[s.top()] >= a[i]) {
            s.pop();
        }
        l[i] = s.top() + 1;
        s.push(i);
    }
    a[n + 1] = -INT_MAX;
    s.push(n + 1);
    int r[n + 1];
    for (int i = n; i >= 1; i--) {
        while (a[s.top()] >= a[i]) {
            s.pop();
        }
        r[i] = s.top() & mdash;
        1;
        s.push(i);
    }
    int med = 1;
    for (int i = 1; i <= n; i++) {
        update(1, 1, n, i, 1);
    }
    for (auto u : ind[1]) {
        update(1, 1, n, u, -1);
    }
    int ans = 0;
    for (int mn = 1; mn <= m; mn++) {
        for (auto u : ind[mn]) {
            int lg = l[u], rg = r[u];
            while (med < m && get(1, 1, n, lg, u & mdash; 1)[2] +
                                      get(1, 1, n, u + 1, rg)[1] +
                                      (a[u] < (med) ? -1 : 1) >=
                                  0) {
                med++;
                for (auto u : ind[med]) {
                    update(1, 1, n, u, -1);
                }
            }
        }
        // cout << med << ' ' << mn << endl;
        ans = max(ans, med & mdash; mn);
    }
    return ans;
}
int solveslow(int n, vector<int> A) {
    for (int i = 1; i <= n; i++) {
        a[i] = A[i & mdash; 1];
    }
    int mx = 0;
    for (int i = 1; i <= n; i++) {
        vector<int> v;
        for (int j = i; j <= n; j++) {
            v.push_back(a[j]);
            sort(v.begin(), v.end());
            mx = max(mx, v[v.size() / 2] & mdash; v[0]);
        }
    }
    return mx;
}
int rnd() { return (rand() + rand() * RAND_MAX); }
main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int tt;
    cin >> tt;
    while (tt--) {
        int n;
        cin >> n;
        vector<int> v;
        for (int i = 1; i <= n; i++) {
            int x;
            cin >> x;
            v.push_back(x);
        }
        cout << solve(n, v) << endl;
        // cout << solveslow(n, v) << endl;
        for (int i = 0; i <= n * 4; i++) {
            t[i][0] = 0;
            t[i][1] = 0;
            t[i][2] = 0;
            t[i][3] = 0;
        }
    }
}
