#include "component.h"
#include "global.h"
#include "render.h"
#include <evangel/app.h>
#include <stdlib.h>

DeclareComponentType(Position);

DeclareComponentType(ScreenCord);

DeclareComponentType(AnimationCord);

DeclareComponentType(Sprite);

DeclareComponentType(AnimationSprite);

DeclareComponentType(Number);

PComponent position_new(Position pos) {
  Position *ptr = malloc(sizeof(Position));
  *ptr = pos;
  return CComponent.default_vtable(ptr);
}

PComponent screen_cord_new(ScreenCord cord) {
  ScreenCord *ptr = malloc(sizeof(ScreenCord));
  *ptr = cord;
  return CComponent.default_vtable(ptr);
}

PComponent animation_cord_new(AnimationCord cord) {
  AnimationCord *ptr = malloc(sizeof(AnimationCord));
  *ptr = cord;
  ptr->_elapse = 0.0;
  return CComponent.default_vtable(ptr);
}

PComponent number_new(Number num) {
  Number *ptr = malloc(sizeof(Number));
  *ptr = num;
  return CComponent.default_vtable(ptr);
}

PComponent sprite_new(Sprite sprite) {
  Sprite *ptr = malloc(sizeof(Sprite));
  *ptr = sprite;
  return CComponent.default_vtable(ptr);
}

static void despawn_animation_sprite(void *s, Entity _id) {
  AnimationSprite *self = (AnimationSprite *)s;
  array_free(ResourceType, &self->eva_imgs);
}

static const VComponent VAnimationSprite = {
    .despawn = despawn_animation_sprite,
};

PComponent animation_sprite_new(AnimationSprite animation_sprite) {
  AnimationSprite *ptr = malloc(sizeof(AnimationSprite));
  *ptr = animation_sprite;
  ptr->_elapse = 0.0;
  ptr->_index = 0;
  PComponent ret = {
      .vtable = &VAnimationSprite,
      .self = ptr,
  };
  return ret;
}

typedef struct {
  ResourceType img;
  isize x, y;
} SpritePRender;

static void v_render_sprite(void *s) {
  SpritePRender *self = (SpritePRender *)s;
  Eva *img = (Eva *)CResource.get(self->img);
  RenderBuffer *buffer = resource_get(RenderBuffer);

  for (isize y = max(-self->y, 0);
       y < min(img->height, (isize)buffer->height - self->y); y++) {
    for (isize x = max(-self->x, 0);
         x < min(img->width, (isize)buffer->width - self->x); x++) {
      RGBA image_pixel = *array_index(RGBA, &img->pixel, y * img->width + x);
      RGB *buffer_pixel =
          array_index(RGB, &buffer->color_buffer,
                      (self->y + y) * buffer->width + self->x + x);
      buffer_pixel->r = ((u16)image_pixel.r * (u16)image_pixel.a +
                         (u16)buffer_pixel->r * (255 - (u16)image_pixel.a)) /
                        255;
      buffer_pixel->g = ((u16)image_pixel.g * (u16)image_pixel.a +
                         (u16)buffer_pixel->g * (255 - (u16)image_pixel.a)) /
                        255;
      buffer_pixel->b = ((u16)image_pixel.b * (u16)image_pixel.a +
                         (u16)buffer_pixel->b * (255 - (u16)image_pixel.a)) /
                        255;
    }
  }
}

static void v_render_sprite_rotate_180(void *s) {
  SpritePRender *self = (SpritePRender *)s;
  Eva *img = (Eva *)CResource.get(self->img);
  RenderBuffer *buffer = resource_get(RenderBuffer);

  for (isize y = max(-self->y, 0);
       y < min(img->height, (isize)buffer->height - self->y); y++) {
    for (isize x = max(-self->x, 0);
         x < min(img->width, (isize)buffer->width - self->x); x++) {
      RGBA image_pixel =
          *array_index(RGBA, &img->pixel,
                       (img->height - y - 1) * img->width + img->width - x - 1);
      RGB *buffer_pixel =
          array_index(RGB, &buffer->color_buffer,
                      (self->y + y) * buffer->width + self->x + x);
      buffer_pixel->r = ((u16)image_pixel.r * (u16)image_pixel.a +
                         (u16)buffer_pixel->r * (255 - (u16)image_pixel.a)) /
                        255;
      buffer_pixel->g = ((u16)image_pixel.g * (u16)image_pixel.a +
                         (u16)buffer_pixel->g * (255 - (u16)image_pixel.a)) /
                        255;
      buffer_pixel->b = ((u16)image_pixel.b * (u16)image_pixel.a +
                         (u16)buffer_pixel->b * (255 - (u16)image_pixel.a)) /
                        255;
    }
  }
}

