#ifndef TESTCODE_GENERATOR_H_
#define TESTCODE_GENERATOR_H_

#include <random>
#include <algorithm> //shuffle

namespace generator {
 std::vector<std::string> getRandomStrings(int32_t count, uint8_t firstLetter, uint8_t lastLetter, int32_t maxSize) {
   std::set<std::string> result;
   std::random_device randomDevice;
   std::mt19937 mt(randomDevice());
   std::uniform_int_distribution<uint8_t> letterDistribution(firstLetter, lastLetter);
   std::uniform_int_distribution<int32_t> sizeDistribution(1, maxSize);

   while (result.size() < count) {
     int32_t currentSize = sizeDistribution(mt);
     std::string currentString = "";

     for (int32_t letterIndex = 0; letterIndex < currentSize; letterIndex++) {
       currentString += letterDistribution(mt);
     }

     result.insert(currentString);
   }
   std::vector<std::string> resultVec(result.begin(), result.end());
   return resultVec;
 }

 std::vector<int32_t> getRandomInt(int32_t count) {
   std::vector<int32_t> result;
   std::random_device randomDevice;

   for (int32_t number = -count; number < count; number++) {
     result.push_back(number);
   }

   std::shuffle(result.begin(), result.end(), randomDevice);
   return result;
 }
}


#endif //TESTCODE_GENERATOR_H_
