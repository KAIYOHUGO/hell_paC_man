#if !defined(__BITSET_C)
#define __BITSET_C

#include "bitset.h"

static BitSet raw_init() {
  BitSet bit_set = {
      .raw = vec_init(BITSET_ITEM),
  };
  return bit_set;
}

static void raw_free(BitSet *b) { vec_free(BITSET_ITEM, &b->raw); }

static void internal_resize(BitSet *b, usize vec_len) {
  usize old_len = b->raw.len;
  vec_resize(BITSET_ITEM, &b->raw, vec_len);
  // set new item to zero
  for (usize i = old_len; i < vec_len; i++) {
    *vec_index(BITSET_ITEM, &b->raw, i) = 0;
  }
}

static void internal_shrink_len(Vec(BITSET_ITEM) * b) {
  while (b->len > 0) {
    BITSET_ITEM last = *vec_index(BITSET_ITEM, b, b->len - 1);
    if (last != 0)
      break;
    vec_pop(BITSET_ITEM, b);
  }
}

static bool raw_insert(BitSet *b, usize value) {
  usize index = value >> BITSET_ITEM_FAST_DIV;
  usize bit = 1 << (value & BITSET_ITEM_FAST_MOD);
  if (index >= b->raw.len) {
    internal_resize(b, index + 1);
  }
  BITSET_ITEM *ptr = vec_index(BITSET_ITEM, &b->raw, index);
  bool ret = *ptr & bit;
  *ptr |= bit;
  return ret;
}

static bool raw_contain(BitSet *b, usize value) {
  usize index = value >> BITSET_ITEM_FAST_DIV;
  usize bit = 1 << (value & BITSET_ITEM_FAST_MOD);
  if (index >= b->raw.len) {
    return false;
  }
  BITSET_ITEM *ptr = vec_index(BITSET_ITEM, &b->raw, index);
  return *ptr & bit;
}

static bool raw_is_subset(BitSet *superset, BitSet *subset) {
  if (superset->raw.len < subset->raw.len) {
    return false;
  }
  for (usize i = 0; i < subset->raw.len; i++) {
    BITSET_ITEM superset_item = *vec_index(BITSET_ITEM, &superset->raw, i);
    BITSET_ITEM subset_item = *vec_index(BITSET_ITEM, &subset->raw, i);
    if ((superset_item & subset_item) != subset_item) {
      return false;
    }
  }
  return true;
}

static BitSet raw_intersection(BitSet *a, BitSet *b) {
  usize len = min(a->raw.len, b->raw.len);
  Vec(BITSET_ITEM) ret = vec_init(BITSET_ITEM);
  vec_resize(BITSET_ITEM, &ret, len);
  for (usize i = 0; i < len; i++) {
    BITSET_ITEM a_item = *vec_index(BITSET_ITEM, &a->raw, i);
    BITSET_ITEM b_item = *vec_index(BITSET_ITEM, &b->raw, i);
    *vec_index(BITSET_ITEM, &ret, i) = a_item & b_item;
  }
  internal_shrink_len(&ret);
  BitSet bit_set = {.raw = ret};
  return bit_set;
}

static BitSetIter raw_iter(BitSet *b) {
  BitSetIter iter = {.ptr = b, .offset = 0};
  return iter;
}

static usize raw_iter_next(BitSetIter *iter) {
  usize offset_index = iter->offset >> BITSET_ITEM_FAST_DIV;
  usize offset_bit_n = iter->offset & BITSET_ITEM_FAST_MOD;
  for (usize i = offset_index; i < iter->ptr->raw.len; i++) {
    BITSET_ITEM bits = *vec_index(BITSET_ITEM, &iter->ptr->raw, i);
    if (bits == 0) {
      continue;
    }
    for (usize j = offset_bit_n + 1; j < 64; j++) {
      BITSET_ITEM bit_mask = 1 << j;

      if ((bits & bit_mask) != 0) {
        return i * BITSET_ITEM_BITS + j;
      }
    }
    offset_bit_n = 0;
  }

  return BITSET_ITER_END;
}

const struct CBitSet CBitSet = {
    .init = raw_init,
    .free = raw_free,
    .insert = raw_insert,
    .contain = raw_contain,
    .is_subset = raw_is_subset,
    .intersection = raw_intersection,
    .iter = raw_iter,
    .iter_next = raw_iter_next,
};

#endif // __BITSET_C
