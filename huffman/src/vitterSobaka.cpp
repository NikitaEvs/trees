#include <iostream>
#include <queue>
#include <utility>
#include <sstream>
#include <fstream>

#include "../util/visualizeHelper.h"
#define ALPHABET_SIZE 256


const int alphSize = 256;

class Node{
 public:
  //-------------------------
  std::string helpString;
  //------------------------
  Node* leftChild;
  Node* rightChild;
  Node* leftBrother;
  Node* rightBrother;
  Node* parent;
  int weight;
  unsigned char code;
  short codeLength;

  Node(std::string hs){
    leftChild = nullptr;
    rightChild = nullptr;
    leftBrother = nullptr;
    rightBrother = nullptr;
    parent = nullptr;
    weight = 0;
    code = 0;
    codeLength = 0;
    helpString = hs;
  }

  void add0ToCode(){
    code = code << 1;
    codeLength++;
  }

  void add1ToCode(){
    code = (code << 1) + 1;
    codeLength++;
  }

  bool isInnerNode(){
    if(leftChild != nullptr) return true;
    return false;
  }


  unsigned char setNewCode(){
    if(parent == nullptr) return 0;

    code = parent->setNewCode();
    codeLength = parent->codeLength;

    if(this == parent->leftChild){
      add0ToCode();
    } else {
      add1ToCode();
    }

    return code;
  }

  void printCode(){
    unsigned char mask = 1;
    for(int i = 0; i < codeLength; i++){
      mask = mask << 1;
    }
    for(int i = 0; i < codeLength; i++){
      std::cout << (int)(mask && code != 0);
      mask = mask >> 1;
    }
  }

  std::string setNewHelpString(){
    if(leftChild != nullptr && rightChild != nullptr){
      helpString = leftChild->setNewHelpString() + rightChild->setNewHelpString();
    }
    return helpString;
  }


  ~Node(){
    if(this != nullptr){
      delete leftChild;
      delete rightChild;
    }
  }
};

class AHTree{
 private:
  Node* zeroNode;
  Node* root;
  Node* table[alphSize];

  void changeZeroNode(Node* newNullNode){
    for(int i = 0; i < alphSize; i++){
      if(table[i] == zeroNode)  table[i] = newNullNode;
    }
  }


  void setTrueBrothers(Node* a){
    std::queue< std::pair<Node*, int> > q;
    q.push(std::make_pair(a, 0));
    while (!q.empty()){
      Node* temp = q.front().first;
      int tempDepth = q.front().second;
      q.pop();
      if(!q.empty() && q.front().second == tempDepth){
        temp->rightBrother = q.front().first;
        q.front().first->leftBrother = temp;
      }
      if(temp->isInnerNode()){
        temp->leftChild->rightBrother = nullptr;
        temp->leftChild->leftBrother = nullptr;
        temp->rightChild->rightBrother = nullptr;
        temp->rightChild->leftBrother = nullptr;
        q.push(std::make_pair(temp->leftChild, tempDepth+1));
        q.push(std::make_pair(temp->rightChild, tempDepth+1));
      }
    }
  }

  void swapNodes(Node* a, Node* b){
    if(a == nullptr || b == nullptr)
      return;
    Node* aPrevP = a->parent;

    a->parent = b->parent;
    if(b->parent->leftChild == b){
      b->parent->leftChild = a;
    } else {
      b->parent->rightChild = a;
    }

    b->parent = aPrevP;
    if(aPrevP->leftChild == a){
      aPrevP->leftChild = b;
    } else {
      aPrevP->rightChild = b;
    }

    setTrueBrothers(root);
  }

  Node* findPrevLeafBlock(Node* a){
    int weight = a->weight;
    Node* blockLeader = a->rightBrother;
    if(blockLeader != nullptr && !blockLeader->isInnerNode() && blockLeader->weight == weight){
      while(blockLeader->rightBrother != nullptr && !blockLeader->rightBrother->isInnerNode() && blockLeader->rightBrother->weight == weight){
        blockLeader = blockLeader->rightBrother;
      }
      if(blockLeader->rightBrother == nullptr){
        blockLeader = blockLeader->parent;
        while (blockLeader != nullptr && (blockLeader->isInnerNode() || blockLeader->weight != weight)){
          blockLeader = blockLeader->leftBrother;
        }
        return blockLeader;
      }else{
        return blockLeader;
      }

    } else if(blockLeader != nullptr){
      return nullptr;
    } else {
      blockLeader = a->parent;
      while (blockLeader != nullptr && (blockLeader->isInnerNode() || blockLeader->weight != weight)){
        blockLeader = blockLeader->leftBrother;
      }
      if(blockLeader != a->parent)
        return blockLeader;
      else
        return nullptr;
    }

  }

