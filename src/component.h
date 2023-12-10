#if !defined(COMPONENT_H)
#define COMPONENT_H

#include <evangel/basic.h>
#include <evangel/component.h>
#include <evangel/resource.h>

typedef struct {
  isize x, y;
} Position;

ExportComponentType(Position);

PComponent position_new(Position pos);

typedef struct {
  isize x, y, z;
} ScreenCord;

ExportComponentType(ScreenCord);

PComponent screen_cord_new(ScreenCord cord);

typedef struct {
  f64 ms_per_pixel;
  isize x, y;
  bool active, idle;
  f64 _elapse;
} AnimationCord;

ExportComponentType(AnimationCord);

PComponent animation_cord_new(AnimationCord cord);

typedef enum {
  Rotation_0,
  Rotation_90,
  Rotation_180,
  Rotation_270,
} Rotation;

typedef struct {
  ResourceType eva_img;
  Rotation rotation;
  bool active;
} Sprite;

ExportComponentType(Sprite);

PComponent sprite_new(Sprite sprite);

typedef struct {
  usize n, len;
} Number;

ExportComponentType(Number);

PComponent number_new(Number num);

typedef enum {
  LoopMode_Disable,
  LoopMode_LoopN,
  LoopMode_LoopInf,
  LoopMode_LoopNPingPong,
  LoopMode_LoopInfPingPong,
} LoopMode;

typedef enum {
  Direction_Forward,
  Direction_Reverse,
} Direction;

typedef struct {
  mov(Array(ResourceType)) eva_imgs;
  f64 ms_per_frame;
  Direction direction;
  LoopMode loop_mode;
  usize loop;
  bool active;

  f64 _elapse;
  usize _index;
} AnimationSprite;

ExportComponentType(AnimationSprite);

PComponent animation_sprite_new(AnimationSprite animation_sprite);

void component_init();

#endif // COMPONENT_H