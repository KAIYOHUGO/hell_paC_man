#define MINIAUDIO_IMPLEMENTATION

#include "audio.h"

DeclareResourceType(ma_engine);

ma_sound *open_sound(const char *path) {
  ma_engine *engine = resource_get(ma_engine);
  ma_sound *sound = malloc(sizeof(ma_sound));
  ma_sound_init_from_file(engine, path, MA_SOUND_FLAG_DECODE, NULL, NULL,
                          sound);
  return sound;
}

void play_sound(ResourceType sound_ty) {
  ma_sound *sound = (ma_sound *)CResource.get(sound_ty);
  ma_sound_start(sound);
}

void audio_init() {
  add_resource_type(ma_engine);

  ma_engine *engine = malloc(sizeof(ma_engine));
  ma_result result = ma_engine_init(NULL, engine);
  assert(result == MA_SUCCESS);
  resource_insert(ma_engine, engine);
}
