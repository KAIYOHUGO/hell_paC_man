#if !defined(__RESOURCE_C)
#define __RESOURCE_C

#include "resource.h"
#include <stdlib.h>

static struct ResourceStorage ResourceStorage = {};

void internal_resource_storage_init() {
  struct ResourceStorage storage = {.list = vec_init(void *)};
  ResourceStorage = storage;
}

static ResourceType raw_add_new_type() {
  usize id = ResourceStorage.list.len;
  vec_push(void *, &ResourceStorage.list, NULL);
  ResourceType resource_id = {.id = id};
  return resource_id;
}

static void raw_insert(ResourceType ty, mov(void *) resource) {
  void **ptr = vec_index(void *, &ResourceStorage.list, ty.id);
  *ptr = resource;
}

static brw(void *) raw_get(ResourceType ty) {
  return *vec_index(void *, &ResourceStorage.list, ty.id);
}

struct CResource CResource = {
    .add_new_type = raw_add_new_type,
    .insert = raw_insert,
    .get = raw_get,
};

#endif // __RESOURCE_C