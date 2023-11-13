#include "basic.h"

enum GameState {
  Playing,
  Pause,
};

struct GameResource {
  usize freeze_cooldown;
};

struct GameStage {
  enum GameState state;
  struct GameResource resource;
};

struct BoosterTrait {
  void *self;
  void (*boost)(void *, struct GameStage *);
};