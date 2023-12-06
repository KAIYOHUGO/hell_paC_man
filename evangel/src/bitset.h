#if !defined(__BITSET_H)
#define __BITSET_H

#include "basic.h"
#include "vec.h"
#include <limits.h>

typedef u64 BITSET_ITEM;
#define BITSET_ITEM_FAST_DIV 6
#define BITSET_ITEM_BITS ((BITSET_ITEM)1 << (BITSET_ITEM)BITSET_ITEM_FAST_DIV)
#define BITSET_ITEM_FAST_MOD (BITSET_ITEM_BITS - (BITSET_ITEM)1)
#define BITSET_ITER_END UINT_MAX

typedef struct {
  Vec(BITSET_ITEM) raw;
} BitSet;

typedef struct {
  BitSet *ptr;
  usize offset;
} BitSetIter;

struct CBitSet {
  BitSet (*init)();

  void (*free)(BitSet *b);

  BitSet (*clone)(brw(BitSet *) b);

  bool (*insert)(BitSet *b, usize value);

  bool (*contain)(BitSet *b, usize value);

  bool (*is_subset)(BitSet *superset, BitSet *subset);

  void (*intersect_with)(brw(BitSet *) self, brw(BitSet *) other);

  BitSet (*intersection)(BitSet *a, BitSet *b);

  void (*difference_with)(brw(BitSet *) self, brw(BitSet *) other);

  BitSet (*difference)(brw(BitSet *) a, brw(BitSet *) b);

  BitSetIter (*iter)(BitSet *b);

  usize (*iter_next)(BitSetIter *iter);

  void (*iter_free)(mov(BitSetIter *) iter);
};

extern const struct CBitSet CBitSet;

#endif // __BITSET_H
