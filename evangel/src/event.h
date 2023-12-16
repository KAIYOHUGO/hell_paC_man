#if !defined(__EVENT_H)
#define __EVENT_H

#include "basic.h"
#include "vec.h"

typedef struct {
  usize id;
} EventType;

typedef struct {
  void *self;
} PEvent;

struct InQueueEvent {
  EventType ty;
  PEvent event;
};

typedef void (*event_free)(void *);

struct EventQueue {
  Vec(Vec(struct PEvent)) type_event_map;
  Vec(InQueueEvent) queue;
};

struct CEvent {
  EventType (*add_new_type)();

  void (*emit)(EventType ty, mov(PEvent) event);

  void (*flush)();

  brw(Vec(PEvent) *) (*listen)(EventType ty);

  PEvent (*default_vtable)(void *event);
};

extern const struct CEvent CEvent;

#define ETy(T) EventType##T

#define event_emit(T, p_event) CEvent.emit(ETy(T), p_event)

#define event_listen(T) CEvent.listen(ETy(T))

#define DeclareEventType(T)                                                    \
  EventType ETy(T);                                                            \
  void add_event_type_##T() { ETy(T) = CEvent.add_new_type(); }

#define ExportEventType(T)                                                     \
  extern EventType ETy(T);                                                     \
  void add_event_type_##T();

#define add_event_type(T) add_event_type_##T()

void internal_event_queue_init();

#endif // __EVENT_H