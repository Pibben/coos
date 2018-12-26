//
// Created by per on 2017-10-07.
//

#ifndef KERNEL_COPROCESSOR_H
#define KERNEL_COPROCESSOR_H

#define WRITE_CP32(src, spec) asm volatile("MCR p15, " spec :: "r" (src))
#define READ_CP32(dst, spec) asm volatile("MRC p15, " spec : "=r" (dst))
#define READ_CP64(dst, spec) asm volatile("MRRC p15, " spec : "=r" (dst))

#define MK_CP(rn, op1, rm, op2) #op1 ", %0, " #rn ", " #rm", " #op2
#define MK_CP64(op1, rm) #op1 ", %Q0, %R0, " #rm

#define MIDR   MK_CP(c0, 0, c0, 0)
#define CTR    MK_CP(c0, 0, c0, 1)
#define TCMTR  MK_CP(c0, 0, c0, 2)
#define TLBTR  MK_CP(c0, 0, c0, 3)
#define MPIDR  MK_CP(c0, 0, c0, 5)
#define REVIDR MK_CP(c0, 0, c0, 6)

#define ID_PFR0  MK_CP(c0, 0, c1, 0)
#define ID_PFR1  MK_CP(c0, 0, c1, 1)
#define ID_DFR0  MK_CP(c0, 0, c1, 2)
#define ID_AFR0  MK_CP(c0, 0, c1, 3)
#define ID_MMFR0 MK_CP(c0, 0, c1, 4)
#define ID_MMFR1 MK_CP(c0, 0, c1, 5)
#define ID_MMFR2 MK_CP(c0, 0, c1, 6)
#define ID_MMFR3 MK_CP(c0, 0, c1, 7)

#define ID_ISAR0 MK_CP(c0, 0, c2, 0)
#define ID_ISAR1 MK_CP(c0, 0, c2, 1)
#define ID_ISAR2 MK_CP(c0, 0, c2, 2)
#define ID_ISAR3 MK_CP(c0, 0, c2, 3)
#define ID_ISAR4 MK_CP(c0, 0, c2, 4)
#define ID_ISAR5 MK_CP(c0, 0, c2, 5)

#define CCSIDR MK_CP(c0, 1, c0, 0)
#define CLIDR  MK_CP(c0, 1, c0, 1)
#define AIDR   MK_CP(c0, 1, c0, 7)

#define VPIDR  MK_CP(c0, 4, c0, 0)
#define VMPIDR MK_CP(c0, 4, c0, 5)

#define SCTLR MK_CP(c1, 0, c0, 0)
#define ACTLR MK_CP(c1, 0, c0, 1)
#define CPACR MK_CP(c1, 0, c0, 2)

#define SCR   MK_CP(c1, 0, c1, 0)
#define SDER  MK_CP(c1, 0, c1, 1)
#define NSACR MK_CP(c1, 0, c1, 2)

#define TTBR0 MK_CP(c2, 0, c0, 0)
#define TTBR1 MK_CP(c2, 0, c0, 1)
#define TTBCR MK_CP(c2, 0, c0, 2)

#define DACR MK_CP(c3, 0, c0, 0)

#define CNTFRQ    MK_CP(c14, 0, c0, 0)
#define CNTP_TVAL MK_CP(c14, 0, c2, 0)
#define CNTP_CTL  MK_CP(c14, 0, c2, 1)
#define CNTPCT    MK_CP64(0, c14)
#define CNTV_TVAL MK_CP(c14, 0, c3, 0)
#define CNTV_CTL  MK_CP(c14, 0, c3, 1)
#define CNTVCT    MK_CP64(1, c14)

#endif //KERNEL_COPROCESSOR_H
