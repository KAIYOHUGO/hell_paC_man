#include "array.h"
#include "component.h"
#include <gtest/gtest.h>

namespace bridge {
extern "C" void internal_component_storage_init(void);
}

struct MockComponent {
  u8 value[50];
};

struct MockComponent1 {
  u8 value[20];
};

struct MockComponent2 {
  u8 value[100];
};

struct MockComponent3 {
  u8 value[130];
};

struct MockComponent4 {
  u8 value[10];
};

void mock_despawn(void *self, Entity entity) {}

const VComponent MockVComponent{
    .despawn = mock_despawn,
};

AddComponentType(MockComponent);
AddComponentType(MockComponent1);
AddComponentType(MockComponent2);
AddComponentType(MockComponent3);
AddComponentType(MockComponent4);

PComponent mock_component_to_ptr(void *component) {
  return {
      .vtable = &MockVComponent,
      .self = component,
  };
}

isize main(isize argc, char **argv) {
  bridge::internal_component_storage_init();
  add_component_type_MockComponent();
  add_component_type_MockComponent1();
  add_component_type_MockComponent2();
  add_component_type_MockComponent3();
  add_component_type_MockComponent4();
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ComponentTest, SpawnDespawn) {
  Vec(Entity) entities = vec_init(Entity);
  for (usize i = 0; i < 1; i++) {
    MockComponent *ptr = new MockComponent;
    MockComponent1 *ptr1 = new MockComponent1;
    TypedComponent bundle[] = {
        {.ty = ComponentTypeMockComponent, .ptr = mock_component_to_ptr(ptr)},
        {.ty = ComponentTypeMockComponent1, .ptr = mock_component_to_ptr(ptr1)},
    };
    Array(TypedComponent) arr = array_ref(TypedComponent, bundle, 2);
    Entity entity = CComponent.spawn(&arr);
    vec_push(Entity, &entities, entity);
  }
  for (usize i = 0; i < 0; i++) {
    MockComponent *ptr = new MockComponent;
    MockComponent1 *ptr1 = new MockComponent1;
    MockComponent2 *ptr2 = new MockComponent2;
    TypedComponent bundle[] = {
        {.ty = ComponentTypeMockComponent, .ptr = mock_component_to_ptr(ptr)},
        {.ty = ComponentTypeMockComponent1, .ptr = mock_component_to_ptr(ptr1)},
        {.ty = ComponentTypeMockComponent2, .ptr = mock_component_to_ptr(ptr2)},
    };
    Array(TypedComponent) arr = array_ref(TypedComponent, bundle, 3);
    Entity entity = CComponent.spawn(&arr);
    vec_push(Entity, &entities, entity);
  }
  for (usize i = 0; i < entities.len; i++) {
    CComponent.despawn(*vec_index(Entity, &entities, i));
  }
  vec_free(Entity, &entities);
}

// TEST(ComponentTest, AddRemoveChildren) {
//   MockComponent *ptr = new MockComponent;
//   MockComponent1 *ptr1 = new MockComponent1;
//   TypedComponent bundle[] = {
//       {.ty = ComponentTypeMockComponent, .ptr = mock_component_to_ptr(ptr)},
//       {.ty = ComponentTypeMockComponent1, .ptr = mock_component_to_ptr(ptr1)},
//   };
//   Array(TypedComponent) arr = array_ref(TypedComponent, bundle, 2);
//   Entity parent = CComponent.spawn(&arr);

//   Vec(Entity) entities = vec_init(Entity);
//   for (size_t i = 0; i < 50; i++) {
//     MockComponent2 *ptr2 = new MockComponent2;
//     MockComponent3 *ptr3 = new MockComponent3;
//     MockComponent4 *ptr4 = new MockComponent4;
//     TypedComponent bundle[] = {
//         {.ty = ComponentTypeMockComponent2, .ptr = mock_component_to_ptr(ptr2)},
//         {.ty = ComponentTypeMockComponent3, .ptr = mock_component_to_ptr(ptr3)},
//         {.ty = ComponentTypeMockComponent4, .ptr = mock_component_to_ptr(ptr4)},
//     };
//     Array(TypedComponent) arr = array_ref(TypedComponent, bundle, 3);
//     Entity entity = CComponent.spawn(&arr);
//     CComponent.add_child(parent, entity);
//     if (i % 3 == 0) {
//       vec_push(Entity, &entities, entity);
//     }
//   }
//   for (usize i = 0; i < entities.len; i++) {
//     Entity entity = *vec_index(Entity, &entities, i);
//     CComponent.remove_child(parent, entity);
//     CComponent.despawn(entity);
//   }
//   vec_free(Entity, &entities);
//   CComponent.despawn(parent);
// }

// TEST(ComponentTest, Query) {
//   Vec(Entity) entities = vec_init(Entity);

//   for (usize i = 0; i < 10; i++) {
//     MockComponent *ptr = new MockComponent;
//     MockComponent1 *ptr1 = new MockComponent1;
//     TypedComponent bundle[] = {
//         {.ty = ComponentTypeMockComponent, .ptr = mock_component_to_ptr(ptr)},
//         {.ty = ComponentTypeMockComponent1, .ptr = mock_component_to_ptr(ptr1)},
//     };
//     Array(TypedComponent) arr = array_ref(TypedComponent, bundle, 2);
//     Entity entity = CComponent.spawn(&arr);
//     vec_push(Entity, &entities, entity);
//   }

//   Vec(Entity) entities1 = vec_init(Entity);
//   for (usize i = 0; i < 20; i++) {
//     MockComponent *ptr = new MockComponent;
//     MockComponent1 *ptr1 = new MockComponent1;
//     MockComponent2 *ptr2 = new MockComponent2;
//     TypedComponent bundle[] = {
//         {.ty = ComponentTypeMockComponent, .ptr = mock_component_to_ptr(ptr)},
//         {.ty = ComponentTypeMockComponent1, .ptr = mock_component_to_ptr(ptr1)},
//         {.ty = ComponentTypeMockComponent2, .ptr = mock_component_to_ptr(ptr2)},
//     };
//     Array(TypedComponent) arr = array_ref(TypedComponent, bundle, 3);
//     Entity entity = CComponent.spawn(&arr);
//     vec_push(Entity, &entities1, entity);
//   }

//   ComponentType query[] = {ComponentTypeMockComponent,
//                            ComponentTypeMockComponent1};
//   QueryIter iter = CComponent.query(array_ref(ComponentType, query, 2),
//                                     array_empty(ComponentType));
//   ComponentType result[2];
//   Entity *result_entity;
//   for (usize i = 0; i < entities.len; i++) {
//     result_entity =
//         CComponent.query_next(&iter, array_ref(ComponentType, result, 2));
//     Entity *expect = vec_index(Entity, &entities, i);
//     EXPECT_EQ(expect->id, result_entity->id);
//   }
//   for (usize i = 0; i < entities1.len; i++) {
//     result_entity =
//         CComponent.query_next(&iter, array_ref(ComponentType, result, 2));
//     Entity *expect = vec_index(Entity, &entities1, i);
//     EXPECT_EQ(expect->id, result_entity->id);
//   }
//   CComponent.query_free(&iter);
//   vec_free(Entity, &entities);
//   vec_free(Entity, &entities1);
// }