#if !defined(__COMPONENT_H)
#define __COMPONENT_H

#include "array.h"
#include "basic.h"
#include "map.h"
#include "vec.h"

// random id
typedef struct {
  usize id;
} ComponentType;

// xor `ComponentType` id
typedef struct {
  usize id;
} BundleType;

typedef struct {
  usize id;
} Entity;

typedef struct {
  // despawn and free up memory (*self)
  void (*despawn)(void *self, Entity entity);
} VComponent;

typedef struct {
  const VComponent *vtable;
  void *self;
} PComponent;

typedef struct {
  ComponentType ty;
  PComponent ptr;
} TypedComponent;

struct ArchetypeTable {
  const Map(Entity, usize) entity_col_id_map;
  Vec(Array) table;
};

struct EntityInfo {
  Entity parent;
  Vec(Entity) children;
  usize table, index;
};

struct ComponentStorage {
  Vec(ArchetypeTable) database;
  Map(Entity, EntityInfo) entity_info_map;
  Map(BundleType, usize) bundle_table_map;
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
