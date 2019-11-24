/**
 * Задание 3. Задача № 2_2
 * Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.
 * Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
 * Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
 * Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
 * При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
 * Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x, а во втором больше или равны x.
 * Получившиеся два дерева сделайте дочерними для нового узла (x, y). Новый узел вставьте на место узла P.
 * Построить также наивное дерево поиска по ключам Xi. Равные ключи добавляйте в правое поддерево.
 * Вычислить количество узлов в самом широком слое декартового дерева и количество узлов в самом широком слое наивного дерева поиска.
 * Вывести их разницу. Разница может быть отрицательна.
 */

#include <iostream>
#include <queue>

//#define DEBUG
//#define DUMP

#if defined(DEBUG) || defined(DUMP)
#include "../visLib/visualization.h"
#endif

template <typename elementType>
class Comparator {
 public:
  bool operator() (const elementType &left, const elementType &right) {
    return left < right;
  }
};

template <typename elementType, typename Comparator>
class BinarySearchTree {
 public:
  explicit BinarySearchTree(Comparator comparator) :
      comparator(comparator) {}

  ~BinarySearchTree() {
    for (auto &currentNode : usedNodes) {
      delete currentNode;
    }
  }

  bool isEmpty() const {
    return root == nullptr;
  }

  void simpleInsert(elementType item) {
    if (isEmpty()) {
      root = new Node(item);
      usedNodes.push_back(root);
    } else {
      Node *currentNode = root;

      while (currentNode != nullptr) {
        if (comparator(item, currentNode->value)) {
          if (currentNode->leftChild != nullptr) {
            currentNode = currentNode->leftChild;
          } else {
            currentNode->leftChild = new Node(item);
            usedNodes.push_back(currentNode -> leftChild);
            break;
          }
        } else  {
          if (currentNode->rightChild != nullptr) {
            currentNode = currentNode->rightChild;
          } else {
            currentNode->rightChild = new Node(item);
            usedNodes.push_back(currentNode -> rightChild);
            break;
          }
        }
      }
    }
  }

  int32_t getMaxWidth() {
    Node *currentNode = root;
    Node *firstNodeInLevel = root;
    std::queue<std::pair<Node*, int32_t> > buffer;
    buffer.push(std::make_pair(root, 0));

    int32_t maxWidth = 0, width = 0, currentLevel = 0;

    std::pair<Node*, int32_t> current = buffer.front();

    while (!buffer.empty()) {
      buffer.pop();

      if (current.second > currentLevel) {
        maxWidth = width > maxWidth ? width : maxWidth;
        width = 0;
        currentLevel = current.second;
      }

      if (current.first -> leftChild != nullptr) {
        buffer.push(std::make_pair(current.first -> leftChild, currentLevel + 1));
      }

      if (current.first -> rightChild != nullptr) {
        buffer.push(std::make_pair(current.first -> rightChild, currentLevel + 1));
      }

      current = buffer.front();
      width++;
    }

    return maxWidth;
  }

 private:
  struct Node {
    elementType value;
    Node *leftChild = nullptr;
    Node *rightChild = nullptr;

    explicit Node (elementType const &input) :value(input) {}

    void print (__int32_t level) const  {
      if (this == nullptr) return;
      rightChild->print(level + 1);
      for (int i = 0; i < level; i++) std::cout << "   ";
      std::cout << value << std::endl;
      leftChild->print(level + 1);
    }

    ~Node() = default;
  };

  Node *root = nullptr;
  std::vector<Node*> usedNodes;
  Comparator comparator;
};

template <typename keyType, typename priorityType, typename KeyComparator, typename PriorityComparator>
class Treap {
 public:
  Treap(KeyComparator keyComparator, PriorityComparator priorityComparator) {
    keyComparator_ = keyComparator;
    priorityComparator_ = priorityComparator;
  };

  ~Treap() {
    delete root;
  }

  bool isEmpty() {
    return root == nullptr;
  }

  void insert(keyType key, priorityType priority) {
#ifdef DEBUG
    visual_.logDump(root, "TREE BEFORE INSERT");
#endif

    Node *newNode = new Node(key, priority);
    Node *currentNode = root;
    Node *previousNode = nullptr;

    bool isLeft = false;

    while (currentNode != nullptr) {
      if (priorityComparator_(currentNode -> priority_, priority))
        break;
      if (keyComparator_(key, currentNode -> key_)) {
        previousNode = currentNode;
        currentNode = currentNode -> leftChild;
        isLeft = true;
      } else {
        previousNode = currentNode;
        currentNode = currentNode -> rightChild;
        isLeft = false;
      }
    }

    if (currentNode == nullptr) {
      currentNode = newNode;
    } else {
      split(currentNode, key, newNode -> leftChild, newNode -> rightChild);
      currentNode = newNode;
    }

    if (previousNode != nullptr) {
      if (isLeft) {
        previousNode -> leftChild = currentNode;
      } else {
        previousNode -> rightChild = currentNode;
      }
    } else {
      root = currentNode;
    }

#ifdef DEBUG
    visual_.logDump(root, "TREE AFTER INSERT");
#endif

#ifdef DUMP
    visual_.dump(root);
#endif
  }

