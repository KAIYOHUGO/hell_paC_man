#if !defined(__COMPONENT_C)
#define __COMPONENT_C

#include "component.h"
#include "app.h"
#include <assert.h>
#include <stdlib.h>

static ComponentType raw_add_new_type() {
  ComponentType ty = {.id = rand()};
  return ty;
}

static Entity raw_spawn(borrow(Array(TypedComponent)) bundle) {
  
}

// static ComponentType raw_add_new_type() {
//   vec_push(Vec(ComponentNode *),
//   &App.component_tree.typeid_component_node_map,
//            vec_init(ComponentNode *));
//   ComponentType typeid = {App.component_tree.typeid_component_node_map.len -
//   1}; return typeid;
// }

// static Entity raw_spawn(ComponentType typeid, PComponent component) {
//   Vec(ComponentNode) *ptr =
//       vec_index(Vec(ComponentNode),
//                 &App.component_tree.typeid_component_node_map, typeid.id);
//   ComponentNode *node = (ComponentNode *)malloc(sizeof(ComponentNode));
//   node->component = component;
//   node->children = vec_init(ComponentNode *);
//   Entity parent = {NULL};
//   node->parent = parent;
//   node->ty = typeid;
//   node->is_despawn = false;
//   vec_push(ComponentNode *, ptr, node);
//   Entity entity = {node};
//   return entity;
// }

// static void raw_add_child(Entity parent, Entity child) {
//   assert(child.ptr->parent.ptr == NULL);
//   vec_push(Entity, &parent.ptr->children, child);
//   child.ptr->parent = parent;
// }

// static void raw_despawn(Entity entity) {
//   entity.ptr->component.vtable->despawn(entity.ptr->component.self, entity);
//   entity.ptr->is_despawn = true;
//   for (size_t i = 0; i < entity.ptr->children.len; i++) {
//     Entity *child = vec_index(Entity, &entity.ptr->children, i);
//     raw_despawn(*child);
//   }
//   vec_free(Entity, &entity.ptr->children);
// }

// const struct CComponent CComponent = {
//     .add_new_type = raw_add_new_type,

//     .spawn = raw_spawn,

//     .add_child = raw_add_child,

//     .despawn = raw_despawn,
// };

#endif // __COMPONENT_C