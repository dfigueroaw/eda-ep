#include <bits/stdc++.h>

using namespace std;

const int p1 = 31;
const int p2 = 53;

unsigned long long pow1[32];
unsigned long long pow2[32];

struct Node {
    unsigned long long hsh = 0;
    Node *l = 0, *r = 0;
    bool was = false;
};

const int max_memory = 13e7;

int pos_memory = 0;
char memory[max_memory];

void *operator new(size_t n) {
    char *res = memory + pos_memory;
    pos_memory += n;
    assert(pos_memory <= max_memory);
    return (void *)res;
}

void operator delete(void *) {}

Node *upd(Node *v, unsigned long long x, int i = 18) {
    if (i == -1) {
        Node *u = new Node();

        if (!v || v && !v->was) {
            u->hsh = ((int(1e9) + 345) ^ x) * (3 * x + 654);
            u->was = true;
        }

        return u;
    }

    if (x & (1 << i)) {
        Node *res = new Node();
        res->l = (v ? v->l : 0);
        res->r = upd((v ? v->r : 0), x, i - 1);
        unsigned long long a = (res->l ? res->l->hsh * pow1[i + 1] : 0);
        unsigned long long b = (res->r ? res->r->hsh * pow2[i + 1] : 0);
        res->hsh = a + b;
        return res;
    } else {
        Node *res = new Node();
        res->l = upd((v ? v->l : 0), x, i - 1);
        res->r = (v ? v->r : 0);
        unsigned long long a = (res->l ? res->l->hsh * pow1[i + 1] : 0);
        unsigned long long b = (res->r ? res->r->hsh * pow2[i + 1] : 0);
        res->hsh = a + b;
        return res;
    }
}

int get(Node *l, Node *r, int i = 18, int now = 0) {
    if ((l ? l->hsh : 0) == (r ? r->hsh : 0))
        return -1;

    if (i == -1)
        return now;

    if ((l && l->l ? l->l->hsh : 0) == (r && r->l ? r->l->hsh : 0))
        return get((l ? l->r : 0), (r ? r->r : 0), i - 1, now + (1 << i));

    return get((l ? l->l : 0), (r ? r->l : 0), i - 1, now);
}

void Solve() {
    pow1[0] = 1;
    pow2[0] = 1;

    for (int i = 1; i <= 31; i++) {
        pow1[i] = p1 * pow1[i - 1];
        pow2[i] = p2 * pow2[i - 1];
    }

    int n;
    cin >> n;
    vector<pair<int, int>> a(n);
    vector<int> mp(n + 1);

    {
        for (int i = 0; i < n; i++) {
            cin >> a[i].first;
            a[i].second = i;
        }

        sort(a.begin(), a.end());
        map<int, int> kek;
        int now = 0;

        for (int i = 0; i < n; i++) {
            if (a[i].first != (i ? a[i - 1].first : -1))
                now++;

            kek[a[i].first] = now;
        }

        sort(a.begin(), a.end(), [](pair<int, int> x, pair<int, int> y) {
            return x.second < y.second;
        });

        for (int i = 0; i < n; i++) {
            int x = kek[a[i].first];
            mp[x] = a[i].first;
            a[i].first = x;
        }
    }

    vector<Node *> t(n + 1);

    t[0] = new Node();

    for (int i = 1; i <= n; i++) {
        int x = a[i - 1].first;
        t[i] = upd(t[i - 1], x);
    }

    int q;
    cin >> q;

    int last = 0;

    while (q--) {
        int a, b;
        cin >> a >> b;

        int l = (last ^ a);
        int r = (last ^ b);

        int kek = get(t[l - 1], t[r]);
        int ans = (kek != -1 ? mp[kek] : 0);
        cout << ans << '\n';
        last = ans;
    }
}

signed main() {
    ios_base::sync_with_stdio(NULL);
    cin.tie(NULL);
    cout.tie(NULL);

    Solve();
}
