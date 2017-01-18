//
// Created by Nikolay Martyanov on 12/01/2017.
//

#include <common.h>
#include <inttypes.h>

#define CONFIG_SWITCH_VIA_SMC   0

#define SMC_VECTOR_OFFSET  0x8
#define START_ADDRESS      0x81000000
#define MON_VECTOR_ADDRESS 0x82000000
#define MOD_MASK           0b11111
#define MON_MODE           0b10110


extern uint32_t* _monitor_vectors;


int show_mode(int argc, char* const argv[]) {
    printf("Press any button to read MVBAR\n");
    (void) getc();
    uint32_t mvbar_val;
    asm volatile(
        "mrc p15, 0, r0, c12, c0, 1 \n"
        "str r0, %0"
        : "=m" (mvbar_val)
        :
        : "r0"
    );
    printf("Done!\n");
    printf("MVBAR: 0x%" PRIx32 "\n", mvbar_val);

#if CONFIG_SWITCH_VIA_SMC
    printf("Press any button to fix MVBAR\n");
    (void) getc();
    asm volatile(
        "ldr r0, %0 \n"
        "mcr p15, 0, r0, c12, c0, 1 \n"
        "isb"
        :
        : "m" (_monitor_vectors)
        : "r0"
    );
    printf("Done!\n");

    printf("Press any button to read MVBAR\n");
    (void) getc();
    asm volatile(
        "mrc p15, 0, r0, c12, c0, 1 \n"
        "str r0, %0"
        : "=m" (mvbar_val)
        :
        : "r0"
    );
    printf("MVBAR: 0x%" PRIx32 "\n", mvbar_val);
    printf("Done!\n");
#endif

    printf("Press any button to read SCTLR\n");
    (void) getc();
    uint32_t sctlr_val;
    asm volatile(
        "mrc p15, 0, r0, c1, c0, 0 \n"
        "str r0, %0"
        : "=m" (sctlr_val)
        :
        : "r0"
    );
    printf("SCTLR: 0x08%" PRIx32 "\n", sctlr_val);
    printf("Done!\n");

    printf("Press any button to read CPSR\n");
    (void) getc();
    uint32_t cpsr_val;
    asm volatile (
        "mrs r0, cpsr \n"
        "str r0, %0     "
        : "=m" (cpsr_val)
        :
        : "r0"
    );
    printf("CPSR: 0x%"  PRIx32 "\n", cpsr_val);
    printf("Done!\n");

    printf("Press any button to read ID_PFR1\n");
    (void) getc();
    uint32_t id_pfr1_val;
    asm volatile(
        "mrc p15, 0, r0, c0, c1, 1 \n"
        "str r0, %0                  "
        : "=m" (id_pfr1_val)
        :
        : "r0"
    );
    printf("ID_PFR1: 0x%" PRIx32 "\n", id_pfr1_val);
    printf("Done!\n");

    printf("Press any button to read SCR\n");
    (void) getc();
    uint32_t scr_val;
    asm volatile(
        "mrc p15, 0, r0, c1, c1, 0 \n"
        "str r0, %0                  "
        : "=m" (scr_val)
        :
        : "r0"
    );
    printf("SCR: 0x%08" PRIx32 "\n", scr_val);
    printf("Done!\n");

#if CONFIG_SWITCH_VIA_SMC
    printf("Press any button to switch to MON mode (via smc)\n");
    asm volatile(
        "smc #0"
        :
        :
        :
    );
    asm("_smc_ret:");
#else
    printf("Press any button to switch to MON mode (via CPSR fix)\n");
    (void)getc();
    uint32_t mon_mode = MON_MODE;
    uint32_t mode_mask = ~MODE_MASK;
    asm volatile(
        "mrs r0, cpsr \n"
        "ldr r1, %1 \n"
        "and r0, r1 \n"
        "ldr r1, %0 \n"
        "orr r0, r1 \n"
        "mov r1, sp \n"
        "mov r2, lr \n"
        "msr cpsr, r0 \n"
        "mov sp, r1 \n"
        "mov lr, r2"
        :
        : "m" (mon_mode), "m" (mode_mask)
        : "r0", "r1", "r2", "cc", "memory"
    );
#endif
    printf("Done!\n");
    printf("Press any button to read CPSR\n");
    (void) getc();
    asm volatile(
        "mrs r0, cpsr \n"
        "str r0, %0     "
        : "=m" (cpsr_val)
        :
        : "r0"
    );
    printf("CPSR: 0x%"  PRIx32 "\n", cpsr_val);
    printf("Done!\n");

    printf("Press any button to set SCR.NS to 1\n");
    (void)getc();
    uint32_t ns_bit = 0b1;
    asm volatile(
        "mrc p15, 0, r0, c1, c1, 0 \n"
        "ldr r1, %0 \n"
        "orr r0, r1 \n"
        "mcr p15, 0, r0, c1, c1, 0"
        :
        : "m" (ns_bit)
        : "r0"
    );
    printf("Done!\n");

    printf("Press any button to read SCR\n");
    (void) getc();
    asm volatile(
    "mrc p15, 0, r0, c1, c1, 0 \n"
            "str r0, %0                  "
    : "=m" (scr_val)
    :
    : "r0"
    );
    printf("SCR: 0x%08" PRIx32 "\n", scr_val);
    printf("Done!\n");

    printf("Press any button to read HVBAR\n");
    (void) getc();
    uint32_t hvbar_val;
    asm volatile(
        "mrc p15, 4, r0, c12, c0, 0 \n"
        "str r0, %0"
        : "=m" (hvbar_val)
        :
        : "r0"
    );
    printf("HVBAR: 0x%" PRIx32 "\n", hvbar_val);
    printf("Done!\n");

    printf("Press any button to enter HYP mode (via hvc)\n");
    (void) getc();
    asm volatile(
        "hvc #0"
        :
        :
        :
    );
    printf("Done!\n");

    printf("Press any button to read CPSR\n");
    (void) getc();
    asm volatile(
        "mrs r0, cpsr \n"
        "str r0, %0     "
        : "=m" (cpsr_val)
        :
        : "r0"
    );
    printf("Done!\n");
    printf("CPSR: 0x%"  PRIx32 "\n", cpsr_val);
    (void) getc();

    return 0;
}

#if CONFIG_SWITCH_VIA_SMC
asm (
".global _monitor_vectors \n"
        ".align	5                 \n"
        "_monitor_vectors:        \n"
        ".word 0                  \n"
        ".word 0                  \n"
        "adr pc, _smc_ret         \n"
        ".word 0                  \n"
        ".word 0                  \n"
        ".word 0                  \n"
        ".word 0                  \n"
        ".word 0                  \n"
);
#endif

