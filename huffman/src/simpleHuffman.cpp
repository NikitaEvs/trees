/** Задание №3. Задача 4.
 * Алгоритм сжатия данных Хаффмана.
 * Напишите две функции для создания архива из одного файла и извлечения файла из архива.
 */

#include <string>
#include <iostream>
#include <queue>

//#define DEBUG
#define ALPHABET_SIZE 256

#ifdef DEBUG
#include <fstream>
#else
#include "Huffman.h"
#endif

class SimpleHuffman {
 public:
  SimpleHuffman() = default;

  ~SimpleHuffman() = default;

  std::string encode(std::string &originalString) {
    if (originalString.empty()) return "";

    int frequencyChar[ALPHABET_SIZE] = {0};
    Node *charMap[ALPHABET_SIZE] = {nullptr};
    std::vector<Node*> usingChars;
    std::vector<Node*> additionalNodes;
    std::priority_queue<Node*, std::vector<Node*>, Compare<Node*> > heap;
    std::string result;

    for (unsigned char currentChar : originalString) {
      frequencyChar[currentChar]++;
    }

    for (int codeIndex = 0; codeIndex < ALPHABET_SIZE; codeIndex++) {
      if (frequencyChar[codeIndex] != 0) {
        Node* newNode = new Node(frequencyChar[codeIndex], codeIndex);
        heap.push(newNode);
        charMap[codeIndex] = newNode;
      }
    }

    buildHelpTree(heap, additionalNodes);
    calculateHelpCode(root, 0, usingChars);
    buildCompressedTree(usingChars, additionalNodes);
    calculateCompressedCode(compressedTreeRoot, "");

    Buffer buffer;
    buffer.addBits(usingChars.size());
    result += buffer.getBuffer();
    buffer.clear();

    /* Make alphabet */
    for (auto &usingCharNode : usingChars) {
      result += usingCharNode -> originalCode;
      result += (unsigned char)(usingCharNode -> codeLength);
    }

    for (unsigned char currentChar : originalString) {
      buffer.addBits(charMap[currentChar] -> compressedCode);
    }

    result += buffer.getBufferWithFlush();

    root = nullptr;
    compressedTreeRoot = nullptr;

    /* Delete char's nodes */
    for (auto &currentNode : charMap) {
      delete currentNode;
    }

    /* Delete additional nodes */
    for (auto &currentNode : additionalNodes) {
      delete currentNode;
    }

    return result;
  }

  std::string decode(std::string &compressedString) {
    if (compressedString.empty()) return "";
    Reader reader(compressedString);

    std::vector<Node*> usingChars = reader.getUsingChars();
    std::vector<Node*> aditionalNodes;
    std::string compressedPart = reader.getCompressed();

    buildCompressedTree(usingChars, aditionalNodes);

    std::string result = decompress(compressedPart);

    for (auto &currentNode : usingChars) {
      delete currentNode;
    }

    for (auto &currentNode : aditionalNodes) {
      delete currentNode;
    }

    return result;
  }

 private:

  struct Node {
    int codeLength = 0;
    int frequency = 0;
    unsigned char originalCode = 0;
    std::string compressedCode = "";

    Node *leftChild = nullptr;
    Node *rightChild = nullptr;

    Node() = default;

    ~Node() = default;

    Node(int initFrequency, unsigned char initCode) {
      frequency = initFrequency;
      originalCode = initCode;
    }

    explicit Node(int initFrequency) {
      frequency = initFrequency;
    }

    friend bool operator<(Node &left, Node &right) {
      return left.frequency < right.frequency;
    }
  };

  template <typename element>
  class Compare {
   public:
    bool operator()(element &left, element &right) {
      return *right < *left;
    }
  };

  template <typename element>
  class CompareCodeLength {
   public:
    bool operator()(element &left, element &right) {
      return left -> codeLength < right -> codeLength;
    }
  };

  class Buffer {
   public:
    void addBits(std::string &bits) {
      for (unsigned char currentBit : bits) {
        currentByte <<= 1;

        if (currentBit == '1')
          currentByte |= 1;

        ptr++;
        if (ptr == 8)
          addToBuffer();
      }
    }

