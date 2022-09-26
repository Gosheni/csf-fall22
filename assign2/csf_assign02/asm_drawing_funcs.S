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
	movl IMAGE_WIDTH_OFFSET(%rdi), %r10d // Store pointer value of image width to r10d 
	movl IMAGE_HEIGHT_OFFSET(%rdi), %r11d // Store pointer value of image height to r11d

	cmpl $0, %esi // Compare 0 and esi
	jl .Lnot_in_bounds // If esi is less than 0, jump to Lnot_in_bounds
	cmpl $0, %edx // Compare 0 and edx
	jl .Lnot_in_bounds // If edx is less than 0, jump to Lnot_in_bounds

	cmpl %r10d, %esi // Compare r10d and esi
	jge .Lnot_in_bounds // If esi is greater than or equal to r10d, jump to Lnot_in_bounds
	cmpl %r11d, %edx // Compare r11d and esi
	jge .Lnot_in_bounds // If edx is greater than or equal to r11d, jump to Lnot_in_bounds

	movl $1, %eax // Store the value 1 into eax to return it
	jmp .L_in_boundsend // Jump to the end

.Lnot_in_bounds:
	movl $0, %eax // Store the value 0 into eax to return it

.L_in_boundsend:
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
	movl IMAGE_WIDTH_OFFSET(%rdi), %r10d // Store pointer value of image width to r10d

	imull %r10d, %edx // Multiply r10d and edx and store it in edx
	addl %edx, %esi // Add edx and esi and store it in esi

	movl %esi, %eax // Move the result esi to eax to return it
	ret


/* If the given value is less than min or
 * greater than the given max, the value
 * is modified to fit that min and max range
 *
 * Parameters:
 *   %edi - int32_t value
 *   %esi - minimum int32_t value val shoulde be clamped to
 *   %edx - maximum int32_t value val shoulde be clamped to
 */
	.globl clamp
clamp:
	cmpl $edi, %esi // Compare edi and esi
	jl .Lclamp_to_min // If edi is less than esi, jump to Lclamp_to_min

	cmpl $edi, %edx // Compare edi and edx
	jg .Lclamp_to_max // If edx is less than 0, jump to Lclamp_to_max

	jmp .L_clampend // Jump to the end

.Lclamp_to_min:
	movl %esi, %edi // Store the value 0 into eax to return it
	jmp .L_clampend // Jump to the end

.Lclamp_to_max:
	movl %edx, %edi // Store the value 0 into eax to return it

.L_clampend:
	movl %edi, %eax // Store result edi value to eax to return it
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
 *   %rdi - int64_t value
 */
	.globl square
square:
	imulq %rdi, %rdi //square the value of the rdi register and store it in rdi 
	movq %rdi, %rax //move rdi to rax register to return it
	ret


/* Returns int64_t distance between 
 * two coordinates (x1,y1) and (x2, y2)
 *
 * Parameters:
 *   %rdi - int64_t value (x1)
 *   %rsi - int64_t value (y1)
 *   %rdx - int64_t value (x2)
 *   %rcx - int64_t value (y2)
 */
	.globl square_dist
square_dist:
	pushq %r12 // Push r12 register onto stack to store y1 value
	pushq %r13 // Push r13 register onto stack to store y2 value
	subq $8, %rsp // Subtract 8 positions, so that rsp is at an address that's multiple of 16 to call functions

	movq %rsi, %r12 // Move rsi register value to r12 register
	movq %rcx, %r13 // Move rcx register value to r13 register

	subq %rdi, %rdx // Subtract rdi from rdx and store the value in rdx
	movq %rdx, %rdi // Move rdx to rdi register, the first arg, to call the function on it
	call square // Call square function

	subq %r12, %r13 // Subtract r12 from r13 and store it in r13
	movq %rax, %r12 // Store the square value it r12
	movq %r13, %rdi // Pass the difference of r12, r13 to rdi to call the function
	call square // Call square function

	addq %r12, %rax // Add the two squares and store it in rax to return it
	
	addq $8, %rsp // Decrement rsp register by 8 to restore the value
	popq %r13; // Restore this callee-saved register
	popq %r12; // Restore

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
