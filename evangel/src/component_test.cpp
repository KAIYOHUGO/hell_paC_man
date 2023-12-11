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

DeclareComponentType(MockComponent);
DeclareComponentType(MockComponent1);
DeclareComponentType(MockComponent2);
DeclareComponentType(MockComponent3);
DeclareComponentType(MockComponent4);

PComponent mock_to_ptr(void *component) {
  return {
      .vtable = &MockVComponent,
      .self = component,
  };
}

isize main(isize argc, char **argv) {
  bridge::internal_component_storage_init();
  add_component_type(MockComponent);
  add_component_type(MockComponent1);
  add_component_type(MockComponent2);
  add_component_type(MockComponent3);
  add_component_type(MockComponent4);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ComponentTest, SpawnDespawn) {
  Vec(Entity) entities = vec_init(Entity);
  for (usize i = 0; i < 10; i++) {
    MockComponent *ptr = new MockComponent;
    MockComponent1 *ptr1 = new MockComponent1;
    TypedComponent bundle[] = {
        {.ty = CTy(MockComponent), .ptr = mock_to_ptr(ptr)},
        {.ty = CTy(MockComponent1), .ptr = mock_to_ptr(ptr1)},
    };
    Entity entity = CComponent.spawn(array_ref(bundle));
    vec_push(Entity, &entities, entity);
  }
  for (usize i = 0; i < 50; i++) {
    MockComponent *ptr = new MockComponent;
    MockComponent1 *ptr1 = new MockComponent1;
    MockComponent2 *ptr2 = new MockComponent2;
    TypedComponent bundle[] = {
        {.ty = CTy(MockComponent), .ptr = mock_to_ptr(ptr)},
        {.ty = CTy(MockComponent1), .ptr = mock_to_ptr(ptr1)},
        {.ty = CTy(MockComponent2), .ptr = mock_to_ptr(ptr2)},
    };
    Entity entity = CComponent.spawn(array_ref(bundle));
    vec_push(Entity, &entities, entity);
  }
  for (usize i = 0; i < entities.len; i++) {
    CComponent.despawn(*vec_index(Entity, &entities, i));
  }
  vec_free(Entity, &entities);
}

TEST(ComponentTest, AddRemoveChildren) {
  MockComponent *ptr = new MockComponent;
  MockComponent1 *ptr1 = new MockComponent1;
  Entity parent =
      Spawn(MockComponent, MockComponent1, mock_to_ptr(ptr), mock_to_ptr(ptr1));
  Vec(Entity) entities = vec_init(Entity);
  for (size_t i = 0; i < 50; i++) {
    MockComponent2 *ptr2 = new MockComponent2;
    MockComponent3 *ptr3 = new MockComponent3;
    MockComponent4 *ptr4 = new MockComponent4;
    TypedComponent bundle[] = {
        {.ty = CTy(MockComponent2), .ptr = mock_to_ptr(ptr2)},
        {.ty = CTy(MockComponent3), .ptr = mock_to_ptr(ptr3)},
        {.ty = CTy(MockComponent4), .ptr = mock_to_ptr(ptr4)},
    };
    Entity entity = CComponent.spawn(array_ref(bundle));
    CComponent.add_child(parent, entity);
    if (i % 3 == 0) {
      vec_push(Entity, &entities, entity);
    }
  }
  for (usize i = 0; i < entities.len; i++) {
    Entity entity = *vec_index(Entity, &entities, i);
    CComponent.remove_child(parent, entity);
    CComponent.despawn(entity);
  }
  vec_free(Entity, &entities);
  CComponent.despawn(parent);
}

TEST(ComponentTest, Query) {
  Vec(Entity) entities = vec_init(Entity);

  for (usize i = 0; i < 10; i++) {
    MockComponent *ptr = new MockComponent;
    MockComponent1 *ptr1 = new MockComponent1;
    TypedComponent bundle[] = Bundle(MockComponent, MockComponent1,
                                     mock_to_ptr(ptr), mock_to_ptr(ptr1));
    Entity entity = CComponent.spawn(array_ref(bundle));
    vec_push(Entity, &entities, entity);
  }

  Vec(Entity) entities1 = vec_init(Entity);
  for (usize i = 0; i < 20; i++) {
    MockComponent *ptr = new MockComponent;
    MockComponent1 *ptr1 = new MockComponent1;
    MockComponent2 *ptr2 = new MockComponent2;

    Entity entity =
        Spawn(MockComponent, MockComponent1, MockComponent2, mock_to_ptr(ptr),
              mock_to_ptr(ptr1), mock_to_ptr(ptr2));
    vec_push(Entity, &entities1, entity);
  }

  QueryIter iter = Query(MockComponent, MockComponent1);

  PComponent result[2];
  Entity *result_entity;
  for (usize i = 0; i < entities.len; i++) {
    result_entity = CComponent.query_next(&iter, array_ref(result));
    Entity *expect = vec_index(Entity, &entities, i);
    EXPECT_EQ(expect->id, result_entity->id);
  }
  for (usize i = 0; i < entities1.len; i++) {
    result_entity = CComponent.query_next(&iter, array_ref(result));
    Entity *expect = vec_index(Entity, &entities1, i);
    EXPECT_EQ(expect->id, result_entity->id);
  }
  CComponent.query_free(&iter);
  vec_free(Entity, &entities);
  vec_free(Entity, &entities1);
}