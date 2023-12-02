#if !defined(__BASIC_H)
#define __BASIC_H

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned int usize;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef int isize;

// FIXME: Not fixed sized
typedef double f32;
typedef long double f64;

usize internal_rand();

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#define mov(ptr) ptr

#define brw(ptr) ptr

// https://stackoverflow.com/questions/1082192/how-to-generate-random-variable-names-in-c-using-macros
#define InternalConcatIdent(A, B) InternalConcatIdent_(A, B)

#define InternalConcatIdent_(A, B) A##B

// https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define InternalOverload_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,   \
                          NAME, ...)                                           \
  NAME
#define InternalOverload(...)                                                  \
  InternalOverload_(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#endif // __BASIC_H
