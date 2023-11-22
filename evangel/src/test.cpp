#include "bitset.h"
#include "map.h"
#include "vec.h"
#include <gtest/gtest.h>

TEST(VecTest, PushAndPop) {
  Vec v = vec_init(isize);

  for (isize i = 0; i <= 100; i++) {
    vec_push(isize, &v, i);
  }
  EXPECT_EQ(101, v.len);
  EXPECT_TRUE(v.cap >= 101);
  for (isize i = 100; i >= 0; i--) {
    EXPECT_EQ(i, vec_pop(isize, &v));
  }
  EXPECT_EQ(0, v.len);
  vec_free(isize, &v);
}

TEST(VecTest, ResizeAndIndex) {
  Vec v = vec_init(isize);
  EXPECT_EQ(0, v.len);
  EXPECT_EQ(0, v.cap);

  vec_resize(isize, &v, 101);
  EXPECT_EQ(101, v.len);
  EXPECT_TRUE(v.cap >= 101);

  for (isize i = 0; i <= 100; i++) {
    auto ptr = vec_index(isize, &v, i);
    *ptr = i;
  }
  for (isize i = 0; i <= 100; i++) {
    auto ptr = vec_index(isize, &v, i);
    EXPECT_EQ(i, *ptr);
  }
  vec_free(isize, &v);
}

TEST(VecTest, Remove) {
  Vec v = vec_init(isize);
  vec_resize(isize, &v, 101);

  for (isize i = 0; i <= 100; i++) {
    auto ptr = vec_index(isize, &v, i);
    *ptr = i;
  }
  vec_remove(isize, &v, 50);
  EXPECT_EQ(100, v.len);
  for (isize i = 0; i < 50; i++) {
    auto ptr = vec_index(isize, &v, i);
    EXPECT_EQ(i, *ptr);
  }
  for (isize i = 50; i < 100; i++) {
    auto ptr = vec_index(isize, &v, i);
    EXPECT_EQ(i + 1, *ptr);
  }

  vec_free(isize, &v);
}

TEST(MapTest, InsertGetAndRemove) {
  Map(u64, u64) m = map_init(u64);
  for (u64 i = 0; i < 100; i++) {
    map_insert(u64, &m, i, i);
  }
  EXPECT_EQ(m.len, 100);

  for (u64 i = 0; i < 100; i++) {
    auto ptr = map_get(u64, &m, i);
    EXPECT_EQ(i, *ptr);
    map_remove(u64, &m, i);
  }
  for (u64 i = 0; i < 100; i += 2) {
    map_insert(u64, &m, i, i);
  }
  EXPECT_EQ(m.len, 50);

  for (u64 i = 0; i < 100; i += 2) {
    auto ptr = map_get(u64, &m, i + 1);
    EXPECT_EQ(nullptr, ptr);
  }

  map_free(isize, &m);
}

TEST(BitSetTest, InsertAndContain) {
  BitSet set = CBitSet.init();
  for (size_t i = 0; i < 100; i += 3) {
    CBitSet.insert(&set, i);
  }

  for (size_t i = 0; i < 100; i += 3) {
    EXPECT_TRUE(CBitSet.contain(&set, i));
  }

  CBitSet.free(&set);
}

TEST(BitSetTest, Intersection) {
  BitSet a = CBitSet.init();
  BitSet b = CBitSet.init();
  for (size_t i = 0; i < 100; i += 2) {
    CBitSet.insert(&a, i);
  }
  for (size_t i = 0; i < 100; i += 3) {
    CBitSet.insert(&b, i);
  }
  BitSet c = CBitSet.intersection(&a, &b);
  for (size_t i = 0; i < 100; i += 6) {
    EXPECT_TRUE(CBitSet.contain(&c, i));
  }

  CBitSet.free(&a);
  CBitSet.free(&b);
  CBitSet.free(&c);
}
