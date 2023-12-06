#include "component.h"
#include "global.h"
#include "render.h"
#include "setting.h"
#include <evangel/app.h>

DeclareComponentType(Position);

DeclareComponentType(ScreenCord);

DeclareComponentType(Sprite);

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

typedef struct {
  ResourceType img;
  isize x, y;
} SpritePRender;

void v_render_sprite(void *s) {
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

static const VRender SpriteVRender = {
    .render = v_render_sprite,
    .free = NULL,
};

void sprite_system() {
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
    PRender p_render = {
        .vtable = &SpriteVRender,
        .self = s,
    };
    add_render_queue(cord.z, p_render);
  }

  CComponent.query_free(&iter);
}

void position_to_screen_cord_system() {
  GameInfo *info = resource_get(GameInfo);
  QueryIter iter = Query(Position, ScreenCord);
  PComponent comp[2];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    Position *pos = (Position *)comp[0].self;
    ScreenCord *cord = (ScreenCord *)comp[1].self;
    cord->x = pos->x * 9 + info->offset_x;
    cord->y = pos->y * 9 + info->offset_y;
  }
  CComponent.query_free(&iter);
}

void number_system() {
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

void component_init() {
  add_component_type(Position);
  add_component_type(ScreenCord);
  add_component_type(Sprite);
  add_component_type(Number);

  AddRenderSystem(position_to_screen_cord_system, sprite_system, number_system);
}