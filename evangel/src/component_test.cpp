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
  // MockComponent2 *ptr2 = new MockComponent2;
  // MockComponent3 *ptr3 = new MockComponent3;
  // MockComponent4 *ptr4 = new MockComponent4;
  Vec(Entity) entities = vec_init(Entity);
  for (usize i = 0; i < 100; i++) {
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
  for (usize i = 0; i < 50; i++) {
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
}
