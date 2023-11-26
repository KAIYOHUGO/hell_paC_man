#if !defined(__COMPONENT_C)
#define __COMPONENT_C

#include "component.h"
#include "array.h"
#include "bitset.h"
#include <assert.h>
#include <stdlib.h>

typedef struct ArchetypeTable ArchetypeTable;
typedef struct ArchetypeRow ArchetypeRow;
typedef struct EntityInfo EntityInfo;

struct ComponentStorage ComponentStorage = {};

void internal_component_storage_init() {
  struct ComponentStorage storage = {
      .database = vec_init(ArchetypeTable),
      .entity_info_map = map_init(EntityInfo),
      .bundle_table_map = map_init(usize),
      .type_in_table_set_map = map_init(BitSet),
  };
  ComponentStorage = storage;
}

static ComponentType raw_add_new_type() {
  usize id = internal_rand();
  map_insert(BitSet, &ComponentStorage.type_in_table_set_map, id,
             CBitSet.init());
  ComponentType ty = {.id = id};
  return ty;
}

static Entity raw_spawn(brw(Array(move_ptr(TypedComponent))) bundle) {
  usize bundle_id = 0;
  TypedComponent *typed_bundle = array_typed(TypedComponent, &bundle);
  for (usize i = 0; i < bundle.len; i++) {
    bundle_id ^= typed_bundle[i].ty.id;
  }
  usize *table_id_ptr =
      map_get(usize, &ComponentStorage.bundle_table_map, bundle_id);
  usize table_id;

  // create new table
  if (table_id_ptr == NULL) {
    table_id = ComponentStorage.database.len;

    // add in_table_set_map
    for (usize i = 0; i < bundle.len; i++) {
      BitSet *set = map_get(BitSet, &ComponentStorage.type_in_table_set_map,
                            typed_bundle[i].ty.id);
      // valgrind think this leak memory
      // in fact it's not
      // all pointer storage in `ComponentStorage.type_in_table_set_map`
      CBitSet.insert(set, table_id);
    }

    // add bundle table map
    map_insert(usize, &ComponentStorage.bundle_table_map, bundle_id, table_id);

    // create table
    // create table type_col_id_map
    Map(ComponentType, usize) type_col_id_map = map_init(usize);
    map_reserve(usize, &type_col_id_map, bundle.len);
    for (usize i = 0; i < bundle.len; i++) {
      map_insert(usize, &type_col_id_map, typed_bundle[i].ty.id, i);
    }

    // create ArchetypeTable
    ArchetypeTable table = {
        .table = vec_init(ArchetypeRow),
        .type_col_id_map = type_col_id_map,
    };
    vec_push(ArchetypeTable, &ComponentStorage.database, table);
  } else {
    table_id = *table_id_ptr;
  }

  // add new row into table
  ArchetypeTable *table =
      vec_index(ArchetypeTable, &ComponentStorage.database, table_id);
  Array(PComponent) row_components = array_init(PComponent, bundle.len);
  for (usize i = 0; i < bundle.len; i++) {
    // should not be null
    usize col_index =
        *map_get(usize, &table->type_col_id_map, typed_bundle[i].ty.id);
    *array_index(PComponent, &row_components, col_index) = typed_bundle[i].ptr;
  }

  usize row_id = table->table.len;
  Entity entity = {.id = internal_rand()};
  ArchetypeRow row = {
      .components = row_components,
      .entity = entity,
  };
  vec_push(ArchetypeRow, &table->table, row);

  // add entity info map
  EntityInfo info = {
      .children = vec_init(Entity),
      .parent = entity,
      .table = table_id,
      .index = row_id,
  };
  map_insert(EntityInfo, &ComponentStorage.entity_info_map, entity.id, info);

  return entity;
}

static void raw_remove_child(Entity parent, Entity child) {
  EntityInfo *parent_info =
      map_get(EntityInfo, &ComponentStorage.entity_info_map, parent.id);
  EntityInfo *child_info =
      map_get(EntityInfo, &ComponentStorage.entity_info_map, child.id);
  child_info->parent = child;
  assert(parent_info != NULL);
  assert(child_info != NULL);

  for (usize i = 0; i < parent_info->children.len; i++) {
    // should not be null
    Entity entity = *vec_index(Entity, &parent_info->children, i);
    if (entity.id != child.id)
      continue;
    vec_swap_remove(Entity, &parent_info->children, i);
    break;
  }
}

static void raw_add_child(Entity parent, Entity child) {
  EntityInfo *parent_info =
      map_get(EntityInfo, &ComponentStorage.entity_info_map, parent.id);
  EntityInfo *child_info =
      map_get(EntityInfo, &ComponentStorage.entity_info_map, child.id);
  child_info->parent = child;
  if (child_info->parent.id != child.id) {
    raw_remove_child(child_info->parent, child);
  }
  child_info->parent = parent;
  vec_push(Entity, &parent_info->children, child);
}

