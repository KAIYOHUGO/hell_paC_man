#if !defined(COMPONENT_C)
#define COMPONENT_C

#include "component.h"
#include "render.h"
#include <evangel/app.h>

DeclareComponentType(Position);

DeclareComponentType(ScreenCord);

DeclareComponentType(Sprite);

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

PComponent sprite_new(Sprite sprite) {
  Sprite *ptr = malloc(sizeof(Sprite));
  *ptr = sprite;
  return CComponent.default_vtable(ptr);
}

typedef struct {
  Sprite sprite;
  isize x, y;
} SpritePRender;

void v_render_sprite(void *s) {
  SpritePRender *self = (SpritePRender *)s;
  Eva *img = (Eva *)CResource.get(self->sprite.eva_img);
  RenderBuffer *buffer = resource_get(RenderBuffer);
  // 1 < img->height;
  // for (isize y = 0; y < 9; y++) {
  // }
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
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_ref(comp))) != NULL) {
    ScreenCord cord = *(ScreenCord *)(comp[0].self);
    Sprite sprite = *(Sprite *)(comp[1].self);
    SpritePRender *s = malloc(sizeof(SpritePRender));
    s->sprite = sprite;
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

void component_init() {
  add_component_type(Position);
  add_component_type(ScreenCord);
  add_component_type(Sprite);

  AddRenderSystem(sprite_system);
}

#endif // COMPONENT_C