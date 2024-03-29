#ifndef TREES_AVL_H_
#define TREES_AVL_H_
  #include <vector>
  #include <stack>
  #include <cstdint>

  //#define DEBUG

  #ifdef DEBUG
  #include "../visLib/visualization.h"
  #endif

  template <typename elementType, typename Comparator>
  class BinarySearchTree {
   public:

    explicit BinarySearchTree(Comparator comparator):
        comparator(comparator) {}

    ~BinarySearchTree() {
      delete root;
    }

    void simpleInsert(elementType item) {
      if (isEmpty()) {
        root = new Node(item);
      } else {
        Node *currentNode = root;

        while (currentNode != nullptr) {
          if (comparator(item, currentNode -> value)) {
            if (currentNode -> leftChild != nullptr) {
              currentNode = currentNode -> leftChild;
            } else {
              currentNode -> leftChild = new Node(item);
              break;
            }
          } else if (comparator(currentNode -> value, item)) {
            if (currentNode -> rightChild != nullptr) {
              currentNode = currentNode -> rightChild;
            } else {
              currentNode -> rightChild = new Node(item);
              break;
            }
          } else {
            return;
          }
        }
      }
    }

    void balancedInsert(elementType item) {
      if (isEmpty()) {
        root = new Node(item);
      }  else {
        insert(root, item);
      }
    }

    bool isExist(elementType item) {
      return findNode(item) != nullptr;
    }

    bool balancedErase(elementType item) {
      if (isEmpty()) return false;
      Node *currentNode = root;
      while (currentNode != nullptr) {
        if (item > currentNode -> value) {
          if (currentNode -> rightChild != nullptr) {
            currentNode = currentNode -> rightChild;
          } else {
            return false;
          }
        } else if (item < currentNode -> value){
          if (currentNode -> leftChild != nullptr) {
            currentNode = currentNode -> leftChild;
          } else {
            return false;
          }
        } else {
          break;
        }
      }
      erase(currentNode);
      return true;
    }

    elementType select(int32_t orderStatistic) {
      Node* currentNode = root;
      while (currentNode != nullptr) {
        int32_t leftChildCount = currentNode -> leftChildCount;
        if (leftChildCount <= orderStatistic) {
          orderStatistic -= leftChildCount;
          if (orderStatistic == 0) {
            return currentNode -> value;
          }
          orderStatistic--;
          currentNode = currentNode -> rightChild;
        } else {
          currentNode = currentNode -> leftChild;
        }
      }
      return 0;
    }

    bool isEmpty() {
      return root == nullptr;
    }

    std::vector<elementType> inOrderTraversal() {
      return inOrder(root);
    }

  #ifdef DEBUG
    void visualize() {
      visual.dump(root);
    }

    void visualize(int32_t index) {
      visual.logDump(root, const_cast<char *>(std::to_string(index).c_str()));
    }
  #endif

   private:

    struct Node {
      elementType value;
      Node *leftChild = nullptr;
      Node *rightChild = nullptr;
      Node *parent = nullptr;
      int32_t diffHeight = 0; // h(R) - h(L)
      int32_t rightChildCount = 0;
      int32_t leftChildCount = 0;

      explicit Node (elementType const &item):
          value(item) {}

      Node (elementType const &item, Node *parent):
          value(item),
          parent(parent) {}

      ~Node() {
        if (this != nullptr) {
          delete leftChild;
          leftChild = nullptr;
          delete rightChild;
          rightChild = nullptr;
        }
      }
    };

    Node* findNode(elementType item) {
      if (isEmpty()) {
        return nullptr;
      } else {
        Node *currentNode = root;
        while (currentNode != nullptr) {
          if (comparator(item, currentNode -> value)) {
            currentNode = currentNode -> leftChild;
          } else if (comparator(currentNode -> value, item)) {
            currentNode = currentNode -> rightChild;
          } else {
            return currentNode;
          }
        }
      }
      return nullptr;
    }

    void rotateRight(Node* &rotatingNode) {
      Node *previousParent = rotatingNode -> parent;
      bool isLeft = false;
      bool isRoot = false;
      if (previousParent == nullptr) {
        isRoot = true;
      } else if (rotatingNode == previousParent -> leftChild) {
        isLeft = true;
      }
      Node *previousLeftChild = rotatingNode -> leftChild;
      previousLeftChild -> parent = rotatingNode -> parent;
      rotatingNode -> leftChild = previousLeftChild -> rightChild;
      if (rotatingNode -> leftChild != nullptr) {
        rotatingNode -> leftChild -> parent = rotatingNode;
      }
      previousLeftChild -> rightChild = rotatingNode;
      rotatingNode -> parent = previousLeftChild;

      if (previousLeftChild -> diffHeight == 0) {
        rotatingNode -> diffHeight = -1;
        previousLeftChild -> diffHeight = 1;
      } else {
        rotatingNode -> diffHeight = 0;
        previousLeftChild -> diffHeight = 0;
      }
      rotatingNode -> leftChildCount = previousLeftChild -> rightChildCount;
      previousLeftChild -> rightChildCount = rotatingNode -> rightChildCount + rotatingNode -> leftChildCount + 1;
      rotatingNode = previousLeftChild;
      if (isRoot) {
        root = rotatingNode;
      } else if (isLeft) {
        previousParent -> leftChild = rotatingNode;
      } else {
        previousParent -> rightChild = rotatingNode;
      }
    }

    void rotateLeft(Node* &rotatingNode) {
      Node *previousParent = rotatingNode -> parent;
      bool isLeft = false;
      bool isRoot = false;
      if (previousParent == nullptr) {
        isRoot = true;
      } else if (rotatingNode == previousParent -> leftChild) {
        isLeft = true;
      }
      Node *previousRightChild = rotatingNode -> rightChild;
      previousRightChild -> parent = rotatingNode -> parent;
      rotatingNode -> rightChild = previousRightChild -> leftChild;
      if (rotatingNode -> rightChild != nullptr) {
        rotatingNode -> rightChild -> parent = rotatingNode;
      }
      previousRightChild -> leftChild = rotatingNode;
      rotatingNode -> parent = previousRightChild;

      if (previousRightChild -> diffHeight == 0) {
        rotatingNode -> diffHeight = 1;
        previousRightChild -> diffHeight = -1;
      } else {
        rotatingNode -> diffHeight = 0;
        previousRightChild -> diffHeight = 0;
      }
      rotatingNode -> rightChildCount = previousRightChild -> leftChildCount;
      previousRightChild -> leftChildCount = rotatingNode -> rightChildCount + rotatingNode -> leftChildCount + 1;
      rotatingNode = previousRightChild;
      if (isRoot) {
        root = rotatingNode;
      } else if (isLeft) {
        previousParent -> leftChild = rotatingNode;
      } else {
        previousParent -> rightChild = rotatingNode;
      }
    }

    void rotateRightLeft(Node* &rotatingNode) {
      Node *previousParent = rotatingNode -> parent;
      bool isLeft = false;
      bool isRoot = false;
      if (previousParent == nullptr) {
        isRoot = true;
      } else if (rotatingNode == previousParent -> leftChild) {
        isLeft = true;
      }

      Node *pivot = rotatingNode -> rightChild;
      Node *bottom = pivot -> leftChild;
      Node *swapChildRight = bottom -> rightChild;

      int32_t bottomLeftChildCount = bottom -> leftChildCount;
      int32_t bottomRightChildCount = bottom -> rightChildCount;

      pivot -> leftChild = swapChildRight;

      if (swapChildRight != nullptr) {
        swapChildRight -> parent = pivot;
      }

      bottom -> rightChild = pivot;
      pivot -> parent = bottom;
      Node *swapChildLeft = bottom -> leftChild;
      rotatingNode -> rightChild = swapChildLeft;
      if (swapChildLeft != nullptr) {
        swapChildLeft -> parent = rotatingNode;
      }
      bottom -> leftChild = rotatingNode;
      rotatingNode -> parent = bottom;
      if (bottom -> diffHeight > 0) {
        rotatingNode -> diffHeight = -1;
        pivot -> diffHeight = 0;
      } else {
        if (bottom -> diffHeight == 0) {
          rotatingNode -> diffHeight = 0;
          pivot -> diffHeight = 0;
        } else {
          rotatingNode -> diffHeight = 0;
          pivot -> diffHeight = 1;
        }
      }
      bottom -> diffHeight = 0;

      pivot -> leftChildCount = bottomRightChildCount;
      rotatingNode -> rightChildCount = bottomLeftChildCount;
      bottom -> rightChildCount = pivot -> rightChildCount + pivot -> leftChildCount + 1;
      bottom -> leftChildCount = rotatingNode -> rightChildCount + rotatingNode -> leftChildCount + 1;

      rotatingNode = bottom;
      rotatingNode -> parent = previousParent;
      if (isRoot) {
        root = rotatingNode;
      } else if (isLeft) {
        previousParent -> leftChild = rotatingNode;
      } else {
        previousParent -> rightChild = rotatingNode;
      }
    }

    void rotateLeftRight(Node* &rotatingNode) {
      Node *previousParent = rotatingNode -> parent;
      bool isLeft = false;
      bool isRoot = false;
      if (previousParent == nullptr) {
        isRoot = true;
      } else if (rotatingNode == previousParent -> leftChild) {
        isLeft = true;
      }


      Node *pivot = rotatingNode -> leftChild;
      Node *bottom = pivot -> rightChild;
      Node *swapChildLeft = bottom -> leftChild;

      int32_t bottomLeftChildCount = bottom -> leftChildCount;
      int32_t bottomRightChildCount = bottom -> rightChildCount;

      pivot -> rightChild = swapChildLeft;

      if (swapChildLeft != nullptr) {
        swapChildLeft -> parent = pivot;
      }

      bottom -> leftChild = pivot;
      pivot -> parent = bottom;
      Node *swapChildRight = bottom -> rightChild;
      rotatingNode -> leftChild = swapChildRight;
      if (swapChildRight != nullptr) {
        swapChildRight -> parent = rotatingNode;
      }
      bottom -> rightChild = rotatingNode;
      rotatingNode -> parent = bottom;
      if (bottom -> diffHeight > 0) {
        rotatingNode -> diffHeight = 0;
        pivot -> diffHeight = -1;
      } else {
        if (bottom -> diffHeight == 0) {
          rotatingNode -> diffHeight = 0;
          pivot -> diffHeight = 0;
        } else {
          rotatingNode -> diffHeight = 1;
          pivot -> diffHeight = 0;
        }
      }
      bottom -> diffHeight = 0;
      pivot -> rightChildCount = bottomLeftChildCount;
      rotatingNode -> leftChildCount = bottomRightChildCount;
      bottom -> rightChildCount = rotatingNode -> leftChildCount + rotatingNode -> rightChildCount + 1;
      bottom -> leftChildCount = pivot -> leftChildCount + pivot -> rightChildCount + 1;
      rotatingNode = bottom;
      rotatingNode -> parent = previousParent;
      if (isRoot) {
        root = rotatingNode;
      } else if (isLeft) {
        previousParent -> leftChild = rotatingNode;
      } else {
        previousParent -> rightChild = rotatingNode;
      }
    }

    void insert(Node* currentNode, elementType item) {
      bool isLeft = false;
      Node *previousNode = nullptr;
      while (currentNode != nullptr) {
        if (comparator(item, currentNode -> value)) {
          currentNode -> leftChildCount++;
          previousNode = currentNode;
          currentNode = currentNode -> leftChild;
          isLeft = true;
        } else if (comparator(currentNode -> value, item)) {
          currentNode -> rightChildCount++;
          previousNode = currentNode;
          currentNode = currentNode -> rightChild;
          isLeft = false;
        } else {
          return;
        }
      }

      currentNode = new Node(item, previousNode);
      if (previousNode) {
        if (isLeft) {
          previousNode -> leftChild = currentNode;
        } else {
          previousNode -> rightChild = currentNode;
        }
      }

      balance(currentNode);
    }

    void erase(Node* currentNode) {
      if ((currentNode -> rightChild == nullptr) && (currentNode -> leftChild == nullptr)) { // Sinple delete
        eraseBalance(currentNode);
        if (currentNode -> parent != nullptr) {
          if (currentNode == currentNode -> parent -> leftChild) {
            currentNode -> parent -> leftChild = nullptr;
          } else {
            currentNode -> parent -> rightChild = nullptr;
          }
        } else {
          root = nullptr;
        }
        currentNode -> leftChild = nullptr;
        currentNode -> rightChild = nullptr;
        delete currentNode;
      } else if (currentNode -> rightChild == nullptr) {
        eraseBalance(currentNode);
        if (currentNode -> parent != nullptr) {
          if (currentNode == currentNode -> parent -> leftChild) {
            currentNode -> parent -> leftChild = currentNode -> leftChild;
            currentNode -> leftChild -> parent = currentNode -> parent;
          } else {
            currentNode -> parent -> rightChild = currentNode -> leftChild;
            currentNode -> leftChild -> parent = currentNode -> parent;
          }
        } else {
          root = currentNode -> leftChild;
          currentNode -> leftChild -> parent = nullptr;
        }
        currentNode -> leftChild = nullptr;
        currentNode -> rightChild = nullptr;
        delete currentNode;
      } else if (currentNode -> leftChild == nullptr) {
        eraseBalance(currentNode);
        if (currentNode -> parent != nullptr) {
          if (currentNode == currentNode -> parent -> leftChild) {
            currentNode -> parent -> leftChild = currentNode -> rightChild;
            currentNode -> rightChild -> parent = currentNode -> parent;
          } else {
            currentNode -> parent -> rightChild = currentNode -> rightChild;
            currentNode -> rightChild -> parent = currentNode -> parent;
          }
        } else {
          root = currentNode -> rightChild;
          currentNode -> rightChild -> parent = nullptr;
        }
        currentNode -> leftChild = nullptr;
        currentNode -> rightChild = nullptr;
        delete currentNode;
      } else {
        Node* nodeForSwap = currentNode -> leftChild;
        while (nodeForSwap -> rightChild != nullptr) {
          nodeForSwap = nodeForSwap -> rightChild;
        }
        currentNode -> value = nodeForSwap -> value;
        erase(nodeForSwap);
      }
    }

    void balance(Node* currentNode) {
      if (currentNode == nullptr) return;
      for (Node* parent = currentNode -> parent; parent != nullptr; parent = currentNode -> parent) {
        if (currentNode == parent -> rightChild) {
          if (parent -> diffHeight > 0) { // Rotate left or rightLeft
            if (currentNode -> diffHeight < 0) {
              rotateRightLeft(parent);
              break;
            } else {
              rotateLeft(parent);
              break;
            }
          } else if (parent -> diffHeight < 0) {
            parent -> diffHeight = 0;
            break;
          } else {
            parent -> diffHeight = 1;
            currentNode = parent;
            continue;
          }
        } else {
          if (parent -> diffHeight < 0) { // Rotate right or leftRight
            if (currentNode -> diffHeight > 0) {
              rotateLeftRight(parent);
              break;
            } else {
              rotateRight(parent);
              break;
            }
          } else if (parent -> diffHeight > 0) {
            parent -> diffHeight = 0;
            break;
          } else {
            parent -> diffHeight = -1;
            currentNode = parent;
            continue;
          }
        }
      }
    }

    void calculateChild(Node *currentNode) {
      if (currentNode == nullptr) return;;
      while (currentNode -> parent != nullptr) {
        if (currentNode == currentNode -> parent -> rightChild) {
          currentNode -> parent -> rightChildCount--;
        } else {
          currentNode -> parent -> leftChildCount--;
        }
        currentNode = currentNode -> parent;
      }
    }

    void eraseBalance(Node* currentNode) {
      if (currentNode == nullptr) return;
      for (Node* parent = currentNode -> parent; parent != nullptr; parent = parent -> parent) {
        if (currentNode == parent -> rightChild) {
          parent -> rightChildCount--;
          if (parent -> diffHeight < 0) { // Rotate right or leftRight
            if (parent -> leftChild -> diffHeight > 0) {
              rotateLeftRight(parent);
            } else {
              rotateRight(parent);
            }
            if (parent -> diffHeight != 0) {
              calculateChild(parent);
              break;
            } else {
              currentNode = parent;
            }
          } else if (parent -> diffHeight > 0) {
            parent -> diffHeight = 0;
            currentNode = parent;
            continue;
          } else {
            parent -> diffHeight = -1;
            calculateChild(parent);
            break;
          }
        } else {
          parent -> leftChildCount--;
          if (parent -> diffHeight > 0) { // Rotate left or rightLeft
            if (parent -> rightChild -> diffHeight < 0) {
              rotateRightLeft(parent);
            } else {
              rotateLeft(parent);
            }
            if (parent -> diffHeight != 0) {
              calculateChild(parent);
              break;
            } else {
              currentNode = parent;
            }
          } else if (parent -> diffHeight < 0) {
            parent -> diffHeight = 0;
            currentNode = parent;
            continue;
          } else {
            parent -> diffHeight = 1;
            calculateChild(parent);
            break;
          }
        }
      }
    }

    std::vector<elementType> inOrder(Node* startNode) {
      std::stack<Node *> stack;
      std::vector<elementType> buffer;

      if (startNode == nullptr) return buffer;

      Node *currentNode = startNode;

      while (currentNode != nullptr || !stack.empty()) {
        while (currentNode != nullptr) {
          stack.push(currentNode);
          currentNode = currentNode -> leftChild;
        }

        currentNode = stack.top();
        stack.pop();

        buffer.push_back(currentNode -> value);

        currentNode = currentNode -> rightChild;
      }
      return buffer;
    }

    Node *root = nullptr;
    Comparator comparator;

  #ifdef DEBUG
    Visual<Node, elementType> visual;
  #endif

  };

#endif
