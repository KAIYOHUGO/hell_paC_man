#if !defined(__EVENT_C)
#define __EVENT_C

#include "event.h"
#include <stdlib.h>

typedef struct InQueueEvent InQueueEvent;

struct EventQueue EventQueue = {};

void internal_event_queue_init(void) {
  struct EventQueue queue = {
      .type_event_map = vec_init(Vec(TypedEvent)),
      .queue = vec_init(InQueueEvent),
  };
  EventQueue = queue;
}

EventType raw_add_new_type() {
  usize id = EventQueue.type_event_map.len;
  vec_push(Vec(PEvent), &EventQueue.type_event_map, vec_init(PEvent));
  EventType typeid = {.id = id};
  return typeid;
}

void raw_emit(EventType ty, PEvent event) {
  InQueueEvent element = {
      .ty = ty,
      .event = event,
  };
  vec_push(InQueueEvent, &EventQueue.queue, element);
}

void raw_flush() {
  usize map_len = EventQueue.type_event_map.len;
  for (usize i = 0; i < map_len; i++) {
    Vec(PEvent) *p_events =
        vec_index(Vec(PEvent), &EventQueue.type_event_map, i);
    for (usize j = 0; j < p_events->len; j++) {
      PEvent *p_event = vec_index(PEvent, p_events, j);
      free(p_event->self);
    }
    vec_clear(PEvent, p_events);
  }

  usize queue_len = EventQueue.queue.len;
  for (usize i = 0; i < queue_len; i++) {
    InQueueEvent *in_queue_event =
        vec_index(InQueueEvent, &EventQueue.queue, i);
    Vec(PEvent) *p_events = vec_index(Vec(PEvent), &EventQueue.type_event_map,
                                      in_queue_event->ty.id);
    vec_push(PEvent, p_events, in_queue_event->event);
  }
  vec_clear(InQueueEvent, &EventQueue.queue);
}

brw(Vec(PEvent) *) raw_listen(EventType ty) {
  return vec_index(Vec(PEvent), &EventQueue.type_event_map, ty.id);
}

const struct CEvent CEvent = {
    .add_new_type = raw_add_new_type,

    .emit = raw_emit,

    .flush = raw_flush,

    .listen = raw_listen,
};

#endif // __EVENT_C