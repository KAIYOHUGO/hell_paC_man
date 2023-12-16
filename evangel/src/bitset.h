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

  void (*free)(mov(BitSet *) b);

  BitSet (*clone)(const brw(BitSet *) b);

  bool (*insert)(brw(BitSet *) b, usize value);

  bool (*contain)(const brw(BitSet *) b, usize value);

  bool (*is_subset)(const brw(BitSet *) superset, const brw(BitSet *) subset);

  void (*intersect_with)(brw(BitSet *) self, brw(BitSet *) other);

  BitSet (*intersection)(const brw(BitSet *) a, const brw(BitSet *) b);

  void (*difference_with)(brw(BitSet *) self, brw(BitSet *) other);

  BitSet (*difference)(const brw(BitSet *) a, const brw(BitSet *) b);

  brw(BitSetIter) (*iter)(const brw(BitSet *) b);

  usize (*iter_next)(brw(BitSetIter *) iter);
};

extern const struct CBitSet CBitSet;

#endif // __BITSET_H
