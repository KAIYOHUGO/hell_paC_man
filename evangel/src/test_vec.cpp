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