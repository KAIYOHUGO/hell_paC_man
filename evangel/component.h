#if !defined(__COMPONENT_H)
#define __COMPONENT_H

#include "basic.h"

typedef struct {
  usize id;
} Entity;

typedef struct {
  Entity entity;
} IComponent;



#endif // __COMPONENT_H
