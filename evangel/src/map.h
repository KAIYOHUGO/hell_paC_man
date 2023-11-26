#if !defined(__MAP_H)
#define __MAP_H

#include "basic.h"
#include "vec.h"

typedef struct {
  usize key;
  u8 value[];
} MapEntry;

typedef struct {
  usize len, size;
  Vec(Vec(MapEntry)) buckets;
} Map;

struct CMap {
  Map (*init)(usize size_of_T);
  void (*free)(Map *m);
  void *(*get)(Map *m, usize key);
  void (*reserve)(Map *m, usize additional);
  void *(*insert)(Map *m, usize key);
  void (*remove)(Map *m, usize key);
  void (*clear)(Map *m);
};

extern const struct CMap CMap;

#define Map(K, T) Map

#define map_init(T) CMap.init(sizeof(T))

#define map_free(T, m) CMap.free(m)

#define map_get(T, m, key) ((T *)CMap.get(m, key))

#define map_reserve(T, m, additional) CMap.reserve(m, additional)

// need to be a heap pointer
#define map_insert(T, m, key, value) (*(T *)CMap.insert(m, key) = value)

// free the element first or it will cause memory leak
#define map_remove(T, m, key) CMap.remove(m, key)

#define map_clear(T, m) CMap.clear(m)

#endif // __MAP_H
