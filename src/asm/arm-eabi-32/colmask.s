	.arch armv6
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"colmask.c"
	.text
	.align	2
	.global	KOLIBA_ColorMask
	.arch armv6
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ColorMask, %function
KOLIBA_ColorMask:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r1, #0
	cmpne	r0, #0
	beq	.L34
	cmp	r2, #0
	vldr.64	d6, [r1]
	vldr.64	d5, [r1, #8]
	vldr.64	d7, [r1, #16]
	beq	.L3
	ldrb	r3, [r2, #16]	@ zero_extendqisi2
	cmp	r3, #6
	bls	.L78
.L3:
	vstr.64	d6, [r0]
	vstr.64	d5, [r0, #8]
	vstr.64	d7, [r0, #16]
	bx	lr
.L78:
	ldrb	r1, [r2, #17]	@ zero_extendqisi2
	cmp	r1, #2
	bhi	.L3
	vldr.64	d3, [r2]
	vldr.64	d4, [r2, #8]
	vcmpe.f64	d3, d4
	vmrs	APSR_nzcv, FPSCR
	bgt	.L3
	str	lr, [sp, #-4]!
	vpush.64	{d8, d9, d10}
	ldrb	ip, [r2, #18]	@ zero_extendqisi2
	sub	sp, sp, #12
	tst	ip, #1
	mov	lr, ip
	vldrne.64	d2, .L81
	vsubne.f64	d6, d2, d6
	vsubne.f64	d5, d2, d5
	vsubne.f64	d7, d2, d7
	cmp	r1, #1
	beq	.L7
	cmp	r1, #2
	bne	.L79
	vcmpe.f64	d6, d7
	vmrs	APSR_nzcv, FPSCR
	bpl	.L71
	vcmpe.f64	d7, d5
	vmrs	APSR_nzcv, FPSCR
	ble	.L71
	vcmpe.f64	d6, d5
	vmov.f64	d2, d6
	vmov.f64	d6, d7
	vmrs	APSR_nzcv, FPSCR
	vmovge.f64	d7, d2
	bge	.L11
	vmov.f64	d7, d5
.L76:
	vmov.f64	d5, d2
.L11:
	tst	ip, #2
	vsubne.f64	d7, d6, d5
	vsubeq.f64	d7, d6, d7
	tst	ip, #4
	beq	.L18
	vcmpe.f64	d6, #0
	vmrs	APSR_nzcv, FPSCR
	vdivgt.f64	d7, d7, d6
.L18:
	vcmpe.f64	d4, d7
	vmrs	APSR_nzcv, FPSCR
	bls	.L42
	vcmpe.f64	d3, d7
	vmrs	APSR_nzcv, FPSCR
	bge	.L43
	vsub.f64	d7, d7, d3
	vsub.f64	d4, d4, d3
	vdiv.f64	d8, d7, d4
	cmp	r3, #6
	addls	pc, pc, r3, asl #2
	b	.L9
.L22:
	b	.L27
	b	.L26
	b	.L25
	b	.L9
	b	.L44
	b	.L46
	b	.L21
	.p2align 1
.L34:
	mov	r0, #0
	bx	lr
.L79:
	vcmpe.f64	d6, d5
	vmrs	APSR_nzcv, FPSCR
	ble	.L71
	vcmpe.f64	d6, d7
	vmrs	APSR_nzcv, FPSCR
	bgt	.L11
.L71:
	vldr.64	d8, .L81+8
.L9:
	tst	lr, #8
	vldrne.64	d7, .L81
	vsubne.f64	d8, d7, d8
	vstr.64	d8, [r0]
	vstr.64	d8, [r0, #8]
	vstr.64	d8, [r0, #16]
	add	sp, sp, #12
	@ sp needed
	vldm	sp!, {d8-d10}
	ldr	pc, [sp], #4
.L7:
	vcmpe.f64	d6, d5
	vmrs	APSR_nzcv, FPSCR
	bpl	.L71
	vcmpe.f64	d7, d5
	vmrs	APSR_nzcv, FPSCR
	bpl	.L71
	vcmpe.f64	d6, d7
	vmov.f64	d2, d6
	vmov.f64	d6, d5
	vmrs	APSR_nzcv, FPSCR
	vmovge.f64	d5, d7
	vmovge.f64	d7, d2
	bge	.L11
	b	.L76
.L27:
	vmul.f64	d8, d8, d8
.L26:
	vmul.f64	d8, d8, d8
.L25:
	vmul.f64	d8, d8, d8
	b	.L9
.L46:
	vmov.f64	d10, d8
.L23:
	vcmp.f64	d10, #0
	vsqrt.f64	d9, d10
	vmrs	APSR_nzcv, FPSCR
	bmi	.L80
.L24:
	vcmp.f64	d9, #0
	vsqrt.f64	d8, d9
	vmrs	APSR_nzcv, FPSCR
	bmi	.L72
	ldrb	ip, [r2, #18]	@ zero_extendqisi2
.L32:
	mov	lr, ip
	b	.L9
.L21:
	vcmp.f64	d8, #0
	vsqrt.f64	d10, d8
	vmrs	APSR_nzcv, FPSCR
	bpl	.L23
	vmov.f64	d0, d8
	str	r0, [sp, #4]
	str	r2, [sp]
	bl	sqrt(PLT)
	ldr	r2, [sp]
	ldr	r0, [sp, #4]
	b	.L23
.L44:
	vmov.f64	d9, d8
	b	.L24
.L42:
	vldr.64	d8, .L81
.L20:
	cmp	r3, #6
	addls	pc, pc, r3, asl #2
	b	.L9
.L33:
	b	.L27
	b	.L26
	b	.L25
	b	.L9
	b	.L31
	b	.L46
	b	.L21
	.p2align 1
.L31:
	vsqrt.f64	d8, d8
	b	.L32
.L43:
	vldr.64	d8, .L81+8
	b	.L20
.L72:
	vmov.f64	d0, d9
	str	r0, [sp, #4]
	str	r2, [sp]
	bl	sqrt(PLT)
	ldr	r2, [sp]
	ldr	r0, [sp, #4]
	ldrb	lr, [r2, #18]	@ zero_extendqisi2
	b	.L9
.L80:
	vmov.f64	d0, d10
	str	r0, [sp, #4]
	str	r2, [sp]
	bl	sqrt(PLT)
	ldr	r2, [sp]
	ldr	r0, [sp, #4]
	b	.L24
.L82:
	.align	3
.L81:
	.word	0
	.word	1072693248
	.word	0
	.word	0
	.size	KOLIBA_ColorMask, .-KOLIBA_ColorMask
	.comm	KOLIBA_FundamentalMalletFlags,19,4
	.ident	"GCC: (GNU Tools for Arm Embedded Processors 8-2019-q3-update) 8.3.1 20190703 (release) [gcc-8-branch revision 273027]"
