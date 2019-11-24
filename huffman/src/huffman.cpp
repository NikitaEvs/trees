#include <cstdint> // _t
#include <cassert> // assert
#include <fstream> // ofstream

// TODO write own reverse
#include <algorithm> // reverse
#include <iostream>

#define DEBUG

#ifdef DEBUG
#include <sstream> // stringstream
#include "../util/log.h"
#include "../util/visualizeHelper.h"
#define LOG(X) X
#define VISUAL(X) X
#else
#define LOG(X)
#define VISUAL(X)
#endif

#define ALPHABET_SIZE 256


class Compressor {
  // TODO fix int_32t
 public:
  Compressor() {
    escapeNodeIndex = ALPHABET_SIZE * 2;
    Node *escapeNode = new Node;
    escapeNode -> nodeChar = '~';
    numberMap[escapeNodeIndex] = escapeNode;

    #ifdef DEBUG
    std::string commandDeleteAllDot = "rm -rf ../huffman/debug/visual/dot/*";
    std::string commandDeleteAllPng = "rm -rf ../huffman/debug/visual/png/*";
    system(commandDeleteAllDot.c_str());
    system(commandDeleteAllPng.c_str());
    #endif
  }

  ~Compressor() {

  }

  // TODO Change std::string to char*
  std::string encode(uint8_t originalByte) {
    LOG(log("encode()", "encode symbol", originalByte);)

    std::string encodedByte = "";

    /* Find node with our symbol */
    int32_t currentNodeIndex = symbolMap[originalByte];

    LOG(log("encode()", currentNodeIndex, "index of our symbol");)
    assert(currentNodeIndex >= 0);

    /* If this node is escape node (we add new symbol), result of encoding is code of this symbol */
    if (currentNodeIndex == 0) {
      LOG(log("encode()", "don't find symbol");)
      encodedByte = originalByte;
      LOG(log("encode()", " - > addSymbol()");)
      addSymbol(originalByte);
      LOG(log("encode()", " - > getCompressedByte()");)
      encodedByte += getCompressedByte(escapeNodeIndex);
    } else {
      LOG(log("encode()", " - > getCompressedByte()");)
      encodedByte = getCompressedByte(currentNodeIndex);
      LOG(log("encode()", " - > updateTree()");)
      updateTree(currentNodeIndex);
    }

    LOG(log("addSymbol()", " - > visualize()");)
    VISUAL(visualize();)

    LOG(log("encode()", "result of encoding", encodedByte);)
    assert(encodedByte.length() > 0);
    return encodedByte;
  }

  uint8_t decode(uint8_t compressedByte) {

  }


 private:
  LOG(logger::log log;)

  struct Node {
    int32_t parentIndex = 0,
            rightChildIndex = 0,
            weight = 0;
    uint8_t nodeChar = 0;
  };

  // TODO change string to char*, optimize algorithm
  std::string getCompressedByte(int32_t symbolNodeIndex) {
    std::string result;
    int32_t parentNodeIndex = numberMap[symbolNodeIndex] -> parentIndex;

    while(parentNodeIndex != ALPHABET_SIZE * 2) {
      LOG(log("getCompressedByte()", parentNodeIndex, "parent index");)
      LOG(log("getCompressedByte()", symbolNodeIndex, "current node index");)
      assert(parentNodeIndex != 0);
      if (numberMap[parentNodeIndex] -> rightChildIndex == symbolNodeIndex) {
        result += '1';
        LOG(log("getCompressedByte()", "come from right child");)
      } else {
        result += '0';
        LOG(log("getCompressedByte()", "come from left child");)
      }
      symbolNodeIndex = parentNodeIndex;
      parentNodeIndex = numberMap[parentNodeIndex] -> parentIndex;
    }

    LOG(log("getCompressedByte()", parentNodeIndex, "parent index");)
    LOG(log("getCompressedByte()", symbolNodeIndex, "current node index");)
    assert(parentNodeIndex != 0);
    if (numberMap[parentNodeIndex] -> rightChildIndex == symbolNodeIndex) {
      result += '1';
      LOG(log("getCompressedByte()", "come from right child");)
    } else {
      result += '0';
      LOG(log("getCompressedByte()", "come from left child");)
    }

    std::reverse(result.begin(), result.end());
    return result;
  }

