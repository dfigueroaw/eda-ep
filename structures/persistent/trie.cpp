#include <bits/stdc++.h>
using namespace std;

struct Node {
    bool end = false;
    Node *children[26]{};
};

void insert(Node *root, string word) {
    Node *current = root;
    for (const char c : word) {
        const int idx = c - 'a';
        if (current->children[idx] == nullptr) {
            current->children[idx] = new Node();
        }
        current = current->children[idx];
    }
    current->end = true;
}

bool search(Node *root, string word) {
    Node *current = root;
    for (const char c : word) {
        const int idx = c - 'a';
        if (current->children[idx] == nullptr) {
            return false;
        }
        current = current->children[idx];
    }
    return current->end;
}

bool startsWith(Node *root, string prefix) {
    Node *current = root;
    for (const char c : prefix) {
        const int idx = c - 'a';
        if (current->children[idx] == nullptr) {
            return false;
        }
        current = current->children[idx];
    }
    return true;
}
