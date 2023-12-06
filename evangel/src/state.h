#if !defined(__STATE_H)
#define __STATE_H

#include "basic.h"
#include "map.h"
#include "vec.h"

typedef struct {
  usize id;
} StateType;

typedef enum {
  StateKind_In,
  StateKind_Enter,
  StateKind_Exit,
} StateKind;

struct StateInfo {
  StateKind kind;
  usize prev_state, state, next_state;
};

struct StateStorage {
  Vec(struct StateInfo) state_list;
};

struct CState {
  StateType (*add_new_type)();

  void (*set)(StateType ty, usize new_state);

  bool (*is_in)(StateType ty, usize state);

  bool (*is_enter)(StateType ty, usize state);

  bool (*is_exit)(StateType ty, usize state);

  void (*flush)();
};

extern const struct CState CState;

#define STy(T) StateType##T

#define state_set(T, new_state) CState.set(STy(T), (usize)new_state)

#define state_is_in(T, state) (CState.is_in(STy(T), (usize)state))

#define state_is_enter(T, state) (CState.is_enter(STy(T), (usize)state))

#define state_is_exit(T, state) (CState.is_exit(STy(T), (usize)state))

#define DeclareStateType(T)                                                    \
  StateType STy(T);                                                            \
  void add_state_type_##T() { STy(T) = CState.add_new_type(); }

#define ExportStateType(T)                                                     \
  extern StateType STy(T);                                                     \
  void add_state_type_##T();

#define add_state_type(T) add_state_type_##T()

void internal_state_storage_init(void);

#endif // __STATE_H