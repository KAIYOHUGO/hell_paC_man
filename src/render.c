#if !defined(RENDER_C)
#define RENDER_C

#define PIXEL_SIZE 41
#define NEWLINE_SIZE 1
#define EXTRA_BUFFER 16

#include "render.h"
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

typedef struct PRenderLayer PRenderLayer;

DeclareResourceType(RenderBuffer);

struct termios init_terminal() {
  struct termios term, new_term;
  tcgetattr(STDIN_FILENO, &term);
  new_term = term;
  new_term.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
  // hide cursor
  printf("\033[?25l");
  return term;
}

void reset_terminal(struct termios old_term) {
  tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
  // clean & show cursor
  printf("\033c\033[?25h");
}

void render_pixel(RGB upper, RGB lower) {
  printf("\033[38;2;%u;%u;%um\033[48;2;%u;%u;%um\u2580", upper.r, upper.g,
         upper.b, lower.r, lower.g, lower.b);
}

void render_clear() {
  // clear
  printf("\033[H");
}

Eva *parse_eva(FILE *file) {
  u8 height = fgetc(file);
  u8 width = fgetc(file);
  Array(RGBA) pixel = array_init(RGBA, height * width);
  for (usize i = 0; i < pixel.len; i++) {
    RGBA color = {
        .r = fgetc(file),
        .g = fgetc(file),
        .b = fgetc(file),
        .a = fgetc(file),
    };
    *array_index(RGBA, &pixel, i) = color;
  }
  Eva *eva = malloc(sizeof(Eva));
  eva->height = height;
  eva->width = width;
  eva->pixel = pixel;
  return eva;
}

Eva *open_eva(char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL)
    return NULL;
  Eva *eva = parse_eva(file);
  fclose(file);
  return eva;
}

void add_render_queue(isize layer, PRender p_render) {
  RenderBuffer *buffer = resource_get(RenderBuffer);
  PRenderLayer p_render_layer = {
      .layer = layer,
      .p_render = p_render,
  };
  vec_push(PRenderLayer, &buffer->queue, p_render_layer);
}

isize layer_cmp(const void *a, const void *b) {
  return ((PRenderLayer *)a)->layer - ((PRenderLayer *)b)->layer;
}

void render() {
  RenderBuffer *buffer = resource_get(RenderBuffer);

  // clean
  for (usize i = 0; i < buffer->color_buffer.len; i++) {
    *array_index(RGB, &buffer->color_buffer, i) = buffer->clean_color;
  }

  qsort(buffer->queue.ptr, buffer->queue.len, buffer->queue.size, layer_cmp);
  for (usize i = 0; i < buffer->queue.len; i++) {
    PRender ptr = vec_index(PRenderLayer, &buffer->queue, i)->p_render;

    ptr.vtable->render(ptr.self);
    if (ptr.vtable->free != NULL)
      ptr.vtable->free(ptr.self);
    free(ptr.self);
  }
  vec_clear(PRenderLayer, &buffer->queue);

  render_clear();
  RGB *typed_buffer = array_typed(RGB, &buffer->color_buffer);
  for (usize y = 0; y < buffer->height; y += 2) {
    for (usize x = 0; x < buffer->width; x++) {
      render_pixel(*(typed_buffer + y * buffer->width + x),
                   *(typed_buffer + (y + 1) * buffer->width + x));
    }
    if (y != buffer->height - 2)
      putchar('\n');
  }
  fflush(stdout);
}

void render_init(usize lines, usize columns, RGB clean_color) {
  add_resource_type(RenderBuffer);
  RenderBuffer *buffer = malloc(sizeof(RenderBuffer));
  buffer->height = lines * 2;
  buffer->width = columns;
  buffer->clean_color = clean_color;
  buffer->color_buffer = array_init(RGB, buffer->height * buffer->width);
  buffer->queue = vec_init(PRenderLayer);
  resource_insert(RenderBuffer, buffer);
  // fix terminal tearing
  setvbuf(stdout, NULL, _IOFBF,
          (PIXEL_SIZE * columns + NEWLINE_SIZE) * lines + EXTRA_BUFFER);
}

#endif // RENDER_C