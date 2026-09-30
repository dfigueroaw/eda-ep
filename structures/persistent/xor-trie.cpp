#include <bits/stdc++.h>
using namespace std;

struct Node {
    Node *children[2] = {};
    int cnt = 0;
};

void add(Node *root, int x) {
    Node *cur = root;
    for (int i = 30; i >= 0; i--) {
        bool has_bit = x & (1 << i);
        if (cur->children[has_bit] == nullptr) {
            cur->children[has_bit] = new Node;
        }
        cur->children[has_bit]->cnt++;
        cur = cur->children[has_bit];
    }
}

void remove(Node *root, int x) {
    Node *cur = root;
    for (int i = 30; i >= 0; i--) {
        bool has_bit = x & (1 << i);
        cur->children[has_bit]->cnt--;
        cur = cur->children[has_bit];
    }
}

int queryMin(Node *root, int x) {
    Node *cur = root;
    int res = 0;
    for (int i = 30; i >= 0; i--) {
        bool has_bit = x & (1 << i);
        if (cur->children[has_bit] != nullptr &&
            cur->children[has_bit]->cnt > 0) {
            cur = cur->children[has_bit];
        } else {
            cur = cur->children[!has_bit];
            res += 1 << i;
        }
    }
    return res;
}

int queryMax(Node *root, int x) {
    Node *cur = root;
    int res = 0;
    for (int i = 30; i >= 0; i--) {
        bool has_bit = x & (1 << i);
        if (cur->children[!has_bit] != nullptr &&
            cur->children[!has_bit]->cnt > 0) {
            cur = cur->children[!has_bit];
            res += 1 << i;
        } else {
            cur = cur->children[has_bit];
        }
    }
    return res;
}