static void v_render_sprite_rotate_90(void *s) {
  SpritePRender *self = (SpritePRender *)s;
  Eva *img = (Eva *)CResource.get(self->img);
  RenderBuffer *buffer = resource_get(RenderBuffer);

  for (isize y = max(-self->y, 0);
       y < min(img->width, (isize)buffer->height - self->y); y++) {
    for (isize x = max(-self->x, 0);
         x < min(img->height, (isize)buffer->width - self->x); x++) {
      RGBA image_pixel =
          *array_index(RGBA, &img->pixel, x * img->width + img->width - y - 1);
      RGB *buffer_pixel =
          array_index(RGB, &buffer->color_buffer,
                      (self->y + y) * buffer->width + self->x + x);
      buffer_pixel->r = ((u16)image_pixel.r * (u16)image_pixel.a +
                         (u16)buffer_pixel->r * (255 - (u16)image_pixel.a)) /
                        255;
      buffer_pixel->g = ((u16)image_pixel.g * (u16)image_pixel.a +
                         (u16)buffer_pixel->g * (255 - (u16)image_pixel.a)) /
                        255;
      buffer_pixel->b = ((u16)image_pixel.b * (u16)image_pixel.a +
                         (u16)buffer_pixel->b * (255 - (u16)image_pixel.a)) /
                        255;
    }
  }
}

static void v_render_sprite_rotate_270(void *s) {
  SpritePRender *self = (SpritePRender *)s;
  Eva *img = (Eva *)CResource.get(self->img);
  RenderBuffer *buffer = resource_get(RenderBuffer);

  for (isize y = max(-self->y, 0);
       y < min(img->width, (isize)buffer->height - self->y); y++) {
    for (isize x = max(-self->x, 0);
         x < min(img->height, (isize)buffer->width - self->x); x++) {
      RGBA image_pixel = *array_index(RGBA, &img->pixel,
                                      (img->height - x - 1) * img->width + y);
      RGB *buffer_pixel =
          array_index(RGB, &buffer->color_buffer,
                      (self->y + y) * buffer->width + self->x + x);
      buffer_pixel->r = ((u16)image_pixel.r * (u16)image_pixel.a +
                         (u16)buffer_pixel->r * (255 - (u16)image_pixel.a)) /
                        255;
      buffer_pixel->g = ((u16)image_pixel.g * (u16)image_pixel.a +
                         (u16)buffer_pixel->g * (255 - (u16)image_pixel.a)) /
                        255;
      buffer_pixel->b = ((u16)image_pixel.b * (u16)image_pixel.a +
                         (u16)buffer_pixel->b * (255 - (u16)image_pixel.a)) /
                        255;
    }
  }
}

static const VRender SpriteVRender = {
    .render = v_render_sprite,
    .free = NULL,
};

static const VRender SpriteVRenderRotate90 = {
    .render = v_render_sprite_rotate_90,
    .free = NULL,
};

static const VRender SpriteVRenderRotate180 = {
    .render = v_render_sprite_rotate_180,
    .free = NULL,
};

static const VRender SpriteVRenderRotate270 = {
    .render = v_render_sprite_rotate_270,
    .free = NULL,
};

static void sprite_system() {
  QueryIter iter = Query(ScreenCord, Sprite);
  PComponent comp[2];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    ScreenCord cord = *(ScreenCord *)comp[0].self;
    Sprite sprite = *(Sprite *)comp[1].self;
    if (!sprite.active)
      continue;

    SpritePRender *s = malloc(sizeof(SpritePRender));
    s->img = sprite.eva_img;
    s->x = cord.x;
    s->y = cord.y;
    const VRender *vtable;
    switch (sprite.rotation) {
    case Rotation_0:
      vtable = &SpriteVRender;
      break;
    case Rotation_90:
      vtable = &SpriteVRenderRotate90;
      break;
    case Rotation_180:
      vtable = &SpriteVRenderRotate180;
      break;
    case Rotation_270:
      vtable = &SpriteVRenderRotate270;
      break;
    }
    PRender p_render = {
        .vtable = vtable,
        .self = s,
    };
    add_render_queue(cord.z, p_render);
  }

  CComponent.query_free(&iter);
}

static void position_to_screen_cord_system() {
  GameInfo *info = resource_get(GameInfo);
  QueryIter iter = QueryWithout(Without(AnimationCord), Position, ScreenCord);
  PComponent comp[2];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    Position *pos = (Position *)comp[0].self;
    ScreenCord *cord = (ScreenCord *)comp[1].self;
    cord->x = pos->x * PLAYER_SIZE + info->offset_x;
    cord->y = pos->y * PLAYER_SIZE + info->offset_y;
  }
  CComponent.query_free(&iter);
}

static void number_system() {
  ResourceType num_types[10] = {RTy(Num0), RTy(Num1), RTy(Num2), RTy(Num3),
                                RTy(Num4), RTy(Num5), RTy(Num6), RTy(Num7),
                                RTy(Num8), RTy(Num9)};
  QueryIter iter = Query(ScreenCord, Number);
  PComponent comp[2];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    ScreenCord cord = *(ScreenCord *)comp[0].self;
    Number num = *(Number *)comp[1].self;
    usize n = num.n;
    for (isize i = num.len - 1; i >= 0; i--) {
      usize offset_x = NUMBER_SIZE * i;
      SpritePRender *s = malloc(sizeof(SpritePRender));
      s->img = num_types[n % 10];
      s->x = cord.x + offset_x;
      s->y = cord.y;
      PRender p_render = {
          .vtable = &SpriteVRender,
          .self = s,
      };
      add_render_queue(cord.z, p_render);
      n /= 10;
    }
  }
  CComponent.query_free(&iter);
}

