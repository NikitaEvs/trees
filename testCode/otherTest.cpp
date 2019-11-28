/*Алгоритм сжатия данных Хаффмана*/
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
template<typename T, class C>
class Haffman {
 private:
  struct node {
    T value;
    int simvol;
    node* left;
    node* right;
    node(node* LEFT, node* RIGHT) {
      left = LEFT;
      right = RIGHT;
      value = LEFT->value + RIGHT->value;
    }
    node() {
      left = nullptr;
      right = nullptr;
    }
    ~node() {
    }
    void print(int level = 0) const {
      if (this == nullptr) return;
      right->print(level + 1);
      for (int i = 0; i < level; i++) cout << "  ";
      cout << value << ' ' << endl;
      left->print(level + 1);
    }
  }
      *root = nullptr;
  C comp;
 public:
  Haffman(C comp) : comp(comp) {}
  ~Haffman() {
    queue <node* > del;
    del.push(root);
    while (del.size() != 0) {
      auto current = del.front();
      del.pop();
      if (current->left != nullptr) {
        del.push(current->left);
      }
      if (current->right != nullptr) {
        del.push(current->right);
      }
      delete current;
    }
  }
  int Partition(vector<node*>& a, int start, int end) {
    if (end > start) {
      int m = (start + end) / 2;
      swap(a[start], a[m]);
      int i = end;
      for (int j = end; j >= (start + 1); --j) {
        if (a[j]->value >= a[start]->value) {
          swap(a[i], a[j]);
          --i;
        }
      }
      swap(a[i], a[start]);
      return(i);
    }
  }
  void qsort(vector <node*>& a, int start, int end) {
    if (start < end) {
      int p = Partition(a, start, end);
      qsort(a, start, p - 1);
      qsort(a, p + 1, end);
    }
  }
  bool insert(vector<int>& Table) {
    vector<node*> AllTree;
    for (int i = 0; i < Table.size(); ++i) {
      if (Table[i] != 0) {
        node* leaf = new node();
        leaf->simvol = (i);
        leaf->value = Table[i];
        AllTree.push_back(leaf);
      }
    }
    while (AllTree.size() >= 2) {
      qsort(AllTree, AllTree.begin(), AllTree.size()-1);
      node* left = AllTree.front();
      AllTree.erase(AllTree.begin());
      node* right = AllTree.front();
      AllTree.erase(AllTree.begin());
      node* parent = new node(left, right);
      AllTree.push_back(parent);
    }
    node* root = (AllTree[0]);
  }
  void EndOfTable2() {
    vector<string> TableITOG;
    vector <string> a;
    root->EndOfTable2(TableITOG, a);
  }
  void EndOfTable2(node* current, vector<string> &TableITOG, vector <string> &a) {
    if (current->left != nullptr) {
      a.push_back("0");
      EndOfTable2(current->left);
    }
    if (current->right != nullptr) {
      a.push_back("1");
      EndOfTable2(current->right);
    }
    if (current->right == nullptr && current->left = nullptr) {
      TableITOG[current->simvol] = a;
    }
    a.pop_back();
  }
  void print2() {
    root->print();
  }
};

class int_comparer {
 public:
  bool operator()(int x, int y) {
    return x < y;
  }
};
int main() {
  int_comparer icomp;
  Haffman<int, int_comparer> tree(icomp);
  string stroka;
  vector<int> Table(257);
  getline(cin, stroka);
  for (int i = 0; i < stroka.size(); ++i) {
    Table[int(stroka[i])] += 1;
  }
  tree.insert(Table);
  tree.print2();
}