    void addBits(int item) {
      for (int byte = 1; byte <= 4; byte++) {
        currentByte = (unsigned char)(item >> 8*(4 - byte));
        addToBuffer();
      }
    }

    std::string getBufferWithFlush() {
      flush();
      return buffer;
    }

    std::string getBuffer() {
      return buffer;
    }

    void clear() {
      currentByte = 0;
      buffer = "";
    }

   private:
    void addToBuffer() {
      buffer += currentByte;
      currentByte = 0;
      ptr = 0;
    }

    void flush() {
      currentByte <<= (8 - ptr);
      buffer += currentByte;
      buffer += (unsigned char)(8 - ptr);
    }

    unsigned char currentByte = 0;
    int ptr = 0;
    std::string buffer = "";
  };

  class Reader {
   public:
    explicit Reader(std::string &inputString) {
      charsCount = readInt(0, inputString);

      for (int byteIndex = 4; byteIndex < charsCount * 2 + 4; byteIndex += 2) {
        Node *newNode = new Node;
        newNode -> originalCode = inputString[byteIndex];
        newNode -> codeLength = inputString[byteIndex + 1];
        usingChars.push_back(newNode);
      }

      compressed = inputString.substr(charsCount * 2 + 4, inputString.length());
    }

    std::vector<Node*> getUsingChars() {
      return usingChars;
    }

    std::string getCompressed() {
      return compressed;
    }

    int readInt(int pos, std::string &inputString) {
      int result = 0;

      for (int byte = 0; byte < 4; byte++) {
        unsigned char currentByte = inputString[pos + byte];
        result <<= 8;
        result |= currentByte;
      }

      return result;
    }

   private:
    std::vector<Node*> usingChars;
    std::string compressed = "";
    int charsCount = 0;
  };

  class Decoder {
   public:
    Decoder(Node *compressedTreeRoot) {
      currentRoot = compressedTreeRoot;
      currentNode = compressedTreeRoot;
    }

    void decompressByte(unsigned char byte, int shift = 8) {
      for (int bitIndex = 0; bitIndex < shift; bitIndex++) {
        if (byte & (1u << 7))   currentNode = currentNode -> rightChild;
        else                    currentNode = currentNode -> leftChild;

        if ((currentNode -> rightChild == nullptr) && (currentNode -> leftChild == nullptr)) {
          outputBuffer += currentNode -> originalCode;
          currentNode = currentRoot;
        }

        byte <<= 1;
      }
    }

    std::string getOutputBuffer() {
      return outputBuffer;
    }

   private:
    Node *currentRoot = nullptr;
    Node *currentNode = nullptr;
    std::string outputBuffer = "";
  };

  void buildHelpTree(std::priority_queue<Node*, std::vector<Node*>, Compare<Node*> > &heap, std::vector<Node*> &additionalNodes) {
    bool aloneSymbol = heap.size() == 1;

    while (heap.size() > 1) {
      Node *firstNode = heap.top();
      heap.pop();
      Node *secondNode = heap.top();
      heap.pop();

      Node *newNode = new Node(firstNode -> frequency + secondNode -> frequency);
      newNode -> leftChild = firstNode;
      newNode -> rightChild = secondNode;

      additionalNodes.push_back(newNode);
      heap.push(newNode);
    }

    if (aloneSymbol) {
      Node *newRoot = new Node;
      additionalNodes.push_back(newRoot);

      Node *currentNode = heap.top();
      newRoot -> rightChild = currentNode;
      newRoot -> frequency = currentNode -> frequency;

      root = newRoot;
      heap.pop();
    } else {
      root = heap.top();
      heap.pop();
    }
  }

