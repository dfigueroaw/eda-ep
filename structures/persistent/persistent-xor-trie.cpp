struct Node {
    int children[2] = {-1, -1};
    int cnt = 0;
};

vector<Node> nodes;

int clone(int node) {
    if (node == -1) {
        nodes.emplace_back(Node());
        return sz(nodes) - 1;
    }
    nodes.emplace_back(nodes[node]);
    return sz(nodes) - 1;
}

int add(int root, int x) {
    int newRoot = clone(root);
    nodes[newRoot].cnt++;
    int curOld = root;
    int curNew = newRoot;
    for (int i = 30; i >= 0; i--) {
        int bit = (x >> i) & 1;
        int nextOld = (curOld == -1 ? -1 : nodes[curOld].children[bit]);
        int nextNew = clone(nextOld);
        nodes[nextNew].cnt++;
        nodes[curNew].children[bit] = nextNew;
        curOld = nextOld;
        curNew = nextNew;
    }
    return newRoot;
}

int remove(int root, int x) {
    int newRoot = clone(root);
    nodes[newRoot].cnt--;
    int curOld = root;
    int curNew = newRoot;
    for (int i = 30; i >= 0; i--) {
        int bit = (x >> i) & 1;
        int nextOld = nodes[curOld].children[bit];
        int nextNew = clone(nextOld);
        nodes[nextNew].cnt--;
        nodes[curNew].children[bit] = nextNew;
        curOld = nextOld;
        curNew = nextNew;
    }
    return newRoot;
}

int kth(int root, int k) {
    int cur = root;
    int res = 0;
    for (int i = 30; i >= 0; i--) {
        int left = nodes[cur].children[0];
        int leftCnt = (left == -1 ? 0 : nodes[left].cnt);
        if (k <= leftCnt) {
            cur = left;
        } else {
            k -= leftCnt;
            cur = nodes[cur].children[1];
            res |= (1 << i);
        }
    }

    return res;
}

int queryMin(int root, int x) {
    int cur = root;
    int res = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (x >> i) & 1;
        int same = nodes[cur].children[bit];
        if (same != -1 && nodes[same].cnt > 0) {
            cur = same;
        } else {
            cur = nodes[cur].children[!bit];
            res |= (1 << i);
        }
    }
    return res;
}

int queryMax(int root, int x) {
    int cur = root;
    int res = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (x >> i) & 1;
        int opposite = nodes[cur].children[!bit];
        if (opposite != -1 && nodes[opposite].cnt > 0) {
            cur = opposite;
            res |= (1 << i);
        } else {
            cur = nodes[cur].children[bit];
        }
    }
    return res;
}
