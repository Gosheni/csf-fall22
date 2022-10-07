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
  	cmpl %esi, %edi // Compare edi and esi
	jl .Lclamp_to_min // If edi is less than esi, jump to Lclamp_to_min

	cmpl %edx, %edi // Compare edi and edx
	jg .Lclamp_to_max // If edx is greater than edx, jump to Lclamp_to_max

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
	shrl $24, %edi // Shift the edi value by 24 bits to the right
	movl %edi, %eax // Store the edi value to the return register eax
	andl $0xFF, %eax // Return the 8 bit value of eax
	ret


/* Returns uint8_t value of the green pixel
 *
 * Parameters:
 *   %edi - uint32_t color value
 */
	.globl get_g
get_g:
	shrl $16, %edi // Shift the edi value by 16 bits to the right
	movl %edi, %eax // Store the edi value to the return register eax
	andl $0xFF, %eax // Return the 8 bit value of eax
	ret


/* Returns uint8_t value of the blue pixel
 *
 * Parameters:
 *   %edi - uint32_t color value
 */
	.globl get_b
get_b:
	shrl $8, %edi // Shift the edi value by 8 bits to the right
	movl %edi, %eax // Store the edi value to the return register eax
	andl $0xFF, %eax // Return the 8 bit value of eax
	ret


/* Returns uint8_t value of alpha (opacity)
 *
 * Parameters:
 *   %edi - uint32_t color value
 */
	.globl get_a
get_a:
	movl %edi, %eax // Store the edi value to the return register eax
	andl $0xFF, %eax // Return the 8 bit value of eax
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
    movl $255, %r10d // Store the 255 value in r10d register

	subl %edx, %r10d // Subtract alpha value from 255 and store the val in r10d
	imull %esi, %r10d // Multiply bg color val with r10d and store it in r10d
	imull %edx, %edi // Multiply alpha and fg color value and store it in edi register
	addl %edi, %r10d // Add the edi value and r10d value together and store it in r10d

	movl %r10d, %eax // Store the result r10d to the return register eax
	movl $0, %edx // Set edx register to 0 to get ready divide
	movl $255, %r10d // Store the 255 value in r10d again
	div %r10d // Divide eax register by r10d

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
	pushq %rbx // Push the callee saved register onto stack
 	pushq %r12 // Push r12 register onto stack
	pushq %r13 // Push r13 register onto stack
	pushq %r14 // Push r14 register onto stack
	pushq %r15 // Push r15 register onto stack
	movl %edi, %r12d // Store the parameter register to r12d
	movl %esi, %r13d // Store esi register to r13d

	call get_a // Call get_a function with edi and esi registers
	movl %eax, %r14d // Store the result onto r14d
	movl %r14d, %edx // Move r14d to edx register to be used blend_componenets is called

	movl %r12d, %edi // Store r12d to edi register
	call get_r // Call get_r funciton with edi register
	movl %eax, %r15d // Store the result onto r15d

	movl %r13d, %edi // Move r13d value to edi register
	call get_r // Call get_r funciton with edi register
	movl %eax, %esi // Store the result onto esi

	movl %r14d, %edx // Move r14d to edx register again
	movl %r15d, %edi // Move 15d register to edi to get ready to call blend_components
	call blend_components // Call blend_components with edi, esi, edx registers
	movl %eax, %ebx // Store the result onto ebx register (color)

	shll $8, %ebx // Shift ebx register (color) by 8 shifts to the left

	movl %r12d, %edi // Move r12d to edi register
	call get_g // Call get_g funciton with edi register
	movl %eax, %r15d // Store the result in r15d register

	movl %r13d, %edi // Move r13d to edi register
	call get_g // Call get_g funciton with edi register
	movl %eax, %esi // Store the result in esi (second argument)

	movl %r14d, %edx // Move r14d to edx register again
	movl %r15d, %edi // Move 15d register to edi to get ready to call blend_components
	call blend_components // Call blend_components with edi, esi, edx registers

	orl %eax, %ebx // Add the green color result to the shifted ebx value
	shll $8, %ebx // Shift ebx register (color) by 8 shifts to the left

	movl %r12d, %edi // Move r12d to edi register
	call get_b // Call get_b funciton with edi register
	movl %eax, %r15d // Store the result in r15d register

	movl %r13d, %edi // Move r13d to edi register
	call get_b // Call get_b funciton with edi register
	movl %eax, %esi // Store the result in esi (second argument)

	movl %r14d, %edx // Move r14d to edx register again
	movl %r15d, %edi // Move 15d register to edi to get ready to call blend_components
	call blend_components // Call blend_components with edi, esi, edx registers

	orl %eax, %ebx // Add the blue color result to the shifted ebx value
	shll $8, %ebx // Shift ebx register (color) by 8 shifts to the left
	orl $255, %ebx // Set opacitiy to full by adding 255 to the last 8 bits
	movl %ebx, %eax // Store the result to return register eax

	popq %r15 // Restore r15 callee saved register
	popq %r14 // Restore r14 callee saved register
	popq %r13 // Restore r13 callee saved register
	popq %r12 // Restore r12 callee saved register
	popq %rbx // Restore rbx callee saved register
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
	pushq %rbx // Push rbx register onto stack
	pushq %r12 // Push r12 callee saved register onto stack
	subq $8, %rsp // Subtract 8 bytes from base stack pointer register

	movl %esi, %r12d // Move the index value to r12d register
	movq IMAGE_DATA_OFFSET(%rdi), %rbx // Move the image->data to rbx register

	movl (%rbx, %r12, 4), %esi // Move the image->data[index] value to esi register (second arg)
	movl %edx, %edi // Move the edx reigster to edi register (first arg)
	call blend_colors // Call blend_colors function with edi, esi registers

	movl %eax, (%rbx, %r12, 4) // Move the result to the image->data[index] 

	addq $8, %rsp // Restore the position of rsp register
	popq %r12 // Restore the r12 register
	popq %rbx // Restore the rbx register
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
	pushq %rbx // Push callee saved rbx register onto stack
	pushq %r12 // Push r12 register onto stack
	pushq %r13 // Push r13 register onto stack

	movq %rdi, %rbx // Save the rdi value onto callee saved rbx register (so that it doesn't get modified when external func is called)
	movl %esi, %r12d // Move esi value onto r12d register
	movl %edx, %r13d // Move edx value onto r13d register

	call in_bounds // Call in_bounds function with rdi, esi, edx registers
	cmpl $1, %eax
	jl .Lpixel_end // If eax value is less than 1 (not within bounds), then jump to the end (Don't do anything)

	movq %rbx, %rdi // Move rbx back to rdi register
	movl %r12d, %esi // Move r12d back to esi register
	movl %r13d, %edx // Move r13d back to edx register to get ready to call function
	call compute_index // Call compute_index function with rdi, esi, edx registers
	
	movl %eax, %esi // Move the result to esi register (second arg)
	movl %ecx, %edx // Move the color value to edx register (third arg)
	movq %rbx, %rdi // Move rbx back to rdi register (first arg) to get ready to call function
	call set_pixel // Call set_pixel function with rdi, esi, edx registers

