#ifndef VISLIB_VISUALIZATION_H_
#define VISLIB_VISUALIZATION_H_

#include "json/json.hpp" // nlohmann::json
#include <string> // string
#include <fstream> // ifstream, ofstream
#include <cassert> // assert

#define CONFIG_FILE "../visLib/config/config.json"

using json = nlohmann::json;

template <typename Node, typename elementType>
class Visual {
 public:

  Visual() {
    configure();
    if (deleteBefore) {
      deleteLastFiles();
    }
  }

  /* Init with json config file */
  void configure() {
    std::string configStr;
    std::ifstream configFile(CONFIG_FILE, std::ios::binary);

    /* Read file in one string */
    configFile.seekg(0, std::ios_base::end);
    int32_t  a = configFile.tellg();
    configStr.resize(configFile.tellg());
    configFile.seekg(0, std::ios_base::beg);
    configFile.read((char*)configStr.data(), (int32_t)configStr.size());

    /* Parse config params */
    json params = json::parse(configStr);
    std::string c_outputDir = params["param"]["outputDir"];
    outputDir = c_outputDir;
    std::string c_dumpName = params["param"]["dumpName"];
    dumpName = c_dumpName;
    std::string c_digraphName = params["param"]["digraphName"];
    digraphName = c_digraphName;
    std::string c_font = params["param"]["font"];
    font = c_font;
    std::string c_shape = params["param"]["shape"];
    shape = c_shape;
    std::string c_imageDir = params["param"]["imageDir"];
    imageDir = c_imageDir;
    bool c_deleteBefore = params["param"]["deleteBefore"];
    deleteBefore = c_deleteBefore;
    bool c_onlyOneFileMode = params["param"]["onlyOneFileMode"];
    onlyOneFileMode = c_onlyOneFileMode;
    bool c_appendMode = params["param"]["appendMode"];
    appendMode = c_appendMode;
    std::string c_logDir = params["param"]["logDir"];
    logDir = c_logDir;
    bool c_diffMode = params["param"]["diffMode"];
    diffMode = c_diffMode;
    bool c_childCountMode = params["param"]["childCountMode"];
    childCountMode = c_childCountMode;
    bool c_treapMode = params["param"]["treapMode"];
    treapMode = c_treapMode;

    assert(!(appendMode && onlyOneFileMode));
    assert(!(diffMode && childCountMode));
    configFile.close();
  }

  /* Simple print tree */
  void dump(Node *root) {
    if (onlyOneFileMode) {
      deleteLastFiles();
    }
    std::stringstream fileName, imageName;
    fileName  << outputDir  << dumpName << dumpIndex << ".dot";
    imageName << imageDir   << dumpName << dumpIndex << ".png";
    std::ofstream dumpFile(fileName.str());
    makeHat(dumpFile);

    // TODO switch between function
    //dumpTreapNode(dumpFile, root);
    dumpNode(dumpFile, root);

    dumpFile << "}";
    dumpFile.close();
    std::string command = "dot -Tpng " + fileName.str() + " -o " + imageName.str();
    system(command.c_str());
    dumpIndex++;
    nullPointIndex = 0;
  }

  /* Print tree with log */
  void logDump(Node *root, char * message) {
    log(message);
    dump(root);
  }

  // TODO fix append mode
  /* Generate new dot file with new node */
  void appendNode(Node *newNode) {
    if (dumpIndex == 0) {
      dump(newNode);
    } else {
      appendNodeWithParent(newNode, newNode -> parent);
    }
  }

  void appendNodeWithParent(Node *newNode, Node *parent) {
    assert(dumpIndex != 0); // You must use dump() in first visualization
    assert(appendMode); // You can use it only in appendMode

    std::string oldDumpStr;
    std::stringstream oldFileName, newFileName, newImageName;
    oldFileName   << outputDir << dumpName << dumpIndex - 1 << ".dot";
    newFileName   << outputDir << dumpName << dumpIndex     << ".dot";
    newImageName  << imageDir  << dumpName << dumpIndex     << ".png";
    std::ifstream oldDumpFile(oldFileName.str());
    std::ofstream newDumpFile(newFileName.str());

    oldDumpFile.seekg(-1, std::ios_base::end);
    oldDumpStr.resize(oldDumpFile.tellg());
    oldDumpFile.seekg(0, std::ios_base::beg);
    oldDumpFile.read((char *) oldDumpStr.data(), oldDumpStr.size());
    oldDumpFile.close();

    newDumpFile   << oldDumpStr << "\n"
                  << "    " << parent -> value << " -> " << newNode -> value << "\n"
                  << "}";
    newDumpFile.close();
    std::string command = "dot -Tpng " + newFileName.str() + " -o " + newImageName.str();
    system(command.c_str());
    dumpIndex++;
    nullPointIndex = 0;
  }

