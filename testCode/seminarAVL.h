#include <string>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <set>
using namespace std;

struct AVLNode {
  AVLNode *left, *right;
  string key;
  int height;
  AVLNode(string const &_key) : key(_key) {
    left = right = nullptr;
    height = 1;
  }

  ~AVLNode() {
    delete left;
    delete right;
  }

  int getBalanceFactor() const {
    int r = right == nullptr ? 0 : right->height;
    int l = left == nullptr ? 0 : left->height;
    return r - l;
  }
  void fix() {
    int r = right == nullptr ? 0 : right->height;
    int l = left == nullptr ? 0 : left->height;
    height = (r > l ? r : l) + 1;
  }

  AVLNode *insert(string const &_key) {
    if (_key < key)
      left = left == nullptr ? new AVLNode(_key) : left->insert(_key);
    else
      right = right == nullptr ? new AVLNode(_key) : right->insert(_key);
    return balance();
  }
  AVLNode *findMinimum() {
    return left != nullptr ? left->findMinimum() : this;
  }

  AVLNode *removeMinimum() {
    if (left == nullptr) return right;
    left = left->removeMinimum();
    return balance();
  }

  static AVLNode *remove(AVLNode *p, string const &_key) {
    if (p == nullptr) return nullptr;
    if (_key < p->key) {
      p->left = remove(p->left, _key);
      return p->balance();
    } else if (_key > p->key) {
      p->right = remove(p->right, _key);
      return p->balance();
    } else {
      AVLNode *l = p->left;
      AVLNode *r = p->right;
      p -> right = nullptr;
      p -> left = nullptr;
      delete p;
      if (r == nullptr) return l;
      AVLNode *min = r->findMinimum();
      min->right = r->removeMinimum();
      min->left = l;
      return min->balance();
    }
  }

  AVLNode *rotateRight() {
    auto result = left;
    left = result -> right;
    result -> right = this;
    fix();
    result -> fix();
    return result;
  }

  AVLNode *rotateLeft() {
    auto result = right;
    right = result -> left;
    result -> left = this;
    fix();
    result -> fix();
    return result;
  }

  AVLNode *balance() {
    fix();

    switch (getBalanceFactor()) {
      case -2:
        if (left -> getBalanceFactor() > 0) {
          left = left -> rotateLeft();
        }
        return rotateRight();
      case 2:
        if (right -> getBalanceFactor() < 0) {
          right = right -> rotateRight();
        }
        return rotateLeft();
      default:
        return this;
    }
  }

  void print(int32_t level) {
    if (this == nullptr) return;
    right->print(level + 1);
    for (int i = 0; i < level; i++) std::cout << "   ";
    std::cout << key << std::endl;
    left->print(level + 1);
  }
};

struct AVLTree {
  AVLNode *root;
  AVLTree() {
    root = nullptr;
  }

  ~AVLTree() {
    delete root;
  }

  void print() const {
    if (root != nullptr) root->print(0);
  }

  bool insert(string const &_key) {
    if (root == nullptr) root = new AVLNode(_key);
    else root = root->insert(_key);
    return true;
  }

  bool remove(string const &_key) {
    root = AVLNode::remove(root, _key);
    return true;
  }
};