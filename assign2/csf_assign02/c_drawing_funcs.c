// C implementations of drawing functions (and helper functions)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "drawing_funcs.h"

////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////

static int32_t in_bounds(struct Image *img, int32_t x, int32_t y) {
  if (x < 0 || y < 0) return 0;
  if (img->width <= x || img->height <= y) return 0;
  return 1;
}

static uint32_t compute_index(struct Image *img, int32_t x, int32_t y) {
  return y * (img->width) + x;
}

static int32_t clamp(int32_t val, int32_t min, int32_t max) {
  if (val < min) val = min;
  else if (val > max) val = max;
  return val;
}

static uint8_t get_r(uint32_t color) {
  return color >> 24;
}

static uint8_t get_g(uint32_t color) {
  return color >> 16;
}

static uint8_t get_b(uint32_t color) {
  return color >> 8;
}

static uint8_t get_a(uint32_t color) {
  return color;
}

static uint8_t blend_components(uint32_t fg, uint32_t bg, uint32_t alpha) {
  return (alpha * fg + (255 - alpha) * bg) / 255;
}

static uint32_t blend_colors(uint32_t fg, uint32_t bg) {
  uint32_t color = blend_components(get_r(fg), get_r(bg), get_a(fg));
  color <<= 8;
  color += blend_components(get_b(fg), get_b(bg), get_a(fg));
  color <<= 8;
  color += blend_components(get_g(fg), get_g(bg), get_a(fg));
  color <<= 8;
  color += get_a(fg);
  return color;
}

static void set_pixel(struct Image *img, uint32_t index, uint32_t color) {
  img->data[index] = blend_colors(color, img->data[index]);
}

static int64_t square(int64_t x) {
  return x * x;
}

static int64_t square_dist(int64_t x1, int64_t y1, int64_t x2, int64_t y2) {
  return square(x2-x1) + square(y2-y1);
}

////////////////////////////////////////////////////////////////////////
// API functions
////////////////////////////////////////////////////////////////////////

//
// Draw a pixel.
//
// Parameters:
//   img   - pointer to struct Image
//   x     - x coordinate (pixel column)
//   y     - y coordinate (pixel row)
//   color - uint32_t color value
//
void draw_pixel(struct Image *img, int32_t x, int32_t y, uint32_t color) {
  if (!in_bounds(img, x, y)) {
    set_pixel(img, compute_index(img, x, y), color);
  }
}

//
// Draw a rectangle.
// The rectangle has rect->x,rect->y as its upper left corner,
// is rect->width pixels wide, and rect->height pixels high.
//
// Parameters:
//   img     - pointer to struct Image
//   rect    - pointer to struct Rect
//   color   - uint32_t color value
//
void draw_rect(struct Image *img,
               const struct Rect *rect,
               uint32_t color) {
  for (int32_t i = rect->y; i < img->height; i++) {
    for (int32_t j = rect->x; j < img->width; j++) {
      draw_pixel(img, j, i, color);
    }
  }
}

//
// Draw a circle.
// The circle has x,y as its center and has r as its radius.
//
// Parameters:
//   img     - pointer to struct Image
//   x       - x coordinate of circle's center
//   y       - y coordinate of circle's center
//   r       - radius of circle
//   color   - uint32_t color value
//
void draw_circle(struct Image *img,
               int32_t x, int32_t y, int32_t r,
               uint32_t color) {
  for (int32_t i = y-r; i <= y+r; i++) {
    for (int32_t j = x-r; j <= x+r; j++) {
      if (square_dist(j, i, x, y) <= r) {
        draw_pixel(img, j, i, color);
      }
    }
  }                
}
//
// Draw a tile by copying all pixels in the region
// enclosed by the tile parameter in the tilemap image
// to the specified x/y coordinates of the destination image.
// No blending of the tile pixel colors with the background
// colors should be done.
//
// Parameters:
//   img     - pointer to Image (dest image)
//   x       - x coordinate of location where tile should be copied
//   y       - y coordinate of location where tile should be copied
//   tilemap - pointer to Image (the tilemap)
//   tile    - pointer to Rect (the tile)
//
void draw_tile(struct Image *img,
               int32_t x, int32_t y,
               struct Image *tilemap,
               const struct Rect *tile) {
  if (x < 0 || y < 0 || x+tile->width >= img->width || y+tile->height >= img->height) {
    return;
  }
  for (int32_t i = 0; i < tile->height; i++) {
    for (int32_t j = 0; j < tile->width; j++) {
      uint32_t color = tilemap->data[compute_index(tilemap, j + tile->x, i + tile->y)];
      img->data[compute_index(img, j+x, i+y)] = color;
    }
  }
}

//
// Draw a sprite by copying all pixels in the region
// enclosed by the sprite parameter in the spritemap image
// to the specified x/y coordinates of the destination image.
// The alpha values of the sprite pixels should be used to
// blend the sprite pixel colors with the background
// pixel colors.
//
// Parameters:
//   img       - pointer to Image (dest image)
//   x         - x coordinate of location where sprite should be copied
//   y         - y coordinate of location where sprite should be copied
//   spritemap - pointer to Image (the spritemap)
//   sprite    - pointer to Rect (the sprite)
//
void draw_sprite(struct Image *img,
                 int32_t x, int32_t y,
                 struct Image *spritemap,
                 const struct Rect *sprite) {
  if (x < 0 || y < 0 || x + sprite->width >= img->width || y + sprite->height >= img->height) {
    return;
  }
  for (int32_t i = 0; i < sprite->height; i++) {
    for (int32_t j = 0; j < sprite->width; j++) {
      uint32_t color = spritemap->data[compute_index(spritemap, j + sprite->x, i + sprite->y)];
      draw_pixel(img, j+x, i+y, color);
    }
  }
}