 private:
  std::string outputDir               ,
              dumpName                ,
              digraphName             ,
              shape                   ,
              imageDir                ,
              logDir                  ,
              font                    ;
  uint32_t    dumpIndex       = 0     ,
              nullPointIndex  = 0     ;
  bool        deleteBefore    = false ,
              appendMode      = false ,
              onlyOneFileMode = false ,
              childCountMode  = false ,
              treapMode       = false ,
              diffMode        = false ;

  /* Make init hat for .dot out file */
  void makeHat(std::ofstream &dumpFile) {
    dumpFile << "digraph " << digraphName << " {" << "\n" <<
                "    node [fontname = \"" << font << "\"];\n";
  }

  /* Write connection of node and children in .dot file */
  void dumpNode(std::ofstream &dumpFile, Node *currentNode) {
    if (currentNode != nullptr) {
      if (currentNode -> leftChild  != nullptr) {
        if (diffMode) {
          dumpFile << "    " << currentNode -> value << " -> " << currentNode -> leftChild  -> value << " [label=\"diff: " << currentNode -> diffHeight << "\"];\n";
        } else if (childCountMode) {
          dumpFile << "    " << currentNode -> value << " -> " << currentNode -> leftChild  -> value  << " [label=\"childCount: " << currentNode -> leftChildCount << "\"];\n";
        } else {
          dumpFile << "    " << currentNode -> value << " -> " << currentNode -> leftChild  -> value << ";\n";
        }
        dumpNode(dumpFile, currentNode -> leftChild);
      } else if (!appendMode) {
        dumpFile << generateNullPointDeclaration();
        dumpFile << "    " << currentNode -> value << " -> " << generateNullPointNode() << ";\n";
      }
      if (currentNode -> rightChild != nullptr) {
        if (diffMode) {
          dumpFile << "    " << currentNode -> value << " -> " << currentNode -> rightChild->value << " [label=\"diff: " << currentNode -> diffHeight << "\"];\n";
        } else if (childCountMode) {
          dumpFile << "    " << currentNode -> value << " -> " << currentNode -> rightChild  -> value   << " [label=\"childCount: " << currentNode -> rightChildCount << "\"];\n";
        } else {
          dumpFile << "    " << currentNode -> value << " -> " << currentNode -> rightChild -> value << ";\n";
        }
        dumpNode(dumpFile, currentNode -> rightChild);
      } else if (!appendMode) {
        dumpFile << generateNullPointDeclaration();
        dumpFile << "    " << currentNode -> value << " -> " << generateNullPointNode() << ";\n";
      }
    }
  }

  /* Write connection of node and children in .dot file for Treap node */
  void dumpTreapNode(std::ofstream &dumpFile, Node *currentNode) {
    if (currentNode != nullptr) {
      if (currentNode->leftChild != nullptr) {
        dumpFile << "    " << currentNode -> key_ << " -> " << currentNode -> leftChild->key_ << " [label=\"priority: " << currentNode -> priority_ << "\"];\n";
        dumpTreapNode(dumpFile, currentNode -> leftChild);
      } else if (!appendMode) {
        dumpFile << generateNullPointDeclaration();
        dumpFile << "    " << currentNode -> key_ << " -> " << generateNullPointNode() << " [label=\"priority: " << currentNode -> priority_ << "\"];\n";
      }
      if (currentNode->rightChild != nullptr) {
        dumpFile << "    " << currentNode -> key_ << " -> " << currentNode -> rightChild -> key_ << " [label=\"priority: " << currentNode -> priority_ << "\"];\n";
        dumpTreapNode(dumpFile, currentNode -> rightChild);
      } else if (!appendMode) {
        dumpFile << generateNullPointDeclaration();
        dumpFile << "    " << currentNode -> key_ << " -> " << generateNullPointNode() << " [label=\"priority: " << currentNode -> priority_ << "\"];\n";
      }
    }
  }

  std::string generateNullPointDeclaration() {
    std::stringstream nullPointerDeclaration;
    nullPointerDeclaration << "    null" << nullPointIndex << "[shape=" << shape << "];\n";
    return nullPointerDeclaration.str();
  };

  std::string generateNullPointNode() {
    std::stringstream nullPointerNode;
    nullPointerNode << "null" << nullPointIndex;
    nullPointIndex++;
    return nullPointerNode.str();
  }

  /* Delete all files from out/dot and out/png */
  void deleteLastFiles() {
    std::string commandDeleteAllDot = "rm -rf " + outputDir + "*";
    std::string commandDeleteAllPng = "rm -rf " + imageDir + "*";
    system(commandDeleteAllDot.c_str());
    system(commandDeleteAllPng.c_str());
  }

  void log(const char * message) {
    std::string logFileName = logDir + "log";
    std::ofstream logFile(logFileName, std::ios_base::app);
    logFile << "dump # " << dumpIndex << " :" << message << "\n";
    logFile.close();
  }
};

#endif //VISLIB_VISUALIZATION_H_
