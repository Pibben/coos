// To keep this in the first portion of the binary.
.section ".text.boot"
 
// Make _start global.
.global _start
.global _enable_interrupts
.global _disable_interrupts
.extern interruptHandler
.extern _cstartup

 
// Entry point for the kernel.
// r15 -> should begin execution at 0x8000.
// r0 -> 0x00000000
// r1 -> 0x00000C42
// r2 -> 0x00000100 - start of ATAGS
// preserve these registers as argument for kernel_main

// See ARM section A2.2 (Processor Modes)

.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

// See ARM section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20

.equ    ACTLR_SMP,              (1 << 6)
.equ    SCTLR_I,                (1 << 12)


_start:
    ldr pc, _reset_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

_reset_h:                           .word   _reset_
_undefined_instruction_vector_h:    .word   halt
_software_interrupt_vector_h:       .word   halt
_prefetch_abort_vector_h:           .word   halt
_data_abort_vector_h:               .word   halt
_unused_handler_h:                  .word   _reset_
_interrupt_vector_h:                .word   irq_handler
_fast_interrupt_vector_h:           .word   halt

_reset_:
    // From https://github.com/qemu/qemu/blob/master/hw/arm/boot.c#L243
//#ifdef QEMU
    mrc p15, 0, r0, c1 , c1, 0 // read SCR
    orr r0, #0x31              // enable AW, FW, NS
    mcr p15, 0, r0, c1, c1, 0  //  write SCR */

    // in QEMU all of 4 ARM CPUs are started simultaniously
    // by default. I don't know if this is the real hw behaviour,
    // but here I jump to halt if CPU ID (stored in MPIDR
    // register, first 2 bits) is not 0

    mrc p15, #0, r1, c0, c0, #5
    and r1, r1, #3
    cmp r1, #0
    bne halt
//#endif
    // We enter execution in supervisor mode. For more information on
    // processor modes see ARM Section A2.2 (Processor Modes)

    // Copy vectors from entry (0x8000) to vector start (0x0000)
    mov     r0, #0x8000
    mov     r1, #0x0000
    ldmia   r0!,{r4, r5, r6, r7, r8, r9, r10, r11}
    stmia   r1!,{r4, r5, r6, r7, r8, r9, r10, r11}
    ldmia   r0!,{r4, r5, r6, r7, r8, r9, r10, r11}
    stmia   r1!,{r4, r5, r6, r7, r8, r9, r10, r11}

    // We're going to use interrupt mode, so setup the interrupt mode
    // stack pointer which differs to the application stack pointer:
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0
    mov sp, #(63 * 1024 * 1024)

    // Switch back to supervisor mode (our application mode) and
    // set the stack pointer towards the end of RAM. Remember that the
    // stack works its way down memory, our heap will work it's way
    // up memory toward the application stack.
    mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0

    // Set the stack pointer at some point in RAM that won't harm us
    // It's different from the IRQ stack pointer above and no matter
    // what the GPU/CPU memory split, 64MB is available to the CPU
    // Keep it within the limits and also keep it aligned to a 32-bit
    // boundary!
    mov     sp, #(64 * 1024 * 1024)

    // The c-startup function which we never return from. This function will
    // initialise the ro data section (most things that have the const
    // declaration) and initialise the bss section variables to 0 (generally
    // known as automatics). It'll then call main, which should never return.
    bl      _cstartup

    // halt
halt:
    wfe
    b halt

irq_handler:
    sub lr, lr, #4
    stmfd sp!, {r0-r3, lr}
    bl interruptHandler
    ldmfd sp!, {r0-r3, pc}^

// core wakes up here, set stack and call core_main
.global core_wakeup
core_wakeup:
	// get core ID
	mrc	p15,0,r0,c0,c0,5
	and	r0, #3

	// get stack
	ldr	r1, =stacks
	ldr	sp, [r1, r0, lsl #2]

	// Call kernel_main
    ldr     r3, =core_main
    blx      r3
  //  bl core_main

	// busy loop
1:	b	1b


.section ".data"
	.align 2
	.global stacks
stacks:	.word 0x03B00000 // 59MB
	.word 0x03C00000
	.word 0x03D00000
	.word 0x03E00000