.Lpixel_end:
	movl %eax, %ecx // Take away value from eax to make it a void function (No return)
	popq %r13 // Restore r13 register
	popq %r12 // Restore r12 register
	popq %rbx // Restore rbx register
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
    /*
    * Register use:
    *   %rbx - pointer to struct Image
    *   %r12d - pointer to struct Rect
    *   %r13d - uint32_t color value
    *   %r14d - i
    *   %r15d - j
    *
    * Stack use (8 bytes):
    *   0(%rsp) - max_i
    *   4(%rsp) - min_j
    */

    /* Save values of callee-saved registers */
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    /* Reserve 8 bytes of memory for local variables */
    subq $8, %rsp

    movq %rdi, %rbx                        /* Save pointer to struct Image onto callee saved rbx register */
    movq %rsi, %r12                        /* Save pointer to struct Rect onto callee saved r12 register */
    movl %edx, %r13d                       /* Save the color value onto callee saved r13d register */
    
    movl RECT_Y_OFFSET(%r12), %r10d        /* Save rect->y in caller-saved r10d register */
    addl RECT_HEIGHT_OFFSET(%r12), %r10d   /* Add rect->height to rect->y */
    movl %r10d, 0(%rsp)                    /* Save rect->y + rect->height in callee saved rsp at position 0 */
    
    movl RECT_X_OFFSET(%r12), %r10d        /* Save rect->x in caller-saved r10d register */
    addl RECT_WIDTH_OFFSET(%r12), %r10d    /* Add rect->width to rect->x */
    movl %r10d, 4(%rsp)                    /* Save rect->x + rect->width in callee saved rsp at position 4 */
    
    movl RECT_Y_OFFSET(%r12), %r14d        /* Set i as rect->y */
.Lrect_outer_loop:
    movl RECT_X_OFFSET(%r12), %r15d        /* Reset j as rect->x */
    cmp 0(%rsp), %r14d
    jge .Lrect_outer_loop_end              /* If i is greater than or equal to max_i, jump to the end */
