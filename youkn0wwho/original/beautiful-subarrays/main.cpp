#include <bits/stdc++.h>

using namespace std;

const int N = 1200300, LOGN = 30, V = N * LOGN;

int n, k;
int a[N];

bool read() {
    if (!(cin >> n >> k))
        return false;
    forn(i, n) assert(scanf("%d", &a[i]) == 1);
    return true;
}

int tsz;
int nt[V][2];
int cnt[V];

void clear() {
    forn(i, V) {
        nt[i][0] = nt[i][1] = -1;
        cnt[i] = 0;
    }
    tsz = 1;
}

void add(int x) {
    int v = 0;
    cnt[v]++;

    nfor(i, LOGN) {
        int b = (x >> i) & 1;
        if (nt[v][b] == -1) {
            assert(tsz < V);
            nt[v][b] = tsz++;
        }
        v = nt[v][b];
        cnt[v]++;
    }
}

int calc(int x) {
    int v = 0;
    int ans = 0;

    auto getCnt = [](int v) { return v == -1 ? 0 : cnt[v]; };

    int cur = 0;
    nfor(i, LOGN) {
        if (v == -1)
            break;
        int b = (x >> i) & 1;
        if ((cur | (1 << i)) >= k) {
            ans += getCnt(nt[v][b ^ 1]);
            v = nt[v][b];
        } else {
            v = nt[v][b ^ 1];
            cur |= (1 << i);
        }
    }
    if (cur >= k)
        ans += getCnt(v);
    return ans;
}

void solve() {
    clear();

    add(0);

    li ans = 0;
    int s = 0;
    forn(i, n) {
        s ^= a[i];
        li cur = calc(s);
        ans += cur;
        add(s);
    }
    cout << ans << endl;
}