static isize get_sign(isize x) { return (x > 0) - (x < 0); }

static void animation_cord_system() {
  QueryIter iter = Query(AnimationCord, ScreenCord);
  PComponent comp[2];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    AnimationCord *animation_cord = (AnimationCord *)comp[0].self;
    ScreenCord *cord = (ScreenCord *)comp[1].self;

    if (!animation_cord->active)
      continue;

    isize d_x = get_sign(animation_cord->x - cord->x),
          d_y = get_sign(animation_cord->y - cord->y);
    if (d_x == 0 && d_y == 0) {
      animation_cord->idle = true;
      animation_cord->_elapse = 0.0;
      continue;
    }
    animation_cord->idle = false;

    animation_cord->_elapse += CApp.get_delta();
    if (animation_cord->_elapse < animation_cord->ms_per_pixel)
      continue;

    usize pixel =
        (usize)(animation_cord->_elapse / animation_cord->ms_per_pixel);
    animation_cord->_elapse -= animation_cord->ms_per_pixel * (f64)pixel;

    cord->x += d_x * pixel;
    cord->y += d_y * pixel;
  }
  CComponent.query_free(&iter);
}

static void position_to_animation_cord_system() {
  GameInfo *info = resource_get(GameInfo);
  QueryIter iter = Query(Position, AnimationCord, ScreenCord);
  PComponent comp[3];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    Position *pos = (Position *)comp[0].self;
    AnimationCord *animation_cord = (AnimationCord *)comp[1].self;
    ScreenCord *cord = (ScreenCord *)comp[2].self;
    animation_cord->x = pos->x * PLAYER_SIZE + info->offset_x;
    animation_cord->y = pos->y * PLAYER_SIZE + info->offset_y;
    if (!animation_cord->active) {
      cord->x = animation_cord->x;
      cord->y = animation_cord->y;
    }
  }
  CComponent.query_free(&iter);
}

static void animation_sprite_system() {
  QueryIter iter = Query(AnimationSprite, Sprite);
  PComponent comp[2];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    AnimationSprite *animation_sprite = (AnimationSprite *)comp[0].self;
    Sprite *sprite = (Sprite *)comp[1].self;
    if (!animation_sprite->active)
      continue;

    animation_sprite->_elapse += CApp.get_delta();
    if (animation_sprite->_elapse < animation_sprite->ms_per_frame)
      continue;

    usize frame_n =
        (usize)(animation_sprite->_elapse / animation_sprite->ms_per_frame);
    animation_sprite->_elapse -= animation_sprite->ms_per_frame * (f64)frame_n;
    animation_sprite->_index += frame_n;

    usize loop_index =
        animation_sprite->_index / animation_sprite->eva_imgs.len;
    usize frame_index =
        animation_sprite->_index % animation_sprite->eva_imgs.len;

    switch (animation_sprite->loop_mode) {
    case LoopMode_Disable:
      if (loop_index == 0)
        break;
      animation_sprite->active = false;
    case LoopMode_LoopN:
      if (loop_index <= animation_sprite->loop)
        break;
      animation_sprite->active = false;
    case LoopMode_LoopInf:
      break;
    case LoopMode_LoopNPingPong:
      if (loop_index > animation_sprite->loop) {
        animation_sprite->_index = 0;
        animation_sprite->_elapse = 0.0;
        animation_sprite->active = false;
        continue;
      }
      if ((loop_index & 1) != 0)
        frame_index = animation_sprite->eva_imgs.len - frame_index - 1;
      break;
    case LoopMode_LoopInfPingPong:
      if ((loop_index & 1) != 0)
        frame_index = animation_sprite->eva_imgs.len - frame_index - 1;
      break;
    }

    if (!animation_sprite->active) {
      animation_sprite->_index = 0;
      animation_sprite->_elapse = 0.0;
      continue;
    }

    switch (animation_sprite->direction) {
    case Direction_Forward:
      break;
    case Direction_Reverse:
      frame_index = animation_sprite->eva_imgs.len - frame_index - 1;
      break;
    }

    sprite->eva_img =
        *array_index(ResourceType, &animation_sprite->eva_imgs, frame_index);
  }
  CComponent.query_free(&iter);
}

void component_init() {
  add_component_type(Position);
  add_component_type(ScreenCord);
  add_component_type(AnimationCord);
  add_component_type(Sprite);
  add_component_type(AnimationSprite);
  add_component_type(Number);

  AddRenderSystem(position_to_screen_cord_system,
                  position_to_animation_cord_system, animation_sprite_system,
                  animation_cord_system);
  AddRenderSystem(number_system, sprite_system);
}