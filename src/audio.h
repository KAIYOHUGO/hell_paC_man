#if !defined(AUDIO_H)
#define AUDIO_H

#include <miniaudio.h>
#include <evangel/resource.h>

ExportResourceType(ma_engine);

ma_sound *open_sound(const char *path);

void play_sound(ResourceType sound_ty);

void audio_init();

#endif // AUDIO_H