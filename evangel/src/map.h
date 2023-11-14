#if !defined(__MAP_H)
#define __MAP_H

#include "basic.h"
#include "vec.h"

struct MapEntry {
  usize key;
  void *value;
};

typedef struct {
  usize len;
  Vec(Vec(struct MapEntry)) buckets;
} Map;

struct CMap {};

const struct CMap CMap;

#endif // __MAP_H
