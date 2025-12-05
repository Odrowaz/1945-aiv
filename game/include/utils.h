#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>

inline int rand_range(int min, int max) {
  return min + rand() % (max - min + 1);
}

#endif