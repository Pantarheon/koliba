;;
;;
;;	fixslut.asm
;;
;;	Copyright 2019 G. Adam Stanislav
;;	All rights reserved
;;
;;	nasm -fwin64 fixslut.asm

default	rel

section	.rdata

; We make these public in the hope the Microsoft linker will merge them
; with any identically named constants (the names are produced by MSC).

GLOBAL	__mask@@AbsDouble@, __real@3df0000000000000

__real@3df0000000000000	dq	03df0000000000000h
__mask@@AbsDouble@		dq	07fffffffffffffffh

section	.text

GLOBAL	KOLIBA_FixSlut, KOLIBA_FixFlut

KOLIBA_FixFlut:
KOLIBA_FixSlut:

	; On Entry:
	;
	;	RCX = address of KOLIBA_SLUT or KOLIBA_FLUT
	;
	; On Exit:
	;
	;	RAX = same as RCX on entry

	mov		rax, rcx
	jrcxz	.done

	movsd	xmm1, [__mask@@AbsDouble@]
	sub		ecx, ecx
	movsd	xmm2, [__real@3df0000000000000]
	mov		edx, ecx

	mov		cl, 24
	jmp		.loop

align 16, int3
.loop:

	movsd	xmm0, [rax + 8* (rcx-1)]
	andpd	xmm0, xmm1
	comisd	xmm2, xmm0
	jb		.next

	mov		[rax + 8 * (rcx-1)], rdx

.next:

	loop	.loop

.done:

	ret

section	.drectve	info

	db	'-export:KOLIBA_FixSlut '
	db	'-export:KOLIBA_FixFlut '