  /* Add new symbol to tree */
  void addSymbol(uint8_t newSymbol) {
    LOG(log("addSymbol()", "add new symbol to the tree", newSymbol);)

    /* Spawn two addition node (one for new escape node and one for symbol) */
    Node  *newSymbolNode = new Node,
          *newEscapeNode = new Node;
    newSymbolNode -> parentIndex = escapeNodeIndex;
    newSymbolNode -> nodeChar = newSymbol;
    newEscapeNode -> parentIndex = escapeNodeIndex;

    numberMap[escapeNodeIndex] -> rightChildIndex = escapeNodeIndex - 1;

    assert((escapeNodeIndex - 1 < ALPHABET_SIZE * 2 + 1) && (escapeNodeIndex - 1 >= 0));
    assert((escapeNodeIndex - 2 < ALPHABET_SIZE * 2 + 1) && (escapeNodeIndex - 2 >= 0));

    symbolMap[newSymbol] = escapeNodeIndex - 1;
    numberMap[escapeNodeIndex - 1] = newSymbolNode;
    numberMap[escapeNodeIndex - 2] = newEscapeNode;

    escapeNodeIndex -= 2;

    LOG(log("addSymbol()", " - > updateTree()");)
    updateTree(escapeNodeIndex + 1);
  }

  /* Save Vitter's rules in tree  */
  void updateTree(int32_t symbolNodeIndex) {
    LOG(log("updateTree()", " - > incrementWeights()");)
    incrementWeights(symbolNodeIndex);


  }

  /* Increment weights */
  void incrementWeights(int32_t symbolNodeIndex) {
    while (symbolNodeIndex != ALPHABET_SIZE * 2) {
      LOG(log("incrementWeights()", symbolNodeIndex, "current node index");)
      numberMap[symbolNodeIndex] -> weight++;
      LOG(log("incrementWeights()", numberMap[symbolNodeIndex] -> weight, "new weight after increment");)
      symbolNodeIndex = numberMap[symbolNodeIndex] -> parentIndex;
      LOG(log("incrementWeights()", symbolNodeIndex, "parent index");)
    }

    numberMap[symbolNodeIndex] -> weight++;
  }

#ifdef DEBUG
  void visualize() {
    LOG(log("visualize()", "create dump");)

    std::stringstream fileName;
    fileName << "../huffman/debug/visual/dot/dump" << dumpIndex << ".dot";
    std::ofstream dotFile(fileName.str(), std::ios_base::app);

    visual::makeHat(dotFile);

    for (int32_t nodeIndex = 0; nodeIndex < ALPHABET_SIZE * 2 + 1; nodeIndex++) {
      if (numberMap[nodeIndex] != nullptr) {
        visual::generateNodeCode(dotFile, nodeIndex, numberMap[nodeIndex] -> weight, numberMap[nodeIndex] -> nodeChar);
      }
    }

    makeConnection(dotFile, ALPHABET_SIZE * 2);
    dotFile << "}";
    dotFile.close();

    std::stringstream command;
    command << "dot -Tpng " << "../huffman/debug/visual/dot/dump" << dumpIndex << ".dot" << " -o " << "../huffman/debug/visual/png/dump" << dumpIndex << ".png";
    system(command.str().c_str());
    dumpIndex++;
  }

  void makeConnection(std::ofstream &out, int32_t currentNumber) {
    if (numberMap[currentNumber] != nullptr) {
      int32_t leftChildIndex = numberMap[currentNumber] -> rightChildIndex - 1;
      int32_t rightChildIndex = numberMap[currentNumber] -> rightChildIndex;

      if (rightChildIndex != 0) {
        visual::generateConnection(out, currentNumber, leftChildIndex);
        visual::generateConnection(out, currentNumber, rightChildIndex);
      }

      makeConnection(out, leftChildIndex);
      makeConnection(out, rightChildIndex);
    }
  }
#endif

  int32_t escapeNodeIndex,
          symbolMap[ALPHABET_SIZE];
  Node   *numberMap[ALPHABET_SIZE * 2 + 1] = {nullptr};

#ifdef DEBUG
  int32_t dumpIndex = 0;
#endif
};


int main() {
  Compressor compressor;
  std::string originalString = "ABCCDDDDBB";

  for (auto originalChar : originalString) {
    std::cout << compressor.encode(originalChar);
  }

  return 0;
}