  Node* findPrevInnerBlock(Node* a){
    int weight = a->weight;
    Node* blockLeader = a->rightBrother;
    if(blockLeader != nullptr && blockLeader->isInnerNode() && blockLeader->weight == weight){
      while(blockLeader->rightBrother != nullptr && blockLeader->rightBrother->isInnerNode() && blockLeader->rightBrother->weight == weight){
        blockLeader = blockLeader->rightBrother;
      }
      if(blockLeader->rightBrother == nullptr){
        blockLeader = blockLeader->parent;
        while (blockLeader != nullptr && (!blockLeader->isInnerNode() || blockLeader->weight != weight)){
          blockLeader = blockLeader->leftBrother;
        }
        return blockLeader;
      }else{
        return blockLeader;
      }

    } else if(blockLeader != nullptr){
      return nullptr;
    } else {
      blockLeader = a->parent;
      while (blockLeader != nullptr && (!blockLeader->isInnerNode() || blockLeader->weight != weight)){
        blockLeader = blockLeader->leftBrother;
      }
      if(blockLeader != a->parent)
        return blockLeader;
      else
        return nullptr;
    }
  }

  void slideAndIncr(Node* a){
    while (a->parent != nullptr){
      if( a->isInnerNode() ){
        a->weight++;
        Node* prevParent = a->parent;
        swapNodes(a, findPrevLeafBlock(a));
        a = prevParent;

      } else {
        swapNodes(a, findPrevInnerBlock(a));

        a->weight++;
        a = a->parent;
      }
    }
  }



 public:
  AHTree(){
    zeroNode = new Node("_");
    root = zeroNode;
    for(int i = 0; i < alphSize; i++){
      table[i] = zeroNode;
    }

    std::string commandDeleteAllDot = "rm -rf ../huffman/debug/visual/dot/*";
    std::string commandDeleteAllPng = "rm -rf ../huffman/debug/visual/png/*";
    system(commandDeleteAllDot.c_str());
    system(commandDeleteAllPng.c_str());
  }

  void encode(std::string input){
    for(int i = 0; i < input.size(); i++){

      root->setNewHelpString();

      // setTrueBrothers(root);

      unsigned char c = input[i];
      table[c]->setNewCode();
      table[c]->printCode();
      if( zeroNode == table[c]){
        std::cout << c;
        Node* newZeroNode = new Node("_");

        std::string str = " "; str[0] = c;
        Node* cNode = new Node(str);

        table[c] = cNode;
        cNode->parent = zeroNode;
        newZeroNode->parent = zeroNode;
        zeroNode->leftChild = newZeroNode;
        zeroNode->rightChild = cNode;

        //setTrueBrothers(zeroNode);

        changeZeroNode(newZeroNode);
        zeroNode = newZeroNode;
      }

      setTrueBrothers(root);
      slideAndIncr(table[c]);
      visualize();
    }
  }

  void visualize() {

    std::stringstream fileName;
    fileName << "../huffman/debug/visual/dot/dump" << dumpIndex << ".dot";
    std::ofstream dotFile(fileName.str(), std::ios_base::app);

    visual::makeHat(dotFile);

    int kek = 0;
    visualizeNode(dotFile, root, kek);




    dotFile << "}";
    dotFile.close();

    std::stringstream command;
    command << "dot -Tpng " << "../huffman/debug/visual/dot/dump" << dumpIndex << ".dot" << " -o " << "../huffman/debug/visual/png/dump" << dumpIndex << ".png";
    system(command.str().c_str());
    dumpIndex++;
  }

  void visualizeNode(std::ofstream &out, Node *currentNode, int &index) {
    int32_t oldIndex = index;
    visual::generateNodeCode(out, index, currentNode -> weight, currentNode -> helpString);
    if (currentNode -> leftChild != nullptr) {
      visual::generateConnection(out, oldIndex, ++index);
      visualizeNode(out, currentNode -> leftChild, index);
    }
    if (currentNode -> rightChild != nullptr) {
      visual::generateConnection(out, oldIndex, ++index);
      visualizeNode(out, currentNode -> rightChild, index);
    }
  }

  int32_t dumpIndex = 0;
};

int main() {
  std::string s = "ABCCDDDDBB";
  AHTree tree;
  tree.encode(s);
  return 0;
}

