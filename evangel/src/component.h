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
  // despawn it self (don't need free `*self`)
  void (*despawn)(borrow_ptr(void *) self, Entity entity);
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
  Map(ComponentType, usize) type_col_id_map;
  Vec(Array) table;
};

struct EntityInfo {
  Entity parent;
  Vec(Entity) children;
  usize table, index;
};

struct ComponentStorage {
  Vec(struct ArchetypeTable) database;
  Map(Entity, EntityInfo) entity_info_map;
  Map(usize, usize) bundle_table_map;
};

struct CComponent {
  ComponentType (*add_new_type)();

  // it don't move the array
  // but it will move the value inside
  Entity (*spawn)(borrow_ptr(Array(move_ptr(TypedComponent)) *) bundle);

  void (*despawn)(Entity entity);

  void (*add_child)(Entity parent, Entity child);

  void (*remove_child)(Entity parent, Entity child);
};

extern const struct CComponent CComponent;

#define AddComponentType(T)                                                    \
  ComponentType ComponentType##T;                                              \
  void add_component_type_##T() {                                              \
    ComponentType##T = CComponent.add_new_type();                              \
  }

void internal_component_storage_init();

#endif // __COMPONENT_H