  bool remove(keyType key) {
    Node *currentNode = root;
    Node *previousNode = nullptr;
    bool isLeft = false;

    while (currentNode != nullptr) {
      if (keyComparator_(key, currentNode -> key_)) {
        previousNode = currentNode;
        currentNode = currentNode -> leftChild;
        isLeft = true;
      } else if (keyComparator_(currentNode -> key_, key)) {
        previousNode = currentNode;
        currentNode = currentNode -> rightChild;
        isLeft = false;
      } else {
        Node *oldNode = currentNode;
        merge(currentNode -> leftChild, currentNode -> rightChild, currentNode);
        oldNode -> leftChild = nullptr;
        oldNode -> rightChild = nullptr;
        delete(oldNode);

        if (isLeft) {
          previousNode -> leftChild = currentNode;
        } else {
          previousNode -> rightChild = currentNode;
        }

#ifdef DUMP
        visual_.dump(root);
#endif

        return true;
      }
    }

    return false;
  }

  int32_t getMaxWidth() {
    Node *currentNode = root;
    Node *firstNodeInLevel = root;
    std::queue<std::pair<Node*, int32_t> > buffer;
    buffer.push(std::make_pair(root, 0));

    int32_t maxWidth = 0, width = 0, currentLevel = 0;

    std::pair<Node*, int32_t> current = buffer.front();

    while (!buffer.empty()) {
      buffer.pop();

      if (current.second > currentLevel) {
        maxWidth = width > maxWidth ? width : maxWidth;
        width = 0;
        currentLevel = current.second;
      }

      if (current.first -> leftChild != nullptr) {
        buffer.push(std::make_pair(current.first -> leftChild, currentLevel + 1));
      }

      if (current.first -> rightChild != nullptr) {
        buffer.push(std::make_pair(current.first -> rightChild, currentLevel + 1));
      }

      current = buffer.front();
      width++;
    }

    return maxWidth;
  }

 private:
  struct Node {
    Node *leftChild = nullptr;
    Node *rightChild = nullptr;

    keyType key_;
    priorityType priority_;

    Node(keyType key, priorityType priority) {
      key_ = key;
      priority_ = priority;
    }

    ~Node() {
      if (this != nullptr) {
        delete leftChild;
        leftChild = nullptr;
        delete rightChild;
        rightChild = nullptr;
      }
    }
  };

  void split(Node *previousRoot, keyType key, Node *&resultLeftRoot, Node *&resultRightRoot) {
#ifdef DEBUG
    visual_.logDump(previousRoot, "TREE BEFORE SPLIT");
#endif

    if (previousRoot == nullptr) {
      resultLeftRoot = nullptr;
      resultRightRoot = nullptr;
      return;
    } else if (keyComparator_(previousRoot -> key_, key)) {
      split(previousRoot -> rightChild, key, previousRoot -> rightChild, resultRightRoot);
      resultLeftRoot = previousRoot;
    } else {
      split(previousRoot -> leftChild, key, resultLeftRoot, previousRoot -> leftChild);
      resultRightRoot = previousRoot;
    }

#ifdef DEBUG
    visual_.logDump(resultLeftRoot, "LEFT TREE AFTER SPLIT");
    visual_.logDump(resultRightRoot, "RIGHT TREE AFTER SPLIT");
#endif
  }

  void merge(Node *leftRoot, Node *rightRoot, Node *&resultRoot) {
#ifdef DEBUG
    visual_.logDump(leftRoot, "LEFT TREE BEFORE MERGE");
    visual_.logDump(rightRoot, "RIGHT ROOT BEFORE MERGE");
#endif

    if ((rightRoot == nullptr) && (leftRoot == nullptr)) {
      resultRoot = nullptr;
      return;
    }
    if (rightRoot == nullptr) {
      resultRoot = leftRoot;
      return;
    }
    if (leftRoot == nullptr) {
      resultRoot = rightRoot;
      return;
    }

    if (priorityComparator_(rightRoot -> priority_, leftRoot -> priority_)) {
      merge(leftRoot -> rightChild, rightRoot, leftRoot -> rightChild);
      resultRoot = leftRoot;
    } else {
      merge(rightRoot -> leftChild, leftRoot, rightRoot -> leftChild);
      resultRoot = rightRoot;
    }



#ifdef DEBUG
    visual_.logDump(resultRoot, "RESULT TREE AFTER MERGE");
#endif
  }

  Node *root = nullptr;
  KeyComparator keyComparator_;
  PriorityComparator priorityComparator_;


#if defined(DEBUG) || defined(DUMP)
  Visual<Node, keyType> visual_;
#endif

};

int main() {
  Comparator<int32_t> comparator;
  Treap<int32_t, int32_t, Comparator<int32_t>, Comparator<int32_t> > treap(comparator, comparator);
  BinarySearchTree<int32_t, Comparator<int32_t> > binarySearchTree(comparator);

  int32_t n;
  std::cin >> n;

  for (int32_t i = 0; i < n; i++) {
    int32_t key, priority;
    std::cin >> key >> priority;
    treap.insert(key, priority);
    binarySearchTree.simpleInsert(key);
  }

  std::cout << treap.getMaxWidth() - binarySearchTree.getMaxWidth() << std::endl;

  return 0;
}