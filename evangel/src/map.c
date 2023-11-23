#if !defined(__MAP_C)
#define __MAP_C

#include "map.h"
#include <stdlib.h>
#include <string.h>

Map raw_init(usize sizeof_T) {
  Map map = {
      .len = 0,
      .size = sizeof_T,
      .buckets = vec_init(Vec(MapEntry)),
  };
  return map;
}

static void raw_free(Map *m) {
  usize buckets_i = m->buckets.len;
  while (buckets_i--) {
    Vec(T) *entry = vec_index(Vec(MapEntry), &m->buckets, buckets_i);
    vec_free(Vec(MapEntry), entry);
  }
  vec_free(Vec(Vec(MapEntry)), &m->buckets);
}

static void *raw_get(Map *m, usize key) {
  if (m->buckets.len == 0)
    return NULL;
  usize bucket_index = key % m->buckets.len;
  Vec(MapEntry) *bucket = vec_index(Vec(MapEntry), &m->buckets, bucket_index);
  for (usize i = 0; i < bucket->len; i++) {
    MapEntry *entry = vec_index(MapEntry, bucket, i);
    if (entry->key == key) {
      return &entry->value;
    }
  }
  return NULL;
}

static void raw_reserve(Map *m, usize additional) {
  usize new_size = m->buckets.len + additional;
  usize dirty_range = m->buckets.len;
  vec_resize(T, &m->buckets, new_size);

  for (size_t i = dirty_range; i < new_size; i++) {
    Vec(MapEntry) new_bucket = CVec.init(sizeof(MapEntry) + m->size);
    *vec_index(Vec(MapEntry), &m->buckets, i) = new_bucket;
  }

  for (usize bucket_index = 0; bucket_index < dirty_range; bucket_index++) {
    Vec(MapEntry) *bucket = vec_index(Vec(MapEntry), &m->buckets, bucket_index);

    usize entry_len = bucket->len;
    for (usize i = 0, entry_index = 0;
         i < entry_len && entry_index < bucket->len; i++) {
      MapEntry *entry = vec_index(MapEntry, bucket, entry_index);
      usize new_bucket_index = entry->key % new_size;
      if (new_bucket_index == bucket_index) {
        entry_index++;
        continue;
      }

      Vec(MapEntry) *new_bucket =
          vec_index(Vec(MapEntry), &m->buckets, new_bucket_index);

      // MapEntry is unsized
      // so memcpy hack require
      MapEntry *ptr = (MapEntry *)CVec.push(new_bucket);
      memcpy(ptr, entry, sizeof(MapEntry) + m->size);
      vec_swap_remove(MapEntry, bucket, entry_index);
    }
  }
}

static void *raw_insert(Map *m, usize key) {
  usize max_entry = (m->buckets.len * 3 + 3) / 4;
  if (m->len >= max_entry) {
    raw_reserve(m, max(m->buckets.len, 4));
  }
  usize bucket_index = key % m->buckets.len;
  Vec(MapEntry) *bucket = vec_index(Vec(MapEntry), &m->buckets, bucket_index);
  for (usize i = 0; i < bucket->len; i++) {
    MapEntry *entry = vec_index(MapEntry, bucket, i);
    if (entry->key != key) {
      continue;
    }
    return &entry->value;
  }
  MapEntry *ptr = (MapEntry *)CVec.push(bucket);
  ptr->key = key;
  m->len++;
  return &ptr->value;
}

static void raw_remove(Map *m, usize key) {
  usize bucket_index = key % m->buckets.len;
  Vec(MapEntry) *bucket = vec_index(Vec(MapEntry), &m->buckets, bucket_index);
  for (usize i = 0; i < bucket->len; i++) {
    MapEntry *entry = vec_index(MapEntry, bucket, i);
    if (entry->key != key) {
      continue;
    }
    vec_swap_remove(MapEntry, bucket, i);
  }
  m->len--;
}

const struct CMap CMap = {
    .init = raw_init,
    .free = raw_free,
    .get = raw_get,
    .reserve = raw_reserve,
    .insert = raw_insert,
    .remove = raw_remove,
};

#endif // __MAP_C