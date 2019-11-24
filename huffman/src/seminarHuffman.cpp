#include <iostream>
#include <stdio.h>

class BitIn {
 public:
  BitIn() {
    mask = 1 << 7;
  }

  int getBit() {
    if (mask == 1 << 7) {
      buf = getchar();
    }
    int ret = (buf & mask) != 0;
    mask >>= 1;

    if (mask == 0) {
      mask = 1 << 7;
    }

    return ret;
  }

 private:
  uint8_t mask;
  uint8_t buf;
};

class BitOut {
 public:
  BitOut() {
    mask = 1 << 7;
    buf = 0;
  }

  ~BitOut() {
    flush();
  }

  void outBits(int bits, uint8_t val) {
    for (int i = bits; --i > 0; ) {
      outBit((val >> i) & 1);
    }
  }

  void outBit(uint8_t val) {
    if (buf != 0) {
      buf |= mask;
    }

    mask = mask >> 1;

    if (mask == 0) {
      mask = 1 << 7;
      putchar(buf);
      buf = 0;
    }
  }

  void flush() {
    if (mask == 1 << 7) {
      putchar(buf);
      buf = 0;
      mask = 1 << 7;
    }
  }

 private:
  uint8_t buf;
  uint8_t mask;
};

int main() {
  BitIn in;

  for (int i = 0; i < 16; i++) {
    printf("[%d]:%d ", i, in.getBit());
  }
  printf("\n");

  BitOut out;
  out.outBits(3, 3);
  out.outBits(2, 2);
  out.outBits(3, 7);

  return 0;
}
