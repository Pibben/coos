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
    // We enter execution in supervisor mode. For more information on
    // processor modes see ARM Section A2.2 (Processor Modes)

    mov     r0, #0x8000
    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

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

_enable_interrupts:
    cpsie   i
    mov     pc, lr

_disable_interrupts:
    cpsid   i
    mov     pc, lr

irq_handler:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl interruptHandler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

.global park_cpus
park_cpus:
    LDR R0,=0x4000008C
    LDR R1,=_CPU_CORE_1_INIT
    STR R1,[R0,#0x10]
    LDR R1,=_CPU_CORE_2_INIT
    STR R1,[R0,#0x20]
    LDR R1,=_CPU_CORE_3_INIT
    STR R1,[R0,#0x30]

    mov pc, lr

_CPU_CORE_1_INIT:
    //Set up core 1
    MRC p15,0,r0,c1,c0,0
    ORR r0,#0x0004 // L1D/L2
    ORR r0,#0x0800 // BP
    ORR r0,#0x1000 // L1I
    MCR p15,0,r0,c1,c0,0
    MRC p15,0,r0,c1,c0,2
    ORR r0,#0x300000
    ORR r0,#0xC00000
    MCR p15,0,r0,c1,c0,2
    MOV r0,#0x40000000
    VMSR fpexc,r0
    LDR SP,=#(80 * 1024 * 1024)
_CPU_1_DO_STUFF:
    B _CPU_1_DO_STUFF

_CPU_CORE_2_INIT:
    //Set up core 2
    MRC p15,0,r0,c1,c0,0
    ORR r0,#0x0004
    ORR r0,#0x0800
    ORR r0,#0x1000
    MCR p15,0,r0,c1,c0,0
    MRC p15,0,r0,c1,c0,2
    ORR r0,#0x300000
    ORR r0,#0xC00000
    MCR p15,0,r0,c1,c0,2
    MOV r0,#0x40000000
    VMSR fpexc,r0
    LDR SP,=#(96 * 1024 * 1024) //Must be unique for CPU2
_CPU_2_DO_STUFF:
    B _CPU_2_DO_STUFF

_CPU_CORE_3_INIT:
    //Set up core 3
    MRC p15,0,r0,c1,c0,0
    ORR r0,#0x0004
    ORR r0,#0x0800
    ORR r0,#0x1000
    MCR p15,0,r0,c1,c0,0
    MRC p15,0,r0,c1,c0,2
    ORR r0,#0x300000
    ORR r0,#0xC00000
    MCR p15,0,r0,c1,c0,2
    MOV r0,#0x40000000
    VMSR fpexc,r0
    LDR SP,=#(112 * 1024 * 1024) //Must be unique for CPU3
_CPU_3_DO_STUFF:
    B _CPU_3_DO_STUFF
