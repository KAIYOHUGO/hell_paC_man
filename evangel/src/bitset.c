#include "bitset.h"

static BitSet raw_init() {
  BitSet bit_set = {
      .raw = vec_init(BITSET_ITEM),
  };
  return bit_set;
}

static void raw_free(mov(BitSet *) b) { vec_free(BITSET_ITEM, &b->raw); }

static BitSet raw_clone(const brw(BitSet *) b) {
  Vec(BITSET_ITEM) new_raw = vec_clone(BITSET_ITEM, &b->raw);
  BitSet bit_set = {.raw = new_raw};
  return bit_set;
}

static void internal_resize(brw(BitSet *) b, usize vec_len) {
  usize old_len = b->raw.len;
  vec_resize(BITSET_ITEM, &b->raw, vec_len);
  // set new item to zero
  for (usize i = old_len; i < vec_len; i++) {
    *vec_index(BITSET_ITEM, &b->raw, i) = 0;
  }
}

static void internal_shrink_len(brw(Vec(BITSET_ITEM) *) b) {
  while (b->len > 0) {
    BITSET_ITEM last = *vec_index(BITSET_ITEM, b, b->len - 1);
    if (last != 0)
      break;
    // unused return value
    (void)vec_pop(BITSET_ITEM, b);
  }
}

static bool raw_insert(brw(BitSet *) b, usize value) {
  usize index = value >> BITSET_ITEM_FAST_DIV;
  BITSET_ITEM bit = (BITSET_ITEM)1 << (value & BITSET_ITEM_FAST_MOD);
  if (index >= b->raw.len) {
    internal_resize(b, index + 1);
  }
  BITSET_ITEM *ptr = vec_index(BITSET_ITEM, &b->raw, index);
  bool ret = *ptr & bit;
  *ptr |= bit;
  return ret;
}

static bool raw_contain(const brw(BitSet *) b, usize value) {
  usize index = value >> BITSET_ITEM_FAST_DIV;
  BITSET_ITEM bit = (BITSET_ITEM)1 << (value & BITSET_ITEM_FAST_MOD);
  if (index >= b->raw.len) {
    return false;
  }
  BITSET_ITEM *ptr = vec_index(BITSET_ITEM, &b->raw, index);
  return *ptr & bit;
}

static bool raw_is_subset(const brw(BitSet *) superset,
                          const brw(BitSet *) subset) {
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

static void raw_intersect_with(brw(BitSet *) self, brw(BitSet *) other) {
  usize len = min(self->raw.len, other->raw.len);
  internal_resize(self, len);
  for (usize i = 0; i < len; i++) {
    *vec_index(BITSET_ITEM, &self->raw, i) &=
        *vec_index(BITSET_ITEM, &other->raw, i);
  }
  internal_shrink_len(&self->raw);
}

static BitSet raw_intersection(const brw(BitSet *) a, const brw(BitSet *) b) {
  BitSet bit_set = raw_clone(a);
  raw_intersect_with(&bit_set, b);
  return bit_set;
}

static void raw_difference_with(brw(BitSet *) self, brw(BitSet *) other) {
  usize len = min(self->raw.len, other->raw.len);
  for (usize i = 0; i < len; i++) {
    *vec_index(BITSET_ITEM, &self->raw, i) &=
        ~*vec_index(BITSET_ITEM, &other->raw, i);
  }
  internal_shrink_len(&self->raw);
}

static BitSet raw_difference(const brw(BitSet *) a, const brw(BitSet *) b) {
  BitSet bit_set = raw_clone(a);
  raw_difference_with(&bit_set, b);
  return bit_set;
}

static brw(BitSetIter) raw_iter(const brw(BitSet *) b) {
  BitSetIter iter = {.ptr = b, .offset = 0};
  return iter;
}

static usize raw_iter_next(brw(BitSetIter *) iter) {
  if (iter->offset == BITSET_ITER_END)
    return BITSET_ITER_END;

  usize offset_index = iter->offset >> BITSET_ITEM_FAST_DIV;
  usize offset_bit_n = iter->offset & BITSET_ITEM_FAST_MOD;
  for (usize i = offset_index; i < iter->ptr->raw.len; i++) {
    BITSET_ITEM bits = *vec_index(BITSET_ITEM, &iter->ptr->raw, i);
    if (bits == 0) {
      continue;
    }
    for (usize j = offset_bit_n; j < 64; j++) {
      BITSET_ITEM bit_mask = (BITSET_ITEM)1 << j;

      if ((bits & bit_mask) != 0) {
        usize offset = i * BITSET_ITEM_BITS + j;
        iter->offset = offset + 1;
        return offset;
      }
    }
    offset_bit_n = 0;
  }
  iter->offset = BITSET_ITER_END;
  return BITSET_ITER_END;
}

const struct CBitSet CBitSet = {
    .init = raw_init,

    .free = raw_free,

    .clone = raw_clone,

    .insert = raw_insert,

    .contain = raw_contain,

    .is_subset = raw_is_subset,

    .intersect_with = raw_intersect_with,

    .intersection = raw_intersection,

    .difference_with = raw_difference_with,

    .difference = raw_difference,

    .iter = raw_iter,

    .iter_next = raw_iter_next,
};
