#include <bits/stdc++.h>
using namespace std;

struct Node {
    int child[26]{};
    int cnt = 0;
};

vector<Node> nodes{Node()};

int clone(int u) {
    nodes.emplace_back(nodes[u]);
    return sz(nodes) - 1;
}

int insert(int root, const string &word) {
    int newRoot = clone(root);
    nodes[newRoot].cnt++;
    int oldNode = root;
    int newNode = newRoot;
    for (char c : word) {
        int idx = c - 'a';
        int oldChild = nodes[oldNode].child[idx];
        int newChild = clone(oldChild);
        nodes[newNode].child[idx] = newChild;
        oldNode = oldChild;
        newNode = newChild;
        nodes[newNode].cnt++;
    }
    return newRoot;
}

int countPrefix(int root, const string &prefix) {
    int u = root;
    for (char c : prefix) {
        int idx = c - 'a';
        u = nodes[u].child[idx];
        if (u == 0) {
            return 0;
        }
    }
    return nodes[u].cnt;
}