static void raw_despawn(Entity entity) {
  EntityInfo *info =
      map_get(EntityInfo, &ComponentStorage.entity_info_map, entity.id);
  if (info == NULL)
    return;

  // remove the link with parent
  if (info->parent.id != entity.id) {
    raw_remove_child(info->parent, entity);
  }

  // despawn children
  for (usize i = 0; i < info->children.len; i++) {
    // should not be null
    Entity child = *vec_index(Entity, &info->children, i);
    raw_despawn(child);
  }
  vec_free(Entity, &info->children);

  // despawn self
  ArchetypeTable *table_ptr =
      vec_index(ArchetypeTable, &ComponentStorage.database, info->table);
  ArchetypeRow *row = vec_index(ArchetypeRow, &table_ptr->table, info->index);
  Entity last_row_entity =
      vec_index(ArchetypeRow, &table_ptr->table, table_ptr->table.len - 1)
          ->entity;

  // despawn components
  for (size_t i = 0; i < row->components.len; i++) {
    PComponent *ptr = array_index(PComponent, &row->components, i);
    ptr->vtable->despawn(ptr->self, entity);
    free(ptr->self);
  }
  array_free(PComponent, &row->components);

  // update row index
  map_get(EntityInfo, &ComponentStorage.entity_info_map, last_row_entity.id)
      ->index = info->index;
  vec_swap_remove(ArchetypeRow, &table_ptr->table, info->index);
  map_remove(EntityInfo, &ComponentStorage.entity_info_map, entity.id);
}

brw(QueryIter) raw_query(brw(Array(ComponentType)) components,
                         brw(Array(ComponentType)) with) {
  ComponentType *typed_components = array_typed(ComponentType, &components);
  ComponentType *typed_with = array_typed(ComponentType, &with);

  if (components.len == 0) {
    QueryIter iter = {
        .table_iter = {.offset = 0, .ptr = NULL},
        .table = BITSET_ITER_END,
        .row = 0,
    };
    return iter;
  }

  // get table id bitset
  BitSet table_set = CBitSet.clone(map_get(
      BitSet, &ComponentStorage.type_in_table_set_map, typed_components[0].id));
  for (usize i = 1; i < components.len; i++) {
    BitSet *set = map_get(BitSet, &ComponentStorage.type_in_table_set_map,
                          typed_components[i].id);
    CBitSet.intersect_with(&table_set, set);
  }
  for (usize i = 1; i < with.len; i++) {
    BitSet *set = map_get(BitSet, &ComponentStorage.type_in_table_set_map,
                          typed_with[i].id);
    CBitSet.intersect_with(&table_set, set);
  }

  BitSet *heap_table_set = malloc(sizeof(BitSet));
  *heap_table_set = table_set;
  BitSetIter table_iter = CBitSet.iter(heap_table_set);
  usize table = CBitSet.iter_next(&table_iter);
  QueryIter iter = {
      .table_iter = table_iter,
      .components = components,
      .table = table,
      .row = 0,
  };
  return iter;
}

// NULL mean the end of query
static brw(Entity *)
    raw_query_next(brw(QueryIter *) iter, brw(Array(PComponent)) dest) {
  assert(dest.len == iter->components.len);

  if (iter->table == BITSET_ITER_END)
    return NULL;

  ArchetypeTable *table_ptr =
      vec_index(ArchetypeTable, &ComponentStorage.database, iter->table);
  ArchetypeRow *row = vec_index(ArchetypeRow, &table_ptr->table, iter->row);
  if (row == NULL) {
    iter->table = CBitSet.iter_next(&iter->table_iter);
    iter->row = 0;
    return raw_query_next(iter, dest);
  }

  ComponentType *typed_components =
      array_typed(ComponentType, &iter->components);
  for (usize i = 0; i < iter->components.len; i++) {
    usize col =
        *map_get(usize, &table_ptr->type_col_id_map, typed_components[i].id);
    *array_index(PComponent, &dest, i) =
        *array_index(PComponent, &row->components, col);
  }
  iter->row++;
  return &row->entity;
}

void raw_query_free(mov(QueryIter *) iter) { free(iter->table_iter.ptr); }

const struct CComponent CComponent = {
    .add_new_type = raw_add_new_type,

    .spawn = raw_spawn,

    .despawn = raw_despawn,

    .add_child = raw_add_child,

    .remove_child = raw_remove_child,

    .query = raw_query,

    .query_next = raw_query_next,

    .query_free = raw_query_free,
};

#endif // __COMPONENT_C