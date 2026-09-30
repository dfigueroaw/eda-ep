/* ~ The Diamond Sky ~ */
#include <bits/stdc++.h>
using namespace std;

template <class SNguyen> bool maximize(SNguyen &a, const SNguyen b) {
    if (a < b) {
        a = b;
        return true;
    }
    return false;
}

template <class SNguyen> bool minimize(SNguyen &a, const SNguyen b) {
    if (a > b) {
        a = b;
        return true;
    }
    return false;
}

#define Double long double
#define Int long long
#define SNC cin.tie(0)->ios::sync_with_stdio(false);
#define For(i, a, b) for (int i = a; i <= b; i++)
#define Rep(i, b, a) for (int i = b; i >= a; i--)
#define Fa(i, x) for (auto &i : x)
#define _sz(x) (int)x.size()
#define MASK(i) ((1LL << (i)))
#define IsMask(S, i) (((S) >> (i)) & 1)
#define ii pair<int, int>
#define II pair<Int, Int>
#define iii pair<int, ii>
#define fr first
#define sc second
#define all(x) x.begin(), x.end()
#define name "SNC"
#define spc " "
#define endl "\n"

void readfile() {
    if (fopen(name ".INP", "r")) {
        freopen(name ".INP", "r", stdin);
        freopen(name ".OUT", "w", stdout);
    }
}

const int MaxNode = 15000000;
const int mod = 1e9;
const int inf = 2e9 + 18082009;
const Int INF = 8e18 + 18082009;
const int N = 2e5 + 10;
typedef int arr[N];
typedef Int Arr[N];
typedef vector<int> vi;
typedef vector<ii> vii;
typedef vector<iii> viii;
typedef vector<Int> vI;
typedef vector<II> vII;

/*
    END OF TEMPLATE!!! SNGUYEN COMPILER WILL WIN VOI 27 NO MATTER WHAT!!!
    THANK YOU FOR EVERYTHING!!!
*/

int n, q, lim = 2e5 + 5;
arr pstVer;

struct node {
    Int sumA, sumB, lazy_a, lazy_b;
    int left_node, right_node;

    node(Int a = 0, Int b = 0, Int e = 0, Int f = 0, int c = 0, int d = 0)
        : sumA(a), sumB(b), left_node(c), right_node(d), lazy_a(e), lazy_b(f) {}
};

struct Persistent_SegmentTree {
    node st[MaxNode];
    int NumNode, Num;
    vi versions;

    void BuildTree(int n) {
        Num = n;
        NumNode = 0;
        versions.emplace_back(build(0, Num));
    }

    int build(int l, int r) {
        int id = ++NumNode;
        if (l == r) {
            st[id] = node();
            return id;
        }
        int mid = (l + r) >> 1;
        st[id].left_node = build(l, mid);
        st[id].right_node = build(mid + 1, r);
        return id;
    }

    void merge(int id, int l, int r) {
        st[id].sumA = st[st[id].left_node].sumA + st[st[id].right_node].sumA +
                      st[id].lazy_a * (r - l + 1);
        st[id].sumB = st[st[id].left_node].sumB + st[st[id].right_node].sumB +
                      st[id].lazy_b * (r - l + 1);
    }

    int update(int oldVer, int l, int r, int u, int v, Int A, Int B) {
        int id = ++NumNode;
        st[id] = st[oldVer];
        if (u <= l && r <= v) {
            st[id].sumA += A * (r - l + 1);
            st[id].sumB += B * (r - l + 1);
            st[id].lazy_a += A;
            st[id].lazy_b += B;
            return id;
        }
        int mid = (l + r) >> 1;
        if (u <= mid)
            st[id].left_node = update(st[id].left_node, l, mid, u, v, A, B);
        if (mid < v)
            st[id].right_node =
                update(st[id].right_node, mid + 1, r, u, v, A, B);
        merge(id, l, r);
        return id;
    }

    void updateVer(int l, int r, Int A, Int B) {
        if (l > r)
            return;
        versions.emplace_back(update(versions.back(), 0, Num, l, r, A, B));
    }

    II get(int id, int l, int r, int pos, Int A, Int B) {
        if (!id || pos < l || pos > r)
            return II(0, 0);
        if (l == r)
            return II(st[id].sumA + A, st[id].sumB + B);
        Int new_A = A + st[id].lazy_a;
        Int new_B = B + st[id].lazy_b;
        int mid = (l + r) >> 1;
        II Lefty = get(st[id].left_node, l, mid, pos, new_A, new_B);
        II Righty = get(st[id].right_node, mid + 1, r, pos, new_A, new_B);
        return II(Lefty.fr + Righty.fr, Lefty.sc + Righty.sc);
    }

    II query(int verL, int verR, int x) {
        II L = get(verL, 0, Num, x, 0, 0);
        II R = get(verR, 0, Num, x, 0, 0);
        return II(R.fr - L.fr, R.sc - L.sc);
    }
} IT;

void Input() { cin >> n; }

void Solve() {
    IT.BuildTree(lim);
    pstVer[0] = IT.versions.back();
    For(i, 1, n) {
        int l, r;
        Int Y1, Y2, A, B;
        cin >> l >> r >> Y1 >> A >> B >> Y2;
        IT.updateVer(0, l, 0, Y1);
        IT.updateVer(l + 1, r, A, B);
        IT.updateVer(r + 1, lim, 0, Y2);
        pstVer[i] = IT.versions.back();
    }
    cin >> q;
    Int prev_ans = 0;
    For(_, 1, q) {
        int l, r;
        Int x;
        cin >> l >> r >> x;
        x = (x + prev_ans) % mod;
        Int posX = min(x, (Int)lim);
        II ret = IT.query(pstVer[l - 1], pstVer[r], (int)posX);
        Int ans = ret.fr * x + ret.sc;
        prev_ans = ans;
        cout << ans << endl;
    }
}

signed main() {
    SNC;
    readfile();
    Input();
    Solve();
    return 0;
}