.Lrect_inner_loop:
    cmp 4(%rsp), %r15d
    jge .Lrect_inner_loop_end              /* If j is greater than or equal to max_j, jump to the end of loop*/
    movq %rbx, %rdi                        /* Move pointer of img to rdi register */
    movl %r15d, %esi                       /* Move j to esi register */
    movl %r14d, %edx                       /* Move i to edx register */
    movl %r13d, %ecx                       /* Move color to ecx register */
    call draw_pixel                        /* Call draw_pixel with rdi, esi, edx, and ecx registers */
    inc %r15d                              /* Increment j by 1 */
    jmp .Lrect_inner_loop                  /* Jump to start of inner loop */
.Lrect_inner_loop_end:
    inc %r14d                              /* Increment i by 1 */
    jmp .Lrect_outer_loop                  /* Jump to start of outer loop */
.Lrect_outer_loop_end:

    /* Restore value of pointer */
    addq $8, %rsp

    /* Restore values of callee-saved registers */
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
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
    /*
    * Register use:
    *   %rbx - pointer to struct Image
    *   %r12d - x
    *   %r13d - y
    *   %r14d - i
    *   %r15d - j
    *
    * Stack use (28 bytes):
    *   0(%rsp) - max_i
    *   4(%rsp) - min_j
    *   8(%rsp) - max_j
    *   12(%rsp) - r
    *   16(%rsp) - color
    *   20(%rsp) - square(r)
    */

    /* Save values of callee-saved registers */
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    /* Reserve 28 bytes of memory for local variables */
    subq $28, %rsp

    movq %rdi, %rbx                        /* Save pointer to struct Image onto callee saved rbx register */
    movl %esi, %r12d                       /* Save x onto callee saved r12d register */
    movl %edx, %r13d                       /* Save y onto callee saved r13d register */
    subl %ecx, %edx
    movl %edx, %r14d                       /* Save y-r in r14d */
    movl %r13d, 0(%rsp)
    addl %ecx, 0(%rsp)                     /* Save y+r in callee saved rsp at position 0 */
    subl %ecx, %esi
    movl %esi, 4(%rsp)                     /* Save x-r in callee saved rsp at position 4 */
    movl %r12d, 8(%rsp)
    addl %ecx, 8(%rsp)                     /* Save x+r in callee saved rsp at position 8 */
    movl %ecx, 12(%rsp)                    /* Save r in callee saved rsp at position 12 */
    movl %r8d, 16(%rsp)                    /* Save color in callee saved rsp at position 16 */
    movl %ecx, %edi                        /* Move r in edi register */
    call square                            /* Call square with edi register */
    movq %rax, 20(%rsp)                    /* Save r^2 in callee saved rsp at position 20 */
.Lcircle_outer_loop:
    movl 4(%rsp), %r15d                    /* Reset j as min_j (x-r) */
    cmp 0(%rsp), %r14d
    jg .Lcircle_outer_loop_end             /* If i is greater than max_i, then jump to end */
.Lcircle_inner_loop:
    cmp 8(%rsp), %r15d
    jg .Lcircle_inner_loop_end             /* If j is greater than max_j, then jump to end of loop */
    movl %r15d, %edi                       /* Move j to edi register */
    movl %r14d, %esi                       /* Move i to esi register */
    movl %r12d, %edx                       /* Move x to edx register */
    movl %r13d, %ecx                       /* Move y to ecx register */
    call square_dist                       /* Call square_dist with edi, esi, edx, and ecx registers */
    cmp %rax, 20(%rsp)
    jl .Lcircle_inner_loop_continue        /* If square of r is less than square_dist (not in circle), skip this iteration*/
    movq %rbx, %rdi                        /* Move pointer of img to rdi register */
    movl %r15d, %esi                       /* Move j to esi register */
    movl %r14d, %edx                       /* Move i to edx register */
    movl 16(%rsp), %ecx                    /* Move color to ecx register */
    call draw_pixel                        /* Call draw_pixel with rdi, esi, edx, and ecx registers */
.Lcircle_inner_loop_continue:
    inc %r15d                              /* Increment j by 1 */
    jmp .Lcircle_inner_loop                /* Jump to start of inner loop */
.Lcircle_inner_loop_end:
    inc %r14d                              /* Increment i by 1 */
    jmp .Lcircle_outer_loop                /* Jump to start of outer loop */
.Lcircle_outer_loop_end:

    /* Restore value of pointer */
    addq $28, %rsp

    /* Restore values of callee-saved registers */
    popq %r15 // Restore the r15 register
    popq %r14 // Restore the r14 register
    popq %r13 // Restore the r13 register
    popq %r12 // Restore the r12 register
    popq %rbx // Restore the rbx register
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
