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
	.file	"nan.c"
	.text
	.global	KOLIBA_NaN
	.section	.rodata
	.align	3
	.type	KOLIBA_NaN, %object
	.size	KOLIBA_NaN, 8
KOLIBA_NaN:
	.word	0
	.word	2147352576
	.ident	"GCC: (GNU Tools for Arm Embedded Processors 8-2019-q3-update) 8.3.1 20190703 (release) [gcc-8-branch revision 273027]"
