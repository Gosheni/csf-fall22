	.file	"c_drawing_funcs.c"
	.text
	.globl	in_bounds
	.type	in_bounds, @function
in_bounds:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	cmpl	$0, -12(%rbp)
	js	.L2
	cmpl	$0, -16(%rbp)
	js	.L2
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	-12(%rbp), %edx
	cmpl	%eax, %edx
	jnb	.L2
	movq	-8(%rbp), %rax
	movl	4(%rax), %eax
	movl	-16(%rbp), %edx
	cmpl	%eax, %edx
	jb	.L3
.L2:
	movl	$0, %eax
	jmp	.L4
.L3:
	movl	$1, %eax
.L4:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	in_bounds, .-in_bounds
	.globl	compute_index
	.type	compute_index, @function
compute_index:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %edx
	movl	-16(%rbp), %eax
	imull	%eax, %edx
	movl	-12(%rbp), %eax
	addl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	compute_index, .-compute_index
	.globl	clamp
	.type	clamp, @function
clamp:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	-4(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jge	.L8
	movl	-8(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	.L9
.L8:
	movl	-4(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jle	.L9
	movl	-12(%rbp), %eax
	movl	%eax, -4(%rbp)
.L9:
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	clamp, .-clamp
	.globl	get_r
	.type	get_r, @function
get_r:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	shrl	$24, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	get_r, .-get_r
	.globl	get_g
	.type	get_g, @function
get_g:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	shrl	$16, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	get_g, .-get_g
	.globl	get_b
	.type	get_b, @function
get_b:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	shrl	$8, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	get_b, .-get_b
	.globl	get_a
	.type	get_a, @function
get_a:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	get_a, .-get_a
	.globl	blend_components
	.type	blend_components, @function
blend_components:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	-12(%rbp), %eax
	imull	-4(%rbp), %eax
	movl	%eax, %edx
	movl	$255, %eax
	subl	-12(%rbp), %eax
	imull	-8(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movl	$2155905153, %eax
	imulq	%rdx, %rax
	shrq	$32, %rax
	shrl	$7, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	blend_components, .-blend_components
	.globl	blend_colors
	.type	blend_colors, @function
blend_colors:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	get_a
	movzbl	%al, %r12d
	movl	-40(%rbp), %eax
	movl	%eax, %edi
	call	get_r
	movzbl	%al, %ebx
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	get_r
	movzbl	%al, %eax
	movl	%r12d, %edx
	movl	%ebx, %esi
	movl	%eax, %edi
	call	blend_components
	movzbl	%al, %eax
	movl	%eax, -20(%rbp)
	sall	$8, -20(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	get_a
	movzbl	%al, %r12d
	movl	-40(%rbp), %eax
	movl	%eax, %edi
	call	get_g
	movzbl	%al, %ebx
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	get_g
	movzbl	%al, %eax
	movl	%r12d, %edx
	movl	%ebx, %esi
	movl	%eax, %edi
	call	blend_components
	movzbl	%al, %eax
	orl	%eax, -20(%rbp)
	sall	$8, -20(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	get_a
	movzbl	%al, %r12d
	movl	-40(%rbp), %eax
	movl	%eax, %edi
	call	get_b
	movzbl	%al, %ebx
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	get_b
	movzbl	%al, %eax
	movl	%r12d, %edx
	movl	%ebx, %esi
	movl	%eax, %edi
	call	blend_components
	movzbl	%al, %eax
	orl	%eax, -20(%rbp)
	sall	$8, -20(%rbp)
	orl	$255, -20(%rbp)
	movl	-20(%rbp), %eax
	addq	$24, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	blend_colors, .-blend_colors
	.globl	set_pixel
	.type	set_pixel, @function
set_pixel:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -16(%rbp)
	movl	%esi, -20(%rbp)
	movl	%edx, -24(%rbp)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movl	-20(%rbp), %edx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %edx
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movl	-20(%rbp), %ecx
	salq	$2, %rcx
	leaq	(%rax,%rcx), %rbx
	movl	-24(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	blend_colors
	movl	%eax, (%rbx)
	nop
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	set_pixel, .-set_pixel
	.globl	square
	.type	square, @function
square:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	imulq	%rax, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	square, .-square
	.globl	square_dist
	.type	square_dist, @function
square_dist:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$32, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	%rdx, -32(%rbp)
	movq	%rcx, -40(%rbp)
	movq	-32(%rbp), %rax
	subq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	square
	movq	%rax, %rbx
	movq	-40(%rbp), %rax
	subq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	square
	addq	%rbx, %rax
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	square_dist, .-square_dist
	.globl	draw_pixel
	.type	draw_pixel, @function
draw_pixel:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$40, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movl	%ecx, -36(%rbp)
	movl	-32(%rbp), %edx
	movl	-28(%rbp), %ecx
	movq	-24(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	in_bounds
	testl	%eax, %eax
	je	.L30
	movl	-32(%rbp), %edx
	movl	-28(%rbp), %ecx
	movq	-24(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	compute_index
	movl	%eax, -4(%rbp)
	movl	-36(%rbp), %edx
	movl	-4(%rbp), %ecx
	movq	-24(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	set_pixel
.L30:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	draw_pixel, .-draw_pixel
	.globl	draw_rect
	.type	draw_rect, @function
draw_rect:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$40, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movq	-32(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -4(%rbp)
	jmp	.L32
.L35:
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -8(%rbp)
	jmp	.L33
.L34:
	movl	-36(%rbp), %ecx
	movl	-4(%rbp), %edx
	movl	-8(%rbp), %esi
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	draw_pixel
	addl	$1, -8(%rbp)
.L33:
	movq	-32(%rbp), %rax
	movl	(%rax), %edx
	movq	-32(%rbp), %rax
	movl	8(%rax), %eax
	addl	%edx, %eax
	cmpl	%eax, -8(%rbp)
	jl	.L34
	addl	$1, -4(%rbp)
.L32:
	movq	-32(%rbp), %rax
	movl	4(%rax), %edx
	movq	-32(%rbp), %rax
	movl	12(%rax), %eax
	addl	%edx, %eax
	cmpl	%eax, -4(%rbp)
	jl	.L35
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	draw_rect, .-draw_rect
	.globl	draw_circle
	.type	draw_circle, @function
draw_circle:
.LFB20:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -32(%rbp)
	movl	%esi, -36(%rbp)
	movl	%edx, -40(%rbp)
	movl	%ecx, -44(%rbp)
	movl	%r8d, -48(%rbp)
	movl	-40(%rbp), %eax
	subl	-44(%rbp), %eax
	movl	%eax, -12(%rbp)
	jmp	.L37
.L41:
	movl	-36(%rbp), %eax
	subl	-44(%rbp), %eax
	movl	%eax, -16(%rbp)
	jmp	.L38
.L40:
	movl	-40(%rbp), %eax
	movslq	%eax, %rcx
	movl	-36(%rbp), %eax
	movslq	%eax, %rdx
	movl	-12(%rbp), %eax
	movslq	%eax, %rsi
	movl	-16(%rbp), %eax
	cltq
	movq	%rax, %rdi
	call	square_dist
	movq	%rax, %rbx
	movl	-44(%rbp), %eax
	cltq
	movq	%rax, %rdi
	call	square
	cmpq	%rax, %rbx
	jg	.L39
	movl	-48(%rbp), %ecx
	movl	-12(%rbp), %edx
	movl	-16(%rbp), %esi
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	draw_pixel
.L39:
	addl	$1, -16(%rbp)
.L38:
	movl	-36(%rbp), %edx
	movl	-44(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -16(%rbp)
	jle	.L40
	addl	$1, -12(%rbp)
.L37:
	movl	-40(%rbp), %edx
	movl	-44(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -12(%rbp)
	jle	.L41
	nop
	nop
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	draw_circle, .-draw_circle
	.globl	draw_tile
	.type	draw_tile, @function
draw_tile:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$64, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -48(%rbp)
	movl	%esi, -52(%rbp)
	movl	%edx, -56(%rbp)
	movq	%rcx, -64(%rbp)
	movq	%r8, -72(%rbp)
	movq	-72(%rbp), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	js	.L50
	movq	-72(%rbp), %rax
	movl	4(%rax), %eax
	testl	%eax, %eax
	js	.L50
	movq	-72(%rbp), %rax
	movl	(%rax), %edx
	movq	-72(%rbp), %rax
	movl	8(%rax), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movq	-64(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%edx, %eax
	jb	.L50
	movq	-72(%rbp), %rax
	movl	4(%rax), %edx
	movq	-72(%rbp), %rax
	movl	12(%rax), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movq	-64(%rbp), %rax
	movl	4(%rax), %eax
	cmpl	%edx, %eax
	jb	.L50
	movq	-48(%rbp), %rax
	movl	(%rax), %edx
	movl	-52(%rbp), %eax
	subl	%eax, %edx
	movq	-72(%rbp), %rax
	movl	8(%rax), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	clamp
	movl	%eax, -20(%rbp)
	movq	-48(%rbp), %rax
	movl	4(%rax), %edx
	movl	-56(%rbp), %eax
	subl	%eax, %edx
	movq	-72(%rbp), %rax
	movl	12(%rax), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	clamp
	movl	%eax, -24(%rbp)
	movl	$0, -12(%rbp)
	jmp	.L46
.L49:
	movl	$0, -16(%rbp)
	jmp	.L47
.L48:
	movq	-64(%rbp), %rax
	movq	8(%rax), %rbx
	movq	-72(%rbp), %rax
	movl	4(%rax), %edx
	movl	-12(%rbp), %eax
	addl	%eax, %edx
	movq	-72(%rbp), %rax
	movl	(%rax), %ecx
	movl	-16(%rbp), %eax
	addl	%eax, %ecx
	movq	-64(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	compute_index
	movl	%eax, %eax
	salq	$2, %rax
	addq	%rbx, %rax
	movl	(%rax), %eax
	movl	%eax, -28(%rbp)
	movq	-48(%rbp), %rax
	movq	8(%rax), %rbx
	movl	-12(%rbp), %edx
	movl	-56(%rbp), %eax
	addl	%eax, %edx
	movl	-16(%rbp), %ecx
	movl	-52(%rbp), %eax
	addl	%eax, %ecx
	movq	-48(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	compute_index
	movl	%eax, %eax
	salq	$2, %rax
	leaq	(%rbx,%rax), %rdx
	movl	-28(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -16(%rbp)
.L47:
	movl	-16(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L48
	addl	$1, -12(%rbp)
.L46:
	movl	-12(%rbp), %eax
	cmpl	-24(%rbp), %eax
	jl	.L49
	jmp	.L42
.L50:
	nop
.L42:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	draw_tile, .-draw_tile
	.globl	draw_sprite
	.type	draw_sprite, @function
draw_sprite:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$64, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -48(%rbp)
	movl	%esi, -52(%rbp)
	movl	%edx, -56(%rbp)
	movq	%rcx, -64(%rbp)
	movq	%r8, -72(%rbp)
	movq	-72(%rbp), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	js	.L59
	movq	-72(%rbp), %rax
	movl	4(%rax), %eax
	testl	%eax, %eax
	js	.L59
	movq	-72(%rbp), %rax
	movl	(%rax), %edx
	movq	-72(%rbp), %rax
	movl	8(%rax), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movq	-64(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%edx, %eax
	jb	.L59
	movq	-72(%rbp), %rax
	movl	4(%rax), %edx
	movq	-72(%rbp), %rax
	movl	12(%rax), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movq	-64(%rbp), %rax
	movl	4(%rax), %eax
	cmpl	%edx, %eax
	jb	.L59
	movq	-48(%rbp), %rax
	movl	(%rax), %edx
	movl	-52(%rbp), %eax
	subl	%eax, %edx
	movq	-72(%rbp), %rax
	movl	8(%rax), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	clamp
	movl	%eax, -20(%rbp)
	movq	-48(%rbp), %rax
	movl	4(%rax), %edx
	movl	-56(%rbp), %eax
	subl	%eax, %edx
	movq	-72(%rbp), %rax
	movl	12(%rax), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	clamp
	movl	%eax, -24(%rbp)
	movl	$0, -12(%rbp)
	jmp	.L55
.L58:
	movl	$0, -16(%rbp)
	jmp	.L56
.L57:
	movq	-64(%rbp), %rax
	movq	8(%rax), %rbx
	movq	-72(%rbp), %rax
	movl	4(%rax), %edx
	movl	-12(%rbp), %eax
	addl	%eax, %edx
	movq	-72(%rbp), %rax
	movl	(%rax), %ecx
	movl	-16(%rbp), %eax
	addl	%eax, %ecx
	movq	-64(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	compute_index
	movl	%eax, %eax
	salq	$2, %rax
	addq	%rbx, %rax
	movl	(%rax), %eax
	movl	%eax, -28(%rbp)
	movl	-12(%rbp), %edx
	movl	-56(%rbp), %eax
	leal	(%rdx,%rax), %edi
	movl	-16(%rbp), %edx
	movl	-52(%rbp), %eax
	leal	(%rdx,%rax), %esi
	movl	-28(%rbp), %edx
	movq	-48(%rbp), %rax
	movl	%edx, %ecx
	movl	%edi, %edx
	movq	%rax, %rdi
	call	draw_pixel
	addl	$1, -16(%rbp)
.L56:
	movl	-16(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L57
	addl	$1, -12(%rbp)
.L55:
	movl	-12(%rbp), %eax
	cmpl	-24(%rbp), %eax
	jl	.L58
	jmp	.L51
.L59:
	nop
.L51:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	draw_sprite, .-draw_sprite
	.ident	"GCC: (GNU) 12.1.1 20220507 (Red Hat 12.1.1-1)"
	.section	.note.GNU-stack,"",@progbits
