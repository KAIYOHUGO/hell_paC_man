#if !defined(__RESOURCE_H)
#define __RESOURCE_H

#include "basic.h"
#include "vec.h"

typedef struct {
  usize id;
} ResourceType;

struct ResourceStorage {
  Vec(void *) list;
};

struct CResource {
  ResourceType (*add_new_type)();

  void (*insert)(ResourceType ty, mov(void *) resource);

  brw(void *) (*get)(ResourceType ty);
};

extern struct CResource CResource;

#define RTy(T) ResourceType##T

#define resource_insert(T, resource) CResource.insert(RTy(T), resource)

#define resource_get(T) ((T *)CResource.get(RTy(T)))

#define DeclareResourceType(T)                                                 \
  ResourceType RTy(T);                                                         \
  void add_resource_type_##T() { RTy(T) = CResource.add_new_type(); }

#define ExportResourceType(T)                                                  \
  extern ResourceType RTy(T);                                                  \
  void add_resource_type_##T();

#define add_resource_type(T) add_resource_type_##T()

void internal_resource_storage_init();

#endif // __RESOURCE_H