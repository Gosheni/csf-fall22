// C implementations of drawing functions (and helper functions)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "drawing_funcs.h"

////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////

// Tests whether the given x and y coordinates
// of the destination image is within the 
// boundaries of the destination image.
//
// Parameters:
//   img     - pointer to Image (dest image)
//   x       - x coordinate of location where tile should be copied
//   y       - y coordinate of location where tile should be copied
//
int32_t in_bounds(struct Image *img, int32_t x, int32_t y) {
  if (x < 0 || y < 0 || x >= img->width || y >= img->height) {
    return 0;
  }
  return 1;
}

// Returns the index of the destination image
// given its x and y coordinates
//
// Parameters:
//   img     - pointer to Image (dest image)
//   x       - x coordinate of location where tile should be copied
//   y       - y coordinate of location where tile should be copied
//
uint32_t compute_index(struct Image *img, int32_t x, int32_t y) {
  return y * (img->width) + x;
}

// If the given value is less than min or
// greater than the given max, the value
// is modified to fit that min and max range
//
// Parameters:
//   val     - int32_t value
//   min     - minimum int32_t value val shoulde be clamped to
//   max     - maximum int32_t value val shoulde be clamped to
//
int32_t clamp(int32_t val, int32_t min, int32_t max) {
  if (val < min) {
    val = min;
  } else if (val > max) {
    val = max;
  }
  return val;
}

// Returns uint8_t value of the red pixel
//
// Parameters:
//   color - uint32_t color value
//
uint8_t get_r(uint32_t color) {
  return color >> 24;
}

// Returns uint8_t value of the green pixel
//
// Parameters:
//   color - uint32_t color value
//
uint8_t get_g(uint32_t color) {
  return color >> 16;
}

// Returns uint8_t value of the blue pixel
//
// Parameters:
//   color - uint32_t color value
//
uint8_t get_b(uint32_t color) {
  return color >> 8;
}

// Returns uint8_t value of alpha (opacity)
//
// Parameters:
//   color - uint32_t color value
//
uint8_t get_a(uint32_t color) {
  return color;
}

// Blends the foreground color with the given
// background color with the alpha value at 8 bit scale 
// 
// Parameters:
//   fg    - uint32_t color value of the foreground
//   bg    - uint32_t color value of the background
//   alpha - uint32_t alpha value of the opacity
//
uint8_t blend_components(uint32_t fg, uint32_t bg, uint32_t alpha) {
  return (alpha * fg + (255 - alpha) * bg) / 255;
}

// Blends the foreground color 
// with the given background color 
// 
// Parameters:
//   fg - uint32_t color value of the foreground
//   bg - uint32_t color value of the background
//
uint32_t blend_colors(uint32_t fg, uint32_t bg) {
  uint32_t color = blend_components(get_r(fg), get_r(bg), get_a(fg));
  color <<= 8;
  color |= blend_components(get_g(fg), get_g(bg), get_a(fg));
  color <<= 8;
  color |= blend_components(get_b(fg), get_b(bg), get_a(fg));
  color <<= 8;
  color |= 255; //Set opacity to max
  return color;
}

// Sets the pixel of the destination image
// at given index with the given color 
// 
// Parameters:
//   img   - pointer to struct Image
//   index - uint32_t value of index of the image
//   color - uint32_t color value
//
void set_pixel(struct Image *img, uint32_t index, uint32_t color) {
  img->data[index] = blend_colors(color, img->data[index]);
}

// Returns int64_t square value of x
//
// Parameters:
//   x - int64_t value
//
int64_t square(int64_t x) {
  return x * x;
}

// Returns int64_t distance between 
// two coordinates (x1,y1) and (x2, y2)
//
// Parameters:
//   x - int64_t value
//
int64_t square_dist(int64_t x1, int64_t y1, int64_t x2, int64_t y2) {
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
  if (in_bounds(img, x, y)) {
    uint32_t ind = compute_index(img, x, y);
    set_pixel(img, ind, color);
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
  for (int32_t i = rect->y; i < rect->y + rect->height; i++) {
    for (int32_t j = rect->x; j < rect->x + rect->width; j++) {
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
      if (square_dist(j, i, x, y) <= square(r)) {
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
  if (tile->x < 0 || tile->y < 0 || 
  tile->x + tile->width > tilemap->width || tile->y + tile->height > tilemap->height) {
    return;
  }
  int32_t a = clamp(tile->width, 0, img->width-x);
  int32_t b = clamp(tile->height, 0, img->height-y);
  for (int32_t i = 0; i < b; i++) {
    for (int32_t j = 0; j < a; j++) {
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
  if (sprite->x < 0 || sprite->y < 0 ||
  sprite->x + sprite->width > spritemap->width || sprite->y + sprite->height > spritemap->height) {
    return;
  }
  int32_t a = clamp(sprite->width, 0, img->width-x);
  int32_t b = clamp(sprite->height, 0, img->height-y);
  for (int32_t i = 0; i < b; i++) {
    for (int32_t j = 0; j < a; j++) {
      uint32_t color = spritemap->data[compute_index(spritemap, j + sprite->x, i + sprite->y)];
      draw_pixel(img, j+x, i+y, color);
    }
  }
}