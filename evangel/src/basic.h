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

#endif // __BASIC_H
