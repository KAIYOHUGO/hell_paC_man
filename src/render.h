#if !defined(RENDER_H)
#define RENDER_H

#include <evangel/array.h>
#include <evangel/resource.h>
#include <stdio.h>
#include <termios.h>

#define PIXEL_SIZE 41
#define NEWLINE_SIZE 1
#define EXTRA_BUFFER 16

struct termios init_terminal();

void reset_terminal(struct termios old_term);

typedef struct {
  u8 r, g, b;
} RGB;

typedef struct {
  u8 r, g, b, a;
} RGBA;

typedef struct {
  u8 height, width;
  Array(RGBA) pixel;
} Eva;

typedef struct {
  void (*render)(void *self);

  void (*free)(void *self);
} VRender;

typedef struct {
  const VRender *vtable;
  void *self;
} PRender;

struct PRenderLayer {
  isize layer;
  PRender p_render;
};

typedef struct {
  usize height, width;
  RGB clean_color;
  Vec(PRenderLayer) queue;
  Array(RGB) color_buffer;
} RenderBuffer;

ExportResourceType(RenderBuffer);

Eva *parse_eva(FILE *file);

Eva *open_eva(char *path);

void add_render_queue(isize layer, PRender p_render);

void render_init(usize lines, usize columns, RGB clean_color);

void render();

#endif // RENDER_H