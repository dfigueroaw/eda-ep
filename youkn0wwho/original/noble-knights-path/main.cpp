#include <bits/stdc++.h>

typedef long double LD;
typedef long long LL;

using namespace std;

#define sz(A) (int)(A).size()
#define mp make_pair
#define pb push_back

const int N = int(1e5 + 5), INF = int(1e9);

struct node {
    int l, r, val;

    node() {}

    node(int a, int b, int c) { l = a, r = b, val = c; }
};

vector<int> graph[N];
int n, root, m, par[N];

int sz[N], heavy[N], depth[N];

int first[N], t = 0, sz_lca;
pair<int, int> times[2 * N], tree_lca[8 * N];

bool in_decomp[N];
int num_trees, num_tree_in[N], pos_tree_in[N], sz_ptree[N];
vector<int> tree[N];
vector<pair<int, int>> roots[N];
vector<node> ptree[N];

void dfs(int v, int d) {
    depth[v] = d;
    sz[v] = 1;

    heavy[v] = -1;
    int mx = 0;

    for (int i = 0; i < sz(graph[v]); i++) {
        int to = graph[v][i];

        if (!first[v])
            first[v] = t + 1;
        times[t++] = mp(d, v);

        if (!first[to])
            first[to] = t + 1;
        times[t++] = mp(d + 1, to);

        dfs(to, d + 1);

        sz[v] += sz[to];

        if (sz[to] > mx) {
            mx = sz[to];
            heavy[v] = to;
        }
    }
}

void build_lca() {
    sz_lca = 1;
    while (sz_lca < t)
        sz_lca <<= 1;

    for (int i = sz_lca; i < sz_lca + t; i++)
        tree_lca[i] = times[i - sz_lca];

    for (int i = sz_lca + t; i < 2 * sz_lca; i++)
        tree_lca[i] = mp(INF, -1);

    for (int i = sz_lca - 1; i > 0; i--)
        tree_lca[i] = min(tree_lca[i * 2], tree_lca[i * 2 + 1]);
}

pair<int, int> min_lca(int l, int r, int L, int R, int v) {
    if (r <= L || R <= l)
        return mp(INF, -1);
    if (l <= L && R <= r)
        return tree_lca[v];
    int mid = (L + R) / 2;
    return min(min_lca(l, r, L, mid, v * 2), min_lca(l, r, mid, R, v * 2 + 1));
}

int lca(int v1, int v2) {
    if (first[v1] > first[v2])
        swap(v1, v2);
    return min_lca(first[v1], first[v2] + 1, 1, sz_lca + 1, 1).second;
}

void dfs_heavy(int v) {
    if (!in_decomp[v]) {
        int now = v;
        while (now != -1) {
            tree[num_trees].pb(now);
            in_decomp[now] = 1;
            num_tree_in[now] = num_trees;
            pos_tree_in[now] = sz(tree[num_trees]);
            now = heavy[now];
        }
        num_trees++;
    }

    for (int i = 0; i < sz(graph[v]); i++)
        dfs_heavy(graph[v][i]);
}

node create_node(int l, int r, int num_t) {
    return node(l, r, ptree[num_t][l].val + ptree[num_t][r].val);
}

int update_ptree(int num_t, int pos, int L, int R, int v, int new_val) {
    if (L + 1 == R) {
        ptree[num_t].pb(create_node(0, 0, num_t));
        ptree[num_t][sz(ptree[num_t]) - 1].val = new_val;
        return sz(ptree[num_t]) - 1;
    }

    int mid = (L + R) / 2;

    if (pos < mid) {
        int new_v =
            update_ptree(num_t, pos, L, mid, ptree[num_t][v].l, new_val);
        ptree[num_t].pb(create_node(new_v, ptree[num_t][v].r, num_t));
    } else {
        int new_v =
            update_ptree(num_t, pos, mid, R, ptree[num_t][v].r, new_val);
        ptree[num_t].pb(create_node(ptree[num_t][v].l, new_v, num_t));
    }
    assert(R - L >= ptree[num_t][sz(ptree[num_t]) - 1].val);
    return sz(ptree[num_t]) - 1;
}

