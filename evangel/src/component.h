#if !defined(__COMPONENT_H)
#define __COMPONENT_H

#include "array.h"
#include "basic.h"
#include "bitset.h"
#include "map.h"
#include "vec.h"

// random id
typedef struct {
  usize id;
} ComponentType;

typedef struct {
  usize id;
} Entity;

typedef struct {
  // despawn it self (don't need free `*self`)
  void (*despawn)(brw(void *) self, Entity entity);
} VComponent;

typedef struct {
  const VComponent *vtable;
  void *self;
} PComponent;

typedef struct {
  ComponentType ty;
  PComponent ptr;
} TypedComponent;

struct ArchetypeRow {
  Entity entity;
  Array(PComponent) components;
};

struct ArchetypeTable {
  Map(ComponentType, usize) type_col_id_map;
  Vec(struct ArchetypeRow) table;
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
  Map(ComponentType, BitSet) type_in_table_set_map;
};

typedef struct {
  BitSetIter table_iter;
  brw(Array(ComponentType)) components;
  usize table, row;
} QueryIter;

struct CComponent {
  ComponentType (*add_new_type)();

  // it don't move the array
  // but it will move the value inside
  Entity (*spawn)(brw(Array(mov(TypedComponent))) bundle);

  void (*despawn)(Entity entity);

  void (*add_child)(Entity parent, Entity child);

  void (*remove_child)(Entity parent, Entity child);

  brw(QueryIter) (*query)(brw(Array(ComponentType)) components,
                          brw(Array(ComponentType)) with);

  brw(Entity *) (*query_next)(brw(QueryIter *) iter,
                              brw(Array(PComponent)) dest);

  void (*query_free)(mov(QueryIter *) iter);

  PComponent (*default_vtable)(void *component);
};

extern struct ComponentStorage ComponentStorage;

extern const struct CComponent CComponent;

#define CTy(T) ComponentType##T

#define DeclareComponentType(T)                                                \
  ComponentType CTy(T);                                                        \
  void add_component_type_##T() { CTy(T) = CComponent.add_new_type(); }

#define ExportComponentType(T)                                                 \
  extern ComponentType CTy(T);                                                 \
  void add_component_type_##T();

#define add_component_type(T) add_component_type_##T()

#define CTyped(T, C)                                                           \
  { .ty = CTy(T), .ptr = C }

#define InternalBundle2(T1, C1) CTyped(T1, C1)
#define InternalBundle4(T1, T2, C1, C2) InternalBundle2(T1, C1), CTyped(T2, C2)
#define InternalBundle6(T1, T2, T3, C1, C2, C3)                                \
  InternalBundle4(T1, T2, C1, C2), CTyped(T3, C3)
#define InternalBundle8(T1, T2, T3, T4, C1, C2, C3, C4)                        \
  InternalBundle6(T1, T2, T3, C1, C2, C3), CTyped(T4, C4)
#define InternalBundle10(T1, T2, T3, T4, T5, C1, C2, C3, C4, C5)               \
  InternalBundle8(T1, T2, T3, T4, C1, C2, C3, C4), CTyped(T5, C5)
#define InternalBundle12(T1, T2, T3, T4, T5, T6, C1, C2, C3, C4, C5, C6)       \
  InternalBundle10(T1, T2, T3, T4, T5, C1, C2, C3, C4, C5), CTyped(T6, C6)

// https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define InternalOverload_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,   \
                          NAME, ...)                                           \
  NAME
#define InternalOverload(...)                                                  \
  InternalOverload_(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define Bundle(...)                                                            \
  {                                                                            \
    InternalConcatIdent(InternalBundle,                                        \
                        InternalOverload(__VA_ARGS__))(__VA_ARGS__)            \
  }

#define Spawn(...)                                                             \
  ({                                                                           \
    TypedComponent InternalConcatIdent(bundle, __LINE__)[] =                   \
        Bundle(__VA_ARGS__);                                                   \
    CComponent.spawn(                                                          \
        array_ref(TypedComponent, InternalConcatIdent(bundle, __LINE__)));     \
  })

#define InternalTySet1(T1) CTy(T1)
#define InternalTySet2(T1, T2) InternalTySet1(T1), CTy(T2)
#define InternalTySet3(T1, T2, T3) InternalTySet2(T1, T2), CTy(T3)
#define InternalTySet4(T1, T2, T3, T4) InternalTySet3(T1, T2, T3), CTy(T4)
#define InternalTySet5(T1, T2, T3, T4, T5)                                     \
  InternalTySet4(T1, T2, T3, T4), CTy(T5)

#define TySet(...)                                                             \
  {                                                                            \
    InternalConcatIdent(InternalTySet,                                         \
                        InternalOverload(__VA_ARGS__))(__VA_ARGS__)            \
  }

void internal_component_storage_init();

#endif // __COMPONENT_H
