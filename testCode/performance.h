#ifndef TESTCODE_PERFOMANCE_H_
#define TESTCODE_PERFOMANCE_H_

#define MIN_COUNT 50000
#define MAX_COUNT 1000000
#define STEP 50000
#define STRING_MAX_SIZE 100

#include <cstdint>
#include "../trees/avl.h" // BinarySearchTree
#include "seminarAVL.h" // AVLTree
#include <set> // set
#include <cassert>
#include <ext/pb_ds/assoc_container.hpp> // order tree (R-B tree)
#include <ext/pb_ds/tree_policy.hpp>

#include <string>
#include <fstream>

#include <chrono>
#include "generator.h"
#include "comparator.h"

namespace tester {
  typedef __gnu_pbds::tree<
      int32_t ,
      __gnu_pbds::null_type,
      less<int32_t>,
      __gnu_pbds::rb_tree_tag,
      __gnu_pbds::tree_order_statistics_node_update>
      IntOrderSet;

  typedef __gnu_pbds::tree<
      std::string,
      __gnu_pbds::null_type,
      less<std::string>,
      __gnu_pbds::rb_tree_tag,
      __gnu_pbds::tree_order_statistics_node_update>
      StringOrderSet;


  void makeInsertStringHat(std::ofstream &os) {
    os << "Количество элементов," << "AVLTree с семинара," << "Set," << "AVL" << std::endl;
  }

  void makeInsertNumberHat(std::ofstream &os) {
    os << "Количество элементов," << "Set," << "AVL" << std::endl;
  }

  void makeDeleteStringHat(std::ofstream &os) {
    os << "Количество элементов," << "AVLTree с семинара," << "Set," << "AVL" << std::endl;
  }

  void makeDeleteNumberHat(std::ofstream &os) {
    os << "Количество элементов," << "Set," << "AVL" << std::endl;
  }

  void makeOrderStatisticHat(std::ofstream &os) {
    os << "Количество элементов (запросов)," << "Set," << "AVL" << std::endl;
  }


  void testAll() {
    Comparator<std::string> comparatorString;
    Comparator<int32_t> comparatorInt;

    std::ofstream resultFile;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

    /* Insert string performance */
    resultFile.open("../testCode/result/insertString.csv");
    makeInsertStringHat(resultFile);

    for (int32_t count = MIN_COUNT; count <= MAX_COUNT; count += STEP) {
      resultFile << count << ',';

      BinarySearchTree<std::string, Comparator<std::string> > stringSearchTree(comparatorString);
      AVLTree stringAVLTree;
      std::set<std::string> stringSet;


      std::vector<std::string> testData = generator::getRandomStrings(count, 'a', 'z', STRING_MAX_SIZE);

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        stringAVLTree.insert(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << ',';

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        stringSet.insert(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << ',';

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        stringSearchTree.balancedInsert(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
    }

    resultFile.close();

    /* Insert number performance */
    resultFile.open("../testCode/result/insertNumber.csv");
    makeInsertNumberHat(resultFile);

    for (int32_t count = MIN_COUNT; count <= MAX_COUNT; count += STEP) {
      resultFile << count << ',';

      BinarySearchTree<int32_t, Comparator<int32_t> > intSearchTree(comparatorInt);
      std::set<int32_t> intSet;


      std::vector<int32_t> testData = generator::getRandomInt(count);

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        intSet.insert(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << ',';

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        intSearchTree.balancedInsert(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
    }

    resultFile.close();

    /* Delete string performance */
    resultFile.open("../testCode/result/deleteString.csv");
    makeDeleteStringHat(resultFile);

    for (int32_t count = MIN_COUNT; count <= MAX_COUNT; count += STEP) {
      resultFile << count << ',';

      BinarySearchTree<std::string, Comparator<std::string> > stringSearchTree(comparatorString);
      AVLTree stringAVLTree;
      std::set<std::string> stringSet;


      std::vector<std::string> testData = generator::getRandomStrings(count, 'a', 'z', STRING_MAX_SIZE);

      for (const auto& item : testData) {
        stringAVLTree.insert(item);
      }

      for (const auto& item : testData) {
        stringSet.insert(item);
      }

      for (const auto& item : testData) {
        stringSearchTree.balancedInsert(item);
      }

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        stringAVLTree.remove(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << ',';

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        stringSet.erase(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << ',';

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        stringSearchTree.balancedErase(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
    }

    resultFile.close();

    /* Delete number performance */
    resultFile.open("../testCode/result/deleteNumber.csv");
    makeDeleteNumberHat(resultFile);

    for (int32_t count = MIN_COUNT; count <= MAX_COUNT; count += STEP) {
      resultFile << count << ',';

      BinarySearchTree<int32_t, Comparator<int32_t> > intSearchTree(comparatorInt);
      std::set<int32_t> intSet;;


      std::vector<int32_t> testData = generator::getRandomInt(count);

      for (const auto& item : testData) {
        intSet.insert(item);
      }

      for (const auto& item : testData) {
        intSearchTree.balancedInsert(item);
      }

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        intSet.erase(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << ',';

      startTime = std::chrono::high_resolution_clock::now();
      for (const auto& item : testData) {
        intSearchTree.balancedErase(item);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
    }

    resultFile.close();

    /* Number order statistic performance */
    resultFile.open("../testCode/result/numberOrderStatistic.csv");
    makeOrderStatisticHat(resultFile);

    for (int32_t count = MIN_COUNT; count <= MAX_COUNT; count += STEP) {
      resultFile << count << ',';

      BinarySearchTree<int32_t, Comparator<int32_t> > intSearchTree(comparatorInt);
      IntOrderSet intOrderSet;


      std::vector<int32_t> testData = generator::getRandomInt(count);

      for (const auto& item : testData) {
        intSearchTree.balancedInsert(item);
      }

      for (const auto& item : testData) {
        intOrderSet.insert(item);
      }

      startTime = std::chrono::high_resolution_clock::now();
      for (uint64_t itemIndex = 0; itemIndex < testData.size(); itemIndex++) {
        intOrderSet.find_by_order(itemIndex);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << ',';

      startTime = std::chrono::high_resolution_clock::now();
      for (uint64_t itemIndex = 0; itemIndex < testData.size(); itemIndex++) {
        intSearchTree.select(itemIndex);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
    }

    resultFile.close();

    /* String order statistic performance */
    resultFile.open("../testCode/result/stringOrderStatistic.csv");
    makeOrderStatisticHat(resultFile);

    for (int32_t count = MIN_COUNT; count <= MAX_COUNT; count += STEP) {
      resultFile << count << ',';

      BinarySearchTree<std::string, Comparator<std::string> > stringSearchTree(comparatorString);
      StringOrderSet stringOrderSet;


      std::vector<std::string> testData = generator::getRandomStrings(count, 'a', 'z', STRING_MAX_SIZE);

      for (const auto& item : testData) {
        stringSearchTree.balancedInsert(item);
      }

      for (const auto& item : testData) {
        stringOrderSet.insert(item);
      }

      startTime = std::chrono::high_resolution_clock::now();
      for (uint64_t itemIndex = 0; itemIndex < testData.size(); itemIndex++) {
        stringOrderSet.find_by_order(itemIndex);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << ',';

      startTime = std::chrono::high_resolution_clock::now();
      for (uint64_t itemIndex = 0; itemIndex < testData.size(); itemIndex++) {
        stringSearchTree.select(itemIndex);
      }
      endTime = std::chrono::high_resolution_clock::now();
      resultFile << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
    }

    resultFile.close();
  }
}

#endif //TESTCODE_PERFOMANCE_H_
