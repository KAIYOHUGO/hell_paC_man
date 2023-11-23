#if !defined(__COMPONENT_C)
#define __COMPONENT_C

#include "component.h"
#include "app.h"
#include "array.h"
#include <assert.h>
#include <stdlib.h>

typedef struct ArchetypeTable ArchetypeTable;
typedef struct EntityInfo EntityInfo;
typedef struct ComponentStorage ComponentStorage;

static usize internal_rand() { return (rand() << 16) ^ rand(); }

void internal_component_storage_init() {
  ComponentStorage storage = {
      .database = vec_init(ArchetypeTable),
      .entity_info_map = map_init(EntityInfo),
      .bundle_table_map = map_init(usize),
  };
  App.component_storage = storage;
}

static Array(PComponent) *
    internal_database_query_row(usize table, usize index) {
  ArchetypeTable *table_ptr =
      vec_index(ArchetypeTable, &App.component_storage.database, table);
  Array(PComponent) *row =
      vec_index(Array(PComponent), &table_ptr->table, index);
  return row;
}

static ComponentType raw_add_new_type() {
  ComponentType ty = {.id = internal_rand()};
  return ty;
}

static Entity raw_spawn(borrow_ptr(Array(move_ptr(TypedComponent)) *) bundle) {
  usize bundle_id = 0;
  TypedComponent *typed_bundle = array_typed(TypedComponent, bundle);
  for (usize i = 0; i < bundle->len; i++) {
    bundle_id ^= typed_bundle[i].ty.id;
  }
  usize *table_id_ptr =
      map_get(usize, &App.component_storage.bundle_table_map, bundle_id);
  usize table_id;

  // create new table
  if (table_id_ptr == NULL) {
    table_id = App.component_storage.database.len;
    map_insert(usize, &App.component_storage.bundle_table_map, bundle_id,
               table_id);
    Map(ComponentType, usize) type_col_id_map = map_init(usize);
    map_reserve(usize, &type_col_id_map, bundle->len);

    for (usize i = 0; i < bundle->len; i++) {
      map_insert(usize, &type_col_id_map, typed_bundle[i].ty.id, i);
    }
    ArchetypeTable table = {
        .table = vec_init(Array(PComponent)),
        .type_col_id_map = type_col_id_map,
    };
    vec_push(ArchetypeTable, &App.component_storage.database, table);
  } else {
    table_id = *table_id_ptr;
  }

  // add new row into table
  ArchetypeTable *table =
      vec_index(ArchetypeTable, &App.component_storage.database, table_id);
  Array(PComponent) row = array_init(PComponent, bundle->len);
  for (usize i = 0; i < bundle->len; i++) {
    // should not be null
    usize col_index =
        *map_get(usize, &table->type_col_id_map, typed_bundle[i].ty.id);
    *array_index(PComponent, &row, col_index) = typed_bundle[i].ptr;
  }
  usize row_id = table->table.len;
  vec_push(Array(PComponent), &table->table, row);

  // add entity info map
  Entity entity = {.id = internal_rand()};
  EntityInfo info = {
      .children = vec_init(Entity),
      .parent = entity,
      .table = table_id,
      .index = row_id,
  };
  map_insert(EntityInfo, &App.component_storage.entity_info_map, entity.id,
             info);

  return entity;
}

static void raw_remove_child(Entity parent, Entity child) {
  EntityInfo *parent_info =
      map_get(EntityInfo, &App.component_storage.entity_info_map, parent.id);
  EntityInfo *child_info =
      map_get(EntityInfo, &App.component_storage.entity_info_map, child.id);
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
      map_get(EntityInfo, &App.component_storage.entity_info_map, parent.id);
  EntityInfo *child_info =
      map_get(EntityInfo, &App.component_storage.entity_info_map, child.id);
  child_info->parent = child;
  if (child_info->parent.id != child.id) {
    raw_remove_child(child_info->parent, child);
  }
  child_info->parent = parent;
  vec_push(Entity, &parent_info->children, child);
}

static void raw_despawn(Entity entity) {
  EntityInfo *info =
      map_get(EntityInfo, &App.component_storage.entity_info_map, entity.id);
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

  // despawn self
  Array(PComponent) *row =
      internal_database_query_row(info->table, info->index);
  for (size_t i = 0; i < row->len; i++) {
    PComponent *ptr = array_index(PComponent, row, i);
    ptr->vtable->despawn(ptr->self, entity);
    free(ptr->self);
  }
  vec_free(Entity, &info->children);
  map_remove(EntityInfo, &App.component_storage.entity_info_map, entity.id);
}

const struct CComponent CComponent = {
    .add_new_type = raw_add_new_type,

    .spawn = raw_spawn,

    .despawn = raw_despawn,

    .add_child = raw_add_child,

    .remove_child = raw_remove_child,
};

#endif // __COMPONENT_C