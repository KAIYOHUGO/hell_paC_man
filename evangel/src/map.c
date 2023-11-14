#if !defined(__MAP_C)
#define __MAP_C

#include "map.h"
#include <stdlib.h>

typedef struct MapEntry MapEntry;

Map raw_map_init() {
  Map map = {
      .len = 0,
      .buckets = vec_init(Vec(MapEntry)),
  };
  return map;
}

void raw_map_free(Map *m) {
  usize buckets_i = m->buckets.len;
  while (buckets_i--) {
    Vec(T) *entry = vec_index(Vec(MapEntry), &m->buckets, buckets_i);
    vec_free(Vec(MapEntry), entry);
  }
  vec_free(Vec(Vec(MapEntry)), &m->buckets);
}

void *raw_map_get(Map *m, usize key) {
  usize bucket_index = key % m->buckets.len;
  Vec(MapEntry) *bucket = vec_index(Vec(MapEntry), &m->buckets, bucket_index);
  for (usize i = 0; i < bucket->len; i++) {
    MapEntry *entry = vec_index(MapEntry, bucket, i);
    if (entry->key == key) {
      return entry->value;
    }
  }
  return NULL;
}

void raw_map_reserve(Map *m, usize additional) {
  usize new_size = m->len + additional;
  usize dirty_range = m->buckets.len;
  vec_resize(T, &m->buckets, new_size);
  for (usize bucket_index = 0; bucket_index < dirty_range; bucket_index++) {
    Vec(MapEntry) *bucket = vec_index(Vec(MapEntry), &m->buckets, bucket_index);

    for (usize entry_index = 0; entry_index < bucket->len; entry_index++) {
      MapEntry *entry = vec_index(MapEntry, bucket, entry_index);
      usize new_bucket_index = entry->key % new_size;
      if (new_bucket_index == bucket_index) {
        continue;
      }

      Vec(MapEntry) *new_bucket =
          vec_index(Vec(MapEntry), &m->buckets, new_bucket_index);

      vec_push(MapEntry, new_bucket, *entry);
      vec_swap_remove(MapEntry, bucket, entry_index);
    }
  }
}

void *raw_map_insert(Map *m, usize key) {
  usize max_entry = (m->buckets.len * 3 + 3) / 4;
  if (m->len + 1 >= max_entry) {
    raw_map_reserve(m, m->buckets.len);
  }
  usize bucket_index = key % m->buckets.len;
  Vec(MapEntry) *bucket = vec_index(Vec(MapEntry), &m->buckets, bucket_index);
  for (usize i = 0; i < bucket->len; i++) {
    MapEntry *entry = vec_index(MapEntry, bucket, i);
    if (entry->key != key) {
      continue;
    }
    vec_swap_remove(MapEntry, bucket, i);
    break;
  }
  return CVec.push(bucket);
}

#endif // __MAP_C