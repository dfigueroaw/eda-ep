#include <vector>

using namespace std;

struct Node {
  Node *left, *right;
  int sum;

  Node(int val) : left(nullptr), right(nullptr), sum(val) {}
  Node(Node *l, Node *r) : left(l), right(r), sum(0) {
    if (l) {
      sum += l->sum;
    }
    if (r) {
      sum += r->sum;
    }
  }
};

Node *build(const vector<int> &a, int tl, int tr) {
  if (tl == tr) {
    return new Node(a[tl]);
  }
  int tm = (tl + tr) / 2;
  return new Node(build(a, tl, tm), build(a, tm + 1, tr));
}

int get_sum(Node *v, int tl, int tr, int l, int r) {
  if (l > r) {
    return 0;
  }
  if (l == tl && r == tr) {
    return v->sum;
  }
  int tm = (tl + tr) / 2;
  return get_sum(v->left, tl, tm, l, min(r, tm)) +
         get_sum(v->right, tm + 1, tr, max(l, tm + 1), r);
}

Node *update(Node *v, int tl, int tr, int pos, int new_val) {
  if (tl == tr) {
    return new Node(new_val);
  }
  int tm = (tl + tr) / 2;
  if (pos <= tm) {
    return new Node(update(v->left, tl, tm, pos, new_val), v->right);
  } else {
    return new Node(v->left, update(v->right, tm + 1, tr, pos, new_val));
  }
}
