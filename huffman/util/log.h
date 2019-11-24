#ifndef HUFFMAN_UTIL_LOG_H_
#define HUFFMAN_UTIL_LOG_H_

#include <fstream>  // ofstream
#include <ctime> // tm, time_t, time
#include <string>
#include <cassert>
#include <sstream> // stringstream


namespace logger {
  class log {
   public:
    log() {
      std::stringstream fileName;
      fileName << logFileName << getTime();
      logFile.open(fileName.str());
      logFile << "***START LOG***" << std::endl;
    }

    ~log() {
      logFile.close();
    }

    void operator()(std::string message) {
      logFile << message << std::endl;
    }

    void operator()(std::string functionName, std::string message) {
      logFile << functionName << " | " << message << std::endl;
    }

    void operator()(std::string functionName, std::string message, uint8_t symbol) {
      logFile << functionName << " | " << message << " | Byte: " << symbol << std::endl;
    }

    void operator()(std::string functionName, std::string message, std::string symbolCode) {
      logFile << functionName << " | " << message << " | Byte code: " << symbolCode << std::endl;
    }

    void operator()(std::string functionName, int32_t index, std::string message) {
      logFile << functionName << " | " << message << " | Number: " << index << std::endl;
    }

    void operator()(std::string functionName, std::string message, bool status) {
      logFile << functionName << " | STATUS: " << status << " | " << message << std::endl;
    }

   private:
    std::ofstream logFile;
    std::string logFileName = "../huffman/debug/log/log";

    std::string getTime() {
      std::string timeFormatted;
      char timeBuffer[120] = {0};

      tm *timeInfo;
      time_t rawTime;
      time(&rawTime);
      timeInfo = localtime(&rawTime);

      if (strftime(timeBuffer, 120, "%d.%m.%Y %H:%M:%S", timeInfo) == 0) {
        timeFormatted = "";
      } else {
        timeFormatted = std::string(timeBuffer);
      }

      assert(timeFormatted.length() > 0);
      return timeFormatted;
    }
  };
}
#endif //HUFFMAN_UTIL_LOG_H_
