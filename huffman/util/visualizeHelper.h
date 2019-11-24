#ifndef HUFFMAN_UTIL_VISUALIZEHELPER_H_
#define HUFFMAN_UTIL_VISUALIZEHELPER_H_

#include <fstream> // oftstream

namespace visual {

  void makeHat(std::ofstream &out) {
      out <<  "digraph D {" << std::endl <<
              "    node [shape=plaintext fontname=\"Sans serif\" fontsize=\"8\"];" << std::endl;
  }

  void generateNodeCode(std::ofstream &out, int32_t number, int32_t weight, uint8_t symbol) {
    if (symbol == 0)
      symbol = '~';
    out <<  "    " << number << " [ label=<" << std::endl <<
            "     <table border=\"1\" cellborder=\"0\" cellspacing=\"1\">"                              << std::endl <<
            "      <tr><td align=\"left\"><b>Number: " << number << "</b></td></tr>"                    << std::endl <<
            "      <tr><td align=\"left\">Weight: " << weight << "</td></tr>"                           << std::endl <<
            "      <tr><td align=\"left\"><font color=\"darkgreen\">" << symbol << "</font></td></tr>"  << std::endl <<
            "     </table>>];"                                                                          << std::endl;
  }

  void generateNodeCode(std::ofstream &out, int32_t number, int32_t weight, std::string symbol) {
    out <<  "    " << number << " [ label=<" << std::endl <<
        "     <table border=\"1\" cellborder=\"0\" cellspacing=\"1\">"                              << std::endl <<
        "      <tr><td align=\"left\"><b>Number: " << number << "</b></td></tr>"                    << std::endl <<
        "      <tr><td align=\"left\">Weight: " << weight << "</td></tr>"                           << std::endl <<
        "      <tr><td align=\"left\"><font color=\"darkgreen\">" << symbol << "</font></td></tr>"  << std::endl <<
        "     </table>>];"                                                                          << std::endl;
  }

  void generateSimpleHelpNodeCode(std::ofstream &out, int32_t number, int32_t frequency, uint8_t originalCode, ) {

  }

  void generateCompresseNodeCode(std::ofstream &out, int32_t number, uint8_t )

  void generateConnection(std::ofstream &out, int32_t firstNumber, int32_t secondNumber) {
    out <<  "    " << firstNumber << "->" << secondNumber << ";" << std::endl;
  }

}

#endif //HUFFMAN_UTIL_VISUALIZEHELPER_H_