  void buildCompressedTree(std::vector<Node*> alphabet, std::vector<Node*> &usedNodes) {
    std::priority_queue<Node*, std::vector<Node*>, CompareCodeLength<Node*> > heapForCompressingTree(alphabet.begin(), alphabet.end());
    bool aloneSymbol = heapForCompressingTree.size() == 1;

    while (heapForCompressingTree.size() != 1) {
      Node *firstNode = heapForCompressingTree.top();
      heapForCompressingTree.pop();
      Node *secondNode = heapForCompressingTree.top();
      heapForCompressingTree.pop();

      Node *newNode = new Node;
      newNode -> codeLength = firstNode -> codeLength - 1;
      newNode -> leftChild = firstNode;
      newNode -> rightChild = secondNode;

      usedNodes.push_back(newNode);
      heapForCompressingTree.push(newNode);
    }

    if (aloneSymbol) {
      Node *newRoot = new Node;
      Node *currentNode = heapForCompressingTree.top();

      newRoot -> rightChild = currentNode;
      newRoot -> codeLength = currentNode -> codeLength - 1;
      compressedTreeRoot = newRoot;

      usedNodes.push_back(newRoot);
      heapForCompressingTree.pop();
    } else {
      compressedTreeRoot = heapForCompressingTree.top();
      heapForCompressingTree.pop();
    }

  }

  void calculateHelpCode(Node *currentNode, int currentLength, std::vector<Node*> &usingChars) {
    if (currentNode == nullptr) return;

    if ((currentNode -> leftChild == nullptr) && (currentNode -> rightChild == nullptr)) {
      currentNode -> codeLength = currentLength;
      usingChars.push_back(currentNode);
    } else {
      if (currentNode -> rightChild != nullptr)
        calculateHelpCode(currentNode -> rightChild, currentLength + 1, usingChars);
      if (currentNode -> leftChild != nullptr)
        calculateHelpCode(currentNode -> leftChild, currentLength + 1, usingChars);
    }
  }

  void calculateCompressedCode(Node *currentNode, std::string compressedCode) {
    if ((currentNode -> leftChild == nullptr) && (currentNode -> rightChild == nullptr)) {
      currentNode -> compressedCode = compressedCode;
    } else {
      if (currentNode -> rightChild != nullptr) {
        calculateCompressedCode(currentNode -> rightChild, compressedCode + "1");
      }
      if (currentNode -> leftChild != nullptr) {
        calculateCompressedCode(currentNode -> leftChild, compressedCode + "0");
      }
    }
  }

  std::string decompress(std::string &compressedString) {
    std::string result;
    Decoder decoder(compressedTreeRoot);

    for (unsigned long long currentByteIndex = 0; currentByteIndex < compressedString.length() - 2; currentByteIndex++) {
      decoder.decompressByte(compressedString[currentByteIndex]);
    }

    decoder.decompressByte(compressedString[compressedString.length() - 2], 8 - compressedString[compressedString.length() - 1]);

    return decoder.getOutputBuffer();
  }


  Node *root = nullptr;
  Node *compressedTreeRoot = nullptr;
};

#ifdef DEBUG

int main() {
  SimpleHuffman simpleHuffman;

  std::string in;
  std::ifstream originalText("../huffman/debug/texts/original/RAY.BMP", std::ios::binary);
  std::ofstream compressedText("../huffman/debug/texts/compressed/RAY.BMP");
  std::ofstream decodedText("../huffman/debug/texts/decoded/RAY.BMP");

  originalText.seekg(0, std::ios_base::end);
  in.resize(originalText.tellg());
  originalText.seekg(0, std::ios_base::beg);
  originalText.read((char*)in.data(), (int)in.size());

  std::string compressed = simpleHuffman.encode(in);
  compressedText << compressed;
  std::string decoded = simpleHuffman.decode(compressed);
  decodedText << decoded;
  std::cout << compressed.length() << " " << in.length();
  return 0;
}

#else

void Encode(IInputStream& original, IOutputStream& compressed) {
  byte inputChar = 0;
  std::string originalText = "", compressedText = "";

  while (original.Read(inputChar)) {
    originalText += inputChar;
  }

  SimpleHuffman simpleHuffman;
  compressedText = simpleHuffman.encode(originalText);

  for (int i = 0; i < compressedText.size(); i++) {
    compressed.Write(compressedText[i]);
  }
}

void Decode(IInputStream& compressed, IOutputStream& original) {
  byte inputChar = 0;
  std::string originalText = "", compressedText = "";

  while (compressed.Read(inputChar)) {
    compressedText += inputChar;
  }

  SimpleHuffman simpleHuffman;
  originalText = simpleHuffman.decode(compressedText);

  for (int i = 0; i < originalText.size(); i++) {
    original.Write(originalText[i]);
  }
}

#endif