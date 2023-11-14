#if !defined(__COMPONENT_H)
#define __COMPONENT_H

#include "basic.h"

typedef struct {
  usize id;
} Entity;

typedef struct {

} VComponent;

typedef struct {
  const VComponent *vtable;
  void *self;
} PComponent;

struct CComponent {};

#endif // __COMPONENT_H
