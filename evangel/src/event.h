#if !defined(__EVENT_H)
#define __EVENT_H

#include "basic.h"
#include "vec.h"

typedef struct {
  usize id;
} EventType;

struct InQueueEvent {
  EventType ty;
  void *event;
};

typedef void (*event_free)(void *);

struct TypedEvent {
  event_free free;
  void *event;
};

struct EventQueue {
  Vec(TypedEvent) typed_event_map;
  Vec(InQueueEvent) queue;
};

struct CEvent {
  EventType (*add_new_type)();

  void (*emit)(EventType typeid, void *event);
  
  void (*flush)();
  
  void *(*listen)(EventType typeid);
};

extern const struct CEvent CEvent;

#define AddEventType(T)                                                        \
  EventType EventType##T;                                                      \
  void add_event_type_##T() { EventType##T = CEvent.add_new_type(); }

#endif // __EVENT_H