#if !defined(__EVENT_C)
#define __EVENT_C

#include "event.h"
#include "app.h"
#include <stdlib.h>

typedef struct InQueueEvent InQueueEvent;
typedef struct TypedEvent TypedEvent;

EventType raw_add_new_type() {
  vec_push(Vec(void *), &App.event_queue.typed_event_map, vec_init(void *));
  EventType typeid = {App.event_queue.typed_event_map.len - 1};
  return typeid;
}

void raw_emit(EventType typeid, void *event) {
  InQueueEvent element = {
      .ty = typeid,
      .event = event,
  };
  vec_push(InQueueEvent, &App.event_queue.queue, element);
}

void raw_flush() {
  for (usize i = 0; i < App.event_queue.typed_event_map.len; i++) {
    TypedEvent *ptr =
        vec_index(TypedEvent, &App.event_queue.typed_event_map, i);
    ptr->free(ptr->event);
    ptr->event = NULL;
  }

  usize queue_len = App.event_queue.queue.len;
  for (usize i = 0, offset = 0; i < queue_len; i++) {
    InQueueEvent *in_queue_event =
        vec_index(InQueueEvent, &App.event_queue.queue, offset);
    TypedEvent *typed_event = vec_index(
        TypedEvent, &App.event_queue.typed_event_map, in_queue_event->ty.id);
    if (typed_event->event != NULL) {
      offset++;
      continue;
    }
    typed_event->event = in_queue_event->event;
    vec_swap_remove(InQueueEvent, &App.event_queue.queue, offset);
  }
}

void *raw_listen(EventType typeid) {
  return vec_index(TypedEvent, &App.event_queue.typed_event_map, typeid.id)
      ->event;
}

const struct CEvent CEvent = {
    .add_new_type = raw_add_new_type,

    .emit = raw_emit,

    .flush = raw_flush,

    .listen = raw_listen,
};

#endif // __EVENT_C