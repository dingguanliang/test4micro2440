#include <core_config.h>

	// interrupt vector table.
.globl	_start	// define globle enter address.
_start:
	b	start_code	// jump to start code.
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

	// initial interrupt vector.
_undefined_instruction:	.word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:		.word prefetch_abort
_data_abort:			.word data_abort
_not_used:				.word not_used
_irq:					.word irq
_fiq:					.word fiq

.balignl	16, 0xdeadbeef	// fill subsection flag.

start_code:
	// change to SVC mode.
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr, r0

	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	// flush v3/v4 cache.
	mcr	p15, 0, r0, c8, c7, 0	// flush v4 TLB.

	// disable MMU stuff and caches.
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	// clear bits 13, 9:8 (--V- --RS).
	bic	r0, r0, #0x00000087	// clear bits 7, 2:0 (B--- -CAM).
	orr	r0, r0, #0x00000002	// set bit 2 (A) Align.
	orr	r0, r0, #0x00001000	// set bit 12 (I) I-Cache.
	mcr	p15, 0, r0, c1, c0, 0

	// check boot mode.
	ldr r0, =0x48000000
	ldr r0, [r0]
    ands r0, r0, #0x06
	bne nom_init

nand_init:
	ldr sp, =0x00001000
	b	init

nom_init:
	ldr sp, =0x40001000

init:
	bl lowlevel_init	// jump to lowlevel initial.

	// check boot mode.
	ldr r0, =0x48000000
	ldr r0, [r0]
    ands r0, r0, #0x06
	bne nom

	// nand boot.
nand:
	ldr sp, =(__code_start - CONFIG_SYS_MALLOC_SIZE - CONFIG_SYS_GBL_DATA_SIZE - CONFIG_STACK_SIZE_IRQ)
	bl	nand_boot
	b	stack_setup

	// nor or mem boot.
nom:			
	adr	r0, _start
	ldr	r1, =__code_start	
	cmp	r0, r1			
	beq	stack_setup

	// code relocate.
	ldr	r2, =__text_start
	ldr	r3, =__text_end
	sub	r2, r3, r2		
	add	r2, r0, r2		

0:
	ldmia r0!, {r3-r10}		
	stmia r1!, {r3-r10}	
	cmp	r0, r2			
	ble	0b

	// mem init.
stack_setup:
	ldr	r0, =__code_start	
	sub	r0, r0, #CONFIG_SYS_MALLOC_SIZE	
	sub	r0, r0, #CONFIG_SYS_GBL_DATA_SIZE
	sub	sp, r0, #CONFIG_STACK_SIZE_IRQ

	// clear bss.
	ldr	r0, =__bss_start		
	ldr	r1, =__bss_end		
	mov	r2, #0x0	

1:
	str	r2, [r0]	
	add	r0, r0, #4
	cmp	r0, r1
	ble	1b

	ldr	pc, =start_armboot	// jump to 2nd.

#define S_FRAME_SIZE	72

#define S_OLD_R0		68
#define S_PSR			64
#define S_PC			60
#define S_LR			56
#define S_SP			52

#define S_IP			48
#define S_FP			44
#define S_R10			40
#define S_R9			36
#define S_R8			32
#define S_R7			28
#define S_R6			24
#define S_R5			20
#define S_R4			16
#define S_R3			12
#define S_R2			8
#define S_R1			4
#define S_R0			0

.macro get_irq_stack			
	ldr	sp, =(__code_start - CONFIG_SYS_MALLOC_SIZE - CONFIG_SYS_GBL_DATA_SIZE)
.endm

.macro save_irq_mode
	sub	sp, sp, #S_FRAME_SIZE
	stmia sp, {r0 - r12}
	add	r7, sp, #S_PC
	stmdb r7, {sp, lr}^	// save last mode sp, lr.
	str	lr, [r7, #0]	// save current	mode lr, spsr, r0.	
	mrs	r6, spsr
	str	r6, [r7, #4]			
	str	r0, [r7, #8]			
	mov	r0, sp			// pass an argument to a function.
.endm

.macro restore_irq_mode
	ldmia sp, {r0 - lr}^	// restore last mode r0 - lr.
	nop
	ldr	lr, [sp, #S_PC]		// get current mode lr.
	add	sp, sp, #S_FRAME_SIZE
	subs pc, lr, #4			// return last mode and continue run.
.endm

undefined_instruction:
	b	.

software_interrupt:
	b	.

prefetch_abort:
	b	.

data_abort:
	b	.

not_used:
	b	.

irq:
	get_irq_stack
	save_irq_mode
	bl	do_irq
	restore_irq_mode

fiq:
	b	.
