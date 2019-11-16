#ifndef TESTCODE_COMPARATOR_H_
#define TESTCODE_COMPARATOR_H_

template <typename elementType>
class Comparator {
 public:
  bool operator() (const elementType &left, const elementType &right) {
    return left < right;
  }
};

#endif //TESTCODE_COMPARATOR_H_
