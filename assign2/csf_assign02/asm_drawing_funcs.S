/*
 * Assembly-language implementations of the drawing functions
 */

/* Offsets of struct Image fields */
#define IMAGE_WIDTH_OFFSET   0
#define IMAGE_HEIGHT_OFFSET  4
#define IMAGE_DATA_OFFSET    8

/* Offsets of struct Rect fields */
#define RECT_X_OFFSET        0
#define RECT_Y_OFFSET        4
#define RECT_WIDTH_OFFSET    8
#define RECT_HEIGHT_OFFSET   12

	.section .text

/***********************************************************************
   Helper functions
   You should declare these as .globl so that
   they can be called from the unit test code.
 ***********************************************************************/

/* TODO: implement your helper functions here */

/* Tests whether the given x and y coordinates
 * of the destination image is within the 
 * boundaries of the destination image.
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where tile should be copied
 *   %edx - y coordinate of location where tile should be copied
 */
	.globl in_bounds
in_bounds:
	ret


/* Returns the index of the destination image
 * given its x and y coordinates
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where tile should be copied
 *   %edx - y coordinate of location where tile should be copied
 */
	.globl compute_index
compute_index:
	ret


/* If the given value is less than min or
 * greater than the given max, the value
 * is modified to fit that min and max range
 *
 * Parameters:
 *   %edi - int32_t value
 *   %esi - x coordinate of location where tile should be copied
 *   %edx - y coordinate of location where tile should be copied
 */
	.globl clamp
clamp:
	ret


/* Returns uint8_t value of the red pixel
 *
 * Parameters:
 *   %edi - uint32_t color value
 */
	.globl get_r
get_r:
	ret


/* Returns uint8_t value of the green pixel
 *
 * Parameters:
 *   %edi - uint32_t color value
 */
	.globl get_g
get_g:
	ret


/* Returns uint8_t value of the blue pixel
 *
 * Parameters:
 *   %edi - uint32_t color value
 */
	.globl get_b
get_b:
	ret


/* Returns uint8_t value of alpha (opacity)
 *
 * Parameters:
 *   %edi - uint32_t color value
 */
	.globl get_a
get_a:
	ret


/* Blends the foreground color with the given
 * background color with the alpha value at 8 bit scale 
 * 
 * Parameters:
 *   %edi - uint32_t color value of the foreground
 *   %esi - uint32_t color value of the background
 *   %edx - uint32_t alpha value of the opacity
 */
	.globl blend_components
blend_components:
	ret


/* Blends the foreground color 
 * with the given background color 
 * 
 * Parameters:
 *   %edi - uint32_t color value of the foreground
 *   %esi - uint32_t color value of the background
 */
	.globl blend_colors
blend_colors:
	ret


/* Sets the pixel of the destination image
 * at given index with the given color 
 * 
 * Parameters:
 *   %rdi - pointer to struct Image
 *   %esi - uint32_t value of index of the image
 *   %edx - uint32_t color value
 */
	.globl set_pixel
set_pixel:
	ret


/* Returns int64_t square value of x
 *
 * Parameters:
 *   %rdi - int64_t value (x1)
 *   %rsi - int64_t value (y1)
 *   %rdx - int64_t value (x2)
 *   %rcx - int64_t value (y2)
 */
	.globl square
square:
	ret


/* Returns int64_t distance between 
 * two coordinates (x1,y1) and (x2, y2)
 *
 * Parameters:
 *   %rdi - int64_t value
 */
	.globl square_dist
square_dist:
	ret

/***********************************************************************
   Public API functions
 ***********************************************************************/

/*
 * Draw a pixel.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %esi     - x coordinate (pixel column)
 *   %edx     - y coordinate (pixel row)
 *   %ecx     - uint32_t color value
 */
	.globl draw_pixel
draw_pixel:
	/* TODO: implement */
	ret


/*
 * Draw a rectangle.
 * The rectangle has rect->x,rect->y as its upper left corner,
 * is rect->width pixels wide, and rect->height pixels high.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %rsi     - pointer to struct Rect
 *   %edx     - uint32_t color value
 */
	.globl draw_rect
draw_rect:
	/* TODO: implement */
	ret

/*
 * Draw a circle.
 * The circle has x,y as its center and has r as its radius.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %esi     - x coordinate of circle's center
 *   %edx     - y coordinate of circle's center
 *   %ecx     - radius of circle
 *   %r8d     - uint32_t color value
 */
	.globl draw_circle
draw_circle:
	/* TODO: implement */
	ret

/*
 * Draw a tile by copying all pixels in the region
 * enclosed by the tile parameter in the tilemap image
 * to the specified x/y coordinates of the destination image.
 * No blending of the tile pixel colors with the background
 * colors should be done.
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where tile should be copied
 *   %edx - y coordinate of location where tile should be copied
 *   %rcx - pointer to Image (the tilemap)
 *   %r8  - pointer to Rect (the tile)
 */
	.globl draw_tile
draw_tile:
	/* TODO: implement */
	ret

/*
 * Draw a sprite by copying all pixels in the region
 * enclosed by the sprite parameter in the spritemap image
 * to the specified x/y coordinates of the destination image.
 * The alpha values of the sprite pixels should be used to
 * blend the sprite pixel colors with the background
 * pixel colors.
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where sprite should be copied
 *   %edx - y coordinate of location where sprite should be copied
 *   %rcx - pointer to Image (the spritemap)
 *   %r8  - pointer to Rect (the sprite)
 */
	.globl draw_sprite
draw_sprite:
	/* TODO: implement */
	ret

/*
vim:ft=gas:
*/
