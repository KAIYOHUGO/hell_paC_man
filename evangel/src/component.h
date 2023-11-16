#if !defined(__COMPONENT_H)
#define __COMPONENT_H

#include "basic.h"
#include "map.h"
#include "vec.h"

// forward declaration
struct ComponentNode;

typedef struct {
  usize id;
} ComponentType;

typedef struct {
  struct ComponentNode *ptr;
} Entity;

typedef struct {
  // despawn and free up memory (*self)
  void (*despawn)(void *self, Entity entity);
} VComponent;

typedef struct {
  const VComponent *vtable;
  void *self;
} PComponent;

struct ComponentNode {
  bool is_despawn;
  ComponentType ty;
  PComponent component;
  Entity parent;
  Vec(Entity) children;
};

struct ComponentTree {
  // a lazy clean up map
  Vec(Vec(struct ComponentNode *)) typeid_component_node_map;
  // typeid map
  // Map(ComponentType) entity_typeid_map;
};

struct CComponent {
  ComponentType (*add_new_type)();

  Entity (*spawn)(ComponentType typeid, PComponent component);

  void (*add_child)(Entity parent, Entity child);

  void (*despawn)(Entity entity);
};

extern const struct CComponent CComponent;

#define AddComponentType(T)                                                    \
  ComponentType ComponentType##T;                                              \
  void add_component_type_##T() {                                              \
    ComponentType##T = CComponent.add_new_type();                              \
  }

#endif // __COMPONENT_H