int sum_ptree(int num_t, int l, int r, int L, int R, int v1, int v2) {
    if (r <= L || R <= l)
        return 0;
    if (l <= L && R <= r)
        return (R - L) - (ptree[num_t][v1].val - ptree[num_t][v2].val);
    int mid = (L + R) / 2;
    return sum_ptree(num_t, l, r, L, mid, ptree[num_t][v1].l,
                     ptree[num_t][v2].l) +
           sum_ptree(num_t, l, r, mid, R, ptree[num_t][v1].r,
                     ptree[num_t][v2].r);
}

int num_version(int tree, int timer) {
    int l = 0, r = sz(roots[tree]);
    while (l + 1 < r) {
        int mid = (l + r) / 2;
        if (roots[tree][mid].first <= timer)
            l = mid;
        else
            r = mid;
    }
    return l;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &par[i]);

        if (par[i] == 0)
            root = i;
        else
            graph[par[i]].pb(i);
    }

    dfs(root, 1);

    build_lca();

    dfs_heavy(root);

    for (int i = 0; i < num_trees; i++) {
        int number = sz(tree[i]);

        sz_ptree[i] = 1;
        while (sz_ptree[i] < number)
            sz_ptree[i] <<= 1;
        ptree[i].resize(sz_ptree[i] * 2);
        roots[i].pb(mp(0, 1));
        for (int j = 1; j < sz_ptree[i]; j++) {
            ptree[i][j].l = j * 2;
            ptree[i][j].r = j * 2 + 1;
        }
        for (int j = sz_ptree[i]; j < 2 * sz_ptree[i]; j++) {
            if (j - sz_ptree[i] < number)
                ptree[i][j].val = 1;
            else
                ptree[i][j].val = 0;
        }
        for (int j = sz_ptree[i] - 1; j > 0; j--) {
            ptree[i][j].val = ptree[i][j * 2].val + ptree[i][j * 2 + 1].val;
        }
    }

    scanf("%d", &m);

    for (int i = 0; i < m; i++) {
        int type;
        scanf("%d", &type);

        if (type == 1) {
            int v;
            scanf("%d", &v);
            int t_num = num_tree_in[v];
            int prev_sz = sz(roots[t_num]);
            roots[t_num].pb(
                mp(i + 1,
                   update_ptree(t_num, pos_tree_in[v], 1, sz_ptree[t_num] + 1,
                                roots[t_num][prev_sz - 1].second, 0)));
        } else {
            int v1, v2, k, start;

            scanf("%d%d%d%d", &v1, &v2, &k, &start);

            int v3 = lca(v1, v2), now;

            bool ans_given = 0;

            int num_t = num_tree_in[v3];
            int vers = num_version(num_t, start), num_r = sz(roots[num_t]);

            int root_val =
                sum_ptree(num_t, pos_tree_in[v3], pos_tree_in[v3] + 1, 1,
                          sz_ptree[num_t] + 1, roots[num_t][vers].second,
                          roots[num_t][num_r - 1].second);
            int start_pos = pos_tree_in[v3];
            if (v3 == v2 || v3 == v1)
                start_pos++;

            now = par[v1];
            while (depth[now] >= depth[v3] && now != v2) {
                int num_t = num_tree_in[now], num_r = sz(roots[num_t]), sum = 0;
                int vers = num_version(num_t, start);

                if (depth[v3] < depth[tree[num_t][0]]) {
                    sum = sum_ptree(num_t, 1, pos_tree_in[now] + 1, 1,
                                    sz_ptree[num_t] + 1,
                                    roots[num_t][vers].second,
                                    roots[num_t][num_r - 1].second);
                } else {
                    sum = sum_ptree(num_t, start_pos, pos_tree_in[now] + 1, 1,
                                    sz_ptree[num_t] + 1,
                                    roots[num_t][vers].second,
                                    roots[num_t][num_r - 1].second);
                }
                if (sum < k) {
                    k -= sum;
                } else {
                    k = sum - k + 1;
                    int left_b = 1;
                    if (num_t == num_tree_in[v3])
                        left_b = start_pos;

                    int r1 = roots[num_t][num_r - 1].second,
                        r2 = roots[num_t][vers].second, L = 1,
                        R = sz_ptree[num_t] + 1;

                    while (L + 1 < R) {
                        int mid = (L + R) / 2;

                        if (sum_ptree(num_t, left_b, pos_tree_in[now] + 1, L,
                                      mid, ptree[num_t][r2].l,
                                      ptree[num_t][r1].l) < k) {
                            k -= sum_ptree(num_t, left_b, pos_tree_in[now] + 1,
                                           L, mid, ptree[num_t][r2].l,
                                           ptree[num_t][r1].l);
                            r1 = ptree[num_t][r1].r;
                            r2 = ptree[num_t][r2].r;
                            L = mid;
                        } else {
                            r1 = ptree[num_t][r1].l;
                            r2 = ptree[num_t][r2].l;
                            R = mid;
                        }
                    }

                    printf("%d\n", tree[num_t][L - 1]);
                    ans_given = 1;
                    break;
                }

                now = par[tree[num_t][0]];
            }

            if (ans_given)
                continue;

            now = par[v2];

            int sum_path = 0;

            if (v3 != v2 && v3 != v1)
                k += root_val;

            while (depth[now] >= depth[v3] && now != v1) {
                int num_t = num_tree_in[now], num_r = sz(roots[num_t]);
                int vers = num_version(num_t, start);

                if (depth[v3] < depth[tree[num_t][0]]) {
                    sum_path += sum_ptree(num_t, 1, pos_tree_in[now] + 1, 1,
                                          sz_ptree[num_t] + 1,
                                          roots[num_t][vers].second,
                                          roots[num_t][num_r - 1].second);
                } else {
                    sum_path += sum_ptree(
                        num_t, start_pos, pos_tree_in[now] + 1, 1,
                        sz_ptree[num_t] + 1, roots[num_t][vers].second,
                        roots[num_t][num_r - 1].second);
                }

                now = par[tree[num_t][0]];
            }

            if (sum_path < k) {
                printf("-1\n");
                continue;
            }

            k = sum_path - k + 1;

            now = par[v2];

            while (depth[now] >= depth[v3] && now != v1) {
                int num_t = num_tree_in[now], num_r = sz(roots[num_t]), sum = 0;
                int vers = num_version(num_t, start);

                if (depth[v3] < depth[tree[num_t][0]]) {
                    sum = sum_ptree(num_t, 1, pos_tree_in[now] + 1, 1,
                                    sz_ptree[num_t] + 1,
                                    roots[num_t][vers].second,
                                    roots[num_t][num_r - 1].second);
                } else {
                    sum = sum_ptree(num_t, start_pos, pos_tree_in[now] + 1, 1,
                                    sz_ptree[num_t] + 1,
                                    roots[num_t][vers].second,
                                    roots[num_t][num_r - 1].second);
                }

                if (sum < k) {
                    k -= sum;
                } else {
                    k = sum - k + 1;
                    int left_b = 1;
                    if (num_t == num_tree_in[v3])
                        left_b = start_pos;

                    int r1 = roots[num_t][num_r - 1].second,
                        r2 = roots[num_t][vers].second, L = 1,
                        R = sz_ptree[num_t] + 1;

                    while (L + 1 < R) {
                        int mid = (L + R) / 2;

                        if (sum_ptree(num_t, left_b, pos_tree_in[now] + 1, L,
                                      mid, ptree[num_t][r2].l,
                                      ptree[num_t][r1].l) < k) {
                            k -= sum_ptree(num_t, left_b, pos_tree_in[now] + 1,
                                           L, mid, ptree[num_t][r2].l,
                                           ptree[num_t][r1].l);
                            r1 = ptree[num_t][r1].r;
                            r2 = ptree[num_t][r2].r;
                            L = mid;
                        } else {
                            r1 = ptree[num_t][r1].l;
                            r2 = ptree[num_t][r2].l;
                            R = mid;
                        }
                    }

                    printf("%d\n", tree[num_t][L - 1]);
                    break;
                }

                now = par[tree[num_t][0]];
            }
        }
    }

    return 0;
}
