#pragma once

#include "disassm.h"

 // Size of opcode tables
#define OPCODE_TABLE_SIZE 256
/*
 * Instruction Group Index Table
 * Maps specific opcodes to group table indices
 * Used when an opcode has the OPATTR_GROUP attribute
 */
static const uint8_t g_group_index_table[OPCODE_TABLE_SIZE] = {
    /* 80 */ 0,  // Group 1
    /* 81 */ 0,  // Group 1
    /* 82 */ 0,  // Group 1 (invalid in 64-bit mode)
    /* 83 */ 0,  // Group 1
    /* 8F */ 1,  // Group 1A
    /* C0 */ 2,  // Group 2
    /* C1 */ 2,  // Group 2
    /* C6 */ 11, // Group 11
    /* C7 */ 11, // Group 11
    /* D0 */ 2,  // Group 2
    /* D1 */ 2,  // Group 2
    /* D2 */ 2,  // Group 2
    /* D3 */ 2,  // Group 2
    /* F6 */ 3,  // Group 3
    /* F7 */ 3,  // Group 3
    /* FE */ 4,  // Group 4
    /* FF */ 5,  // Group 5

    /* 0F:00 */ 6,  // Group 6
    /* 0F:01 */ 7,  // Group 7
    /* 0F:18 */ 16, // Group 16 (prefetch)
    /* 0F:71 */ 12, // Group 12
    /* 0F:72 */ 13, // Group 13
    /* 0F:73 */ 14, // Group 14
    /* 0F:AE */ 15, // Group 15
    /* 0F:B9 */ 10, // Group 10
    /* 0F:BA */ 8,  // Group 8
    /* 0F:C7 */ 9,  // Group 9
};

/*
 * Group opcode attributes table
 * Contains the opcode attributes for each ModR/M.reg value within a group
 */
static const uint8_t g_group_opattr_table[32][8] = {
    /* Group 1 (0) - ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m, imm */
    {
        OPATTR_MODRM, // 000: ADD
        OPATTR_MODRM, // 001: OR
        OPATTR_MODRM, // 010: ADC
        OPATTR_MODRM, // 011: SBB
        OPATTR_MODRM, // 100: AND
        OPATTR_MODRM, // 101: SUB
        OPATTR_MODRM, // 110: XOR
        OPATTR_MODRM  // 111: CMP
    },

    /* Group 1A (1) - POP r/m16/32/64 */
    {
        OPATTR_MODRM, // 000: POP r/m
        OPATTR_ERROR, // 001: Reserved
        OPATTR_ERROR, // 010: Reserved
        OPATTR_ERROR, // 011: Reserved
        OPATTR_ERROR, // 100: Reserved
        OPATTR_ERROR, // 101: Reserved
        OPATTR_ERROR, // 110: Reserved
        OPATTR_ERROR  // 111: Reserved
    },

    /* Group 2 (2) - ROL/ROR/RCL/RCR/SHL/SHR/SAR r/m, imm/1/CL */
    {
        OPATTR_MODRM, // 000: ROL
        OPATTR_MODRM, // 001: ROR
        OPATTR_MODRM, // 010: RCL
        OPATTR_MODRM, // 011: RCR
        OPATTR_MODRM, // 100: SHL/SAL
        OPATTR_MODRM, // 101: SHR
        OPATTR_ERROR, // 110: Reserved
        OPATTR_MODRM  // 111: SAR
    },

    /* Group 3 (3) - TEST/NOT/NEG/MUL/IMUL/DIV/IDIV r/m */
    {
        OPATTR_MODRM | OPATTR_IMM8,  // 000: TEST r/m8, imm8 (F6)
        // 000: TEST r/m16/32/64, imm16/32 (F7)
OPATTR_ERROR,                // 001: Reserved
OPATTR_MODRM,                // 010: NOT
OPATTR_MODRM,                // 011: NEG
OPATTR_MODRM,                // 100: MUL
OPATTR_MODRM,                // 101: IMUL
OPATTR_MODRM,                // 110: DIV
OPATTR_MODRM                 // 111: IDIV
},

/* Group 4 (4) - INC/DEC r/m8 */
{
    OPATTR_MODRM, // 000: INC r/m8
    OPATTR_MODRM, // 001: DEC r/m8
    OPATTR_ERROR, // 010: Reserved
    OPATTR_ERROR, // 011: Reserved
    OPATTR_ERROR, // 100: Reserved
    OPATTR_ERROR, // 101: Reserved
    OPATTR_ERROR, // 110: Reserved
    OPATTR_ERROR  // 111: Reserved
},

/* Group 5 (5) - INC/DEC/CALL/CALL far/JMP/JMP far/PUSH r/m16/32/64 */
{
    OPATTR_MODRM,                // 000: INC r/m16/32/64
    OPATTR_MODRM,                // 001: DEC r/m16/32/64
    OPATTR_MODRM,                // 010: CALL r/m16/32/64
    OPATTR_MODRM,                // 011: CALL m16:16/32/64
    OPATTR_MODRM,                // 100: JMP r/m16/32/64
    OPATTR_MODRM,                // 101: JMP m16:16/32/64
    OPATTR_MODRM,                // 110: PUSH r/m16/32/64
    OPATTR_ERROR                 // 111: Reserved
},

/* Group 6 (6) - SLDT/STR/LLDT/LTR/VERR/VERW/JMPE */
{
    OPATTR_MODRM,                // 000: SLDT r/m16
    OPATTR_MODRM,                // 001: STR r/m16
    OPATTR_MODRM,                // 010: LLDT r/m16
    OPATTR_MODRM,                // 011: LTR r/m16
    OPATTR_MODRM,                // 100: VERR r/m16
    OPATTR_MODRM,                // 101: VERW r/m16
    OPATTR_ERROR,                // 110: JMPE (IA-64)
    OPATTR_ERROR                 // 111: Reserved
},

/* Group 7 (7) - SGDT/SIDT/LGDT/LIDT/SMSW/LMSW/INVLPG/SWAPGS/RDTSCP */
{
    OPATTR_MODRM,                // 000: SGDT m
    OPATTR_MODRM,                // 001: SIDT m
    OPATTR_MODRM,                // 010: LGDT m
    OPATTR_MODRM,                // 011: LIDT m
    OPATTR_MODRM,                // 100: SMSW r/m16
    OPATTR_MODRM,                // 101: LMSW r/m16
    OPATTR_MODRM,                // 110: INVLPG m / SWAPGS / RDTSCP
    OPATTR_MODRM                 // 111: Reserved
},

/* Group 8 (8) - BT/BTS/BTR/BTC r/m, imm8 */
{
    OPATTR_MODRM | OPATTR_IMM8,  // 000: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 001: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 010: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 011: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 100: BT r/m16/32/64, imm8
    OPATTR_MODRM | OPATTR_IMM8,  // 101: BTS r/m16/32/64, imm8
    OPATTR_MODRM | OPATTR_IMM8,  // 110: BTR r/m16/32/64, imm8
    OPATTR_MODRM | OPATTR_IMM8   // 111: BTC r/m16/32/64, imm8
},

/* Group 9 (9) - Reserved/CMPXCHG8B/CMPXCHG16B/VMPTRLD/VMCLEAR/VMXON/etc. */
{
    OPATTR_ERROR,                // 000: Reserved
    OPATTR_MODRM,                // 001: CMPXCHG8B/CMPXCHG16B m64/m128
    OPATTR_ERROR,                // 010: Reserved for VMX instructions
    OPATTR_ERROR,                // 011: Reserved for VMX instructions
    OPATTR_ERROR,                // 100: Reserved for VMX instructions
    OPATTR_ERROR,                // 101: Reserved for VMX instructions
    OPATTR_ERROR,                // 110: VMPTRLD/VMCLEAR/VMXON
    OPATTR_ERROR                 // 111: VMPTRST
},

/* Group 10 (10) - UD1/UD2/POPCNT */
{
    OPATTR_ERROR,                // 000: Reserved
    OPATTR_ERROR,                // 001: Reserved
    OPATTR_ERROR,                // 010: Reserved
    OPATTR_ERROR,                // 011: Reserved
    OPATTR_ERROR,                // 100: Reserved
    OPATTR_ERROR,                // 101: Reserved
    OPATTR_ERROR,                // 110: Reserved
    OPATTR_ERROR                 // 111: Reserved
},

/* Group 11 (11) - MOV r/m, imm */
{
    OPATTR_MODRM | OPATTR_IMM8,  // 000: MOV r/m8, imm8 (C6)
    // 000: MOV r/m16/32/64, imm16/32 (C7)
OPATTR_ERROR,                // 001: Reserved
OPATTR_ERROR,                // 010: Reserved
OPATTR_ERROR,                // 011: Reserved
OPATTR_ERROR,                // 100: Reserved
OPATTR_ERROR,                // 101: Reserved
OPATTR_ERROR,                // 110: Reserved
OPATTR_ERROR                 // 111: Reserved
},

/* Group 12 (12) - Reserved/PSRLW/PSRAW/PSLLW */
{
    OPATTR_ERROR,                // 000: Reserved
    OPATTR_ERROR,                // 001: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 010: PSRLW xmm, imm8
    OPATTR_ERROR,                // 011: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 100: PSRAW xmm, imm8
    OPATTR_ERROR,                // 101: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 110: PSLLW xmm, imm8
    OPATTR_ERROR                 // 111: Reserved
},

/* Group 13 (13) - Reserved/PSRLD/PSRAD/PSLLD */
{
    OPATTR_ERROR,                // 000: Reserved
    OPATTR_ERROR,                // 001: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 010: PSRLD xmm, imm8
    OPATTR_ERROR,                // 011: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 100: PSRAD xmm, imm8
    OPATTR_ERROR,                // 101: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 110: PSLLD xmm, imm8
    OPATTR_ERROR                 // 111: Reserved
},

/* Group 14 (14) - Reserved/PSRLQ/PSLLQ/PSRLDQ/PSLLDQ */
{
    OPATTR_ERROR,                // 000: Reserved
    OPATTR_ERROR,                // 001: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 010: PSRLQ xmm, imm8
    OPATTR_MODRM | OPATTR_IMM8,  // 011: PSRLDQ xmm, imm8
    OPATTR_ERROR,                // 100: Reserved
    OPATTR_ERROR,                // 101: Reserved
    OPATTR_MODRM | OPATTR_IMM8,  // 110: PSLLQ xmm, imm8
    OPATTR_MODRM | OPATTR_IMM8   // 111: PSLLDQ xmm, imm8
},

/* Group 15 (15) - Various memory and fence instructions */
{
    OPATTR_MODRM,                // 000: FXSAVE m512byte
    OPATTR_MODRM,                // 001: FXRSTOR m512byte
    OPATTR_MODRM,                // 010: LDMXCSR m32
    OPATTR_MODRM,                // 011: STMXCSR m32
    OPATTR_ERROR,                // 100: Reserved
    OPATTR_MODRM,                // 101: LFENCE/XRSTOR/XSAVES
    OPATTR_MODRM,                // 110: MFENCE/XSAVEOPT/CLWB
    OPATTR_MODRM                 // 111: SFENCE/CLFLUSH
},

/* Group 16 (16) - Prefetch instructions */
{
    OPATTR_MODRM,                // 000: PREFETCHNTA m8
    OPATTR_MODRM,                // 001: PREFETCHT0 m8
    OPATTR_MODRM,                // 010: PREFETCHT1 m8
    OPATTR_MODRM,                // 011: PREFETCHT2 m8
    OPATTR_MODRM,                // 100: HINT_NOP r/m16/32
    OPATTR_MODRM,                // 101: HINT_NOP r/m16/32
    OPATTR_MODRM,                // 110: HINT_NOP r/m16/32
    OPATTR_MODRM                 // 111: HINT_NOP r/m16/32
},

/* Group 17 (17) - MPX instructions and non-temporal moves */
{
    OPATTR_MODRM,                // 000: Reserved
    OPATTR_MODRM,                // 001: Reserved
    OPATTR_MODRM,                // 010: Reserved
    OPATTR_MODRM,                // 011: Reserved
    OPATTR_MODRM,                // 100: Reserved
    OPATTR_MODRM,                // 101: Reserved
    OPATTR_MODRM,                // 110: Reserved
    OPATTR_MODRM                 // 111: Reserved
},

/* Group P (18) - Prefetch instructions (SSE3) */
{
    OPATTR_MODRM,                // 000: PREFETCH m8
    OPATTR_MODRM,                // 001: PREFETCH m8
    OPATTR_MODRM,                // 010: PREFETCH m8
    OPATTR_MODRM,                // 011: PREFETCH m8
    OPATTR_MODRM,                // 100: Reserved
    OPATTR_MODRM,                // 101: Reserved
    OPATTR_MODRM,                // 110: Reserved
    OPATTR_MODRM                 // 111: Reserved
},

/* Additional space for new groups as the architecture evolves */
{ OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR,
  OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR },
{ OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR,
  OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR },
{ OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR,
  OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR },
{ OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR,
  OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR },
{ OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR,
  OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR },
{ OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR,
  OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR },
{ OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR,
  OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR },
{ OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR,
  OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR },
{ OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR,
  OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR, OPATTR_ERROR }
};

/*
 * FPU instruction tables for opcodes D8-DF
 */

 /* FPU instructions with mod != 3 (memory operands) */
static const uint8_t g_fpu_mod01_table[8] = {
    OPATTR_MODRM, // D8: FADD/FMUL/FCOM/FCOMP/FSUB/FSUBR/FDIV/FDIVR mem32
    OPATTR_MODRM, // D9: FLD/FST/FSTP/FLDENV/FLDCW/FSTENV/FSTCW mem
    OPATTR_MODRM, // DA: FIADD/FIMUL/FICOM/FICOMP/FISUB/FISUBR/FIDIV/FIDIVR mem32
    OPATTR_MODRM, // DB: FILD/FISTTP/FIST/FISTP/FLD/FSTP mem
    OPATTR_MODRM, // DC: FADD/FMUL/FCOM/FCOMP/FSUB/FSUBR/FDIV/FDIVR mem64
    OPATTR_MODRM, // DD: FLD/FISTTP/FST/FSTP/FRSTOR/FSAVE/FSTSW mem
    OPATTR_MODRM, // DE: FIADD/FIMUL/FICOM/FICOMP/FISUB/FISUBR/FIDIV/FIDIVR mem16
    OPATTR_MODRM  // DF: FILD/FISTTP/FIST/FISTP/FBLD/FBSTP/FSTSWAX mem
};

/* FPU instructions with mod == 3 (register operands) - complete table */
static const uint8_t g_fpu_mod3_table[8][8] = {
    /* D8 */ {
        OPATTR_NONE, // FADD ST, ST(0)
        OPATTR_NONE, // FADD ST, ST(1)
        OPATTR_NONE, // FADD ST, ST(2)
        OPATTR_NONE, // FADD ST, ST(3)
        OPATTR_NONE, // FADD ST, ST(4)
        OPATTR_NONE, // FADD ST, ST(5)
        OPATTR_NONE, // FADD ST, ST(6)
        OPATTR_NONE  // FADD ST, ST(7)
    },

    /* D9 */ {
        OPATTR_NONE, // FLD ST(0)
        OPATTR_NONE, // FLD ST(1)
        OPATTR_NONE, // FLD ST(2)
        OPATTR_NONE, // FLD ST(3)
        OPATTR_NONE, // FLD ST(4)
        OPATTR_NONE, // FLD ST(5)
        OPATTR_NONE, // FLD ST(6)
        OPATTR_NONE  // FLD ST(7)
    },

    /* DA */ {
        OPATTR_NONE, // FCMOVB ST, ST(0)
        OPATTR_NONE, // FCMOVB ST, ST(1)
        OPATTR_NONE, // FCMOVB ST, ST(2)
        OPATTR_NONE, // FCMOVB ST, ST(3)
        OPATTR_NONE, // FCMOVB ST, ST(4)
        OPATTR_NONE, // FCMOVB ST, ST(5)
        OPATTR_NONE, // FCMOVB ST, ST(6)
        OPATTR_NONE  // FCMOVB ST, ST(7)
    },

    /* DB */ {
        OPATTR_NONE, // FCMOVE ST, ST(0)
        OPATTR_NONE, // FCMOVE ST, ST(1)
        OPATTR_NONE, // FCMOVE ST, ST(2)
        OPATTR_NONE, // FCMOVE ST, ST(3)
        OPATTR_NONE, // FCMOVE ST, ST(4)
        OPATTR_NONE, // FCMOVE ST, ST(5)
        OPATTR_NONE, // FCMOVE ST, ST(6)
        OPATTR_NONE  // FCMOVE ST, ST(7)
    },

    /* DC */ {
        OPATTR_NONE, // FADD ST(0), ST
        OPATTR_NONE, // FADD ST(1), ST
        OPATTR_NONE, // FADD ST(2), ST
        OPATTR_NONE, // FADD ST(3), ST
        OPATTR_NONE, // FADD ST(4), ST
        OPATTR_NONE, // FADD ST(5), ST
        OPATTR_NONE, // FADD ST(6), ST
        OPATTR_NONE  // FADD ST(7), ST
    },

    /* DD */ {
        OPATTR_NONE, // FFREE ST(0)
        OPATTR_NONE, // FFREE ST(1)
        OPATTR_NONE, // FFREE ST(2)
        OPATTR_NONE, // FFREE ST(3)
        OPATTR_NONE, // FFREE ST(4)
        OPATTR_NONE, // FFREE ST(5)
        OPATTR_NONE, // FFREE ST(6)
        OPATTR_NONE  // FFREE ST(7)
    },

    /* DE */ {
        OPATTR_NONE, // FADDP ST(0), ST
        OPATTR_NONE, // FADDP ST(1), ST
        OPATTR_NONE, // FADDP ST(2), ST
        OPATTR_NONE, // FADDP ST(3), ST
        OPATTR_NONE, // FADDP ST(4), ST
        OPATTR_NONE, // FADDP ST(5), ST
        OPATTR_NONE, // FADDP ST(6), ST
        OPATTR_NONE  // FADDP ST(7), ST
    },

    /* DF */ {
        OPATTR_NONE, // FFREEP ST(0)
        OPATTR_NONE, // FFREEP ST(1)
        OPATTR_NONE, // FFREEP ST(2)
        OPATTR_NONE, // FFREEP ST(3)
        OPATTR_NONE, // FFREEP ST(4)
        OPATTR_NONE, // FFREEP ST(5)
        OPATTR_NONE, // FFREEP ST(6)
        OPATTR_NONE  // FFREEP ST(7)
    }
};

/*
 * Prefix validity table
 * Indicates which prefixes are valid for each opcode
 */
static const uint8_t g_prefix_table[OPCODE_TABLE_SIZE] = {
    /* 00-0F */
    /* 00 */ PREFIX_ANY,       // ADD r/m8, r8
    /* 01 */ PREFIX_ANY,       // ADD r/m16/32, r16/32
    /* 02 */ PREFIX_ANY,       // ADD r8, r/m8
    /* 03 */ PREFIX_ANY,       // ADD r16/32, r/m16/32
    /* 04 */ PREFIX_ANY,       // ADD AL, imm8
    /* 05 */ PREFIX_ANY,       // ADD AX/EAX, imm16/32
    /* 06 */ PREFIX_ANY,       // PUSH ES (Invalid in 64-bit mode)
    /* 07 */ PREFIX_ANY,       // POP ES (Invalid in 64-bit mode)
    /* 08 */ PREFIX_ANY,       // OR r/m8, r8
    /* 09 */ PREFIX_ANY,       // OR r/m16/32, r16/32
    /* 0A */ PREFIX_ANY,       // OR r8, r/m8
    /* 0B */ PREFIX_ANY,       // OR r16/32, r/m16/32
    /* 0C */ PREFIX_ANY,       // OR AL, imm8
    /* 0D */ PREFIX_ANY,       // OR AX/EAX, imm16/32
    /* 0E */ PREFIX_ANY,       // PUSH CS (Invalid in 64-bit mode)
    /* 0F */ PREFIX_ANY,       // Two-byte opcode prefix

    /* 10-1F */
    /* 10 */ PREFIX_ANY,       // ADC r/m8, r8
    /* 11 */ PREFIX_ANY,       // ADC r/m16/32, r16/32
    /* 12 */ PREFIX_ANY,       // ADC r8, r/m8
    /* 13 */ PREFIX_ANY,       // ADC r16/32, r/m16/32
    /* 14 */ PREFIX_ANY,       // ADC AL, imm8
    /* 15 */ PREFIX_ANY,       // ADC AX/EAX, imm16/32
    /* 16 */ PREFIX_ANY,       // PUSH SS (Invalid in 64-bit mode)
    /* 17 */ PREFIX_ANY,       // POP SS (Invalid in 64-bit mode)
    /* 18 */ PREFIX_ANY,       // SBB r/m8, r8
    /* 19 */ PREFIX_ANY,       // SBB r/m16/32, r16/32
    /* 1A */ PREFIX_ANY,       // SBB r8, r/m8
    /* 1B */ PREFIX_ANY,       // SBB r16/32, r/m16/32
    /* 1C */ PREFIX_ANY,       // SBB AL, imm8
    /* 1D */ PREFIX_ANY,       // SBB AX/EAX, imm16/32
    /* 1E */ PREFIX_ANY,       // PUSH DS (Invalid in 64-bit mode)
    /* 1F */ PREFIX_ANY,       // POP DS (Invalid in 64-bit mode)

    /* 20-2F */
    /* 20 */ PREFIX_ANY,       // AND r/m8, r8
    /* 21 */ PREFIX_ANY,       // AND r/m16/32, r16/32
    /* 22 */ PREFIX_ANY,       // AND r8, r/m8
    /* 23 */ PREFIX_ANY,       // AND r16/32, r/m16/32
    /* 24 */ PREFIX_ANY,       // AND AL, imm8
    /* 25 */ PREFIX_ANY,       // AND AX/EAX, imm16/32
    /* 26 */ PREFIX_NONE,      // ES prefix (acts as a prefix itself)
    /* 27 */ PREFIX_ANY,       // DAA (Invalid in 64-bit mode)
    /* 28 */ PREFIX_ANY,       // SUB r/m8, r8
    /* 29 */ PREFIX_ANY,       // SUB r/m16/32, r16/32
    /* 2A */ PREFIX_ANY,       // SUB r8, r/m8
    /* 2B */ PREFIX_ANY,       // SUB r16/32, r/m16/32
    /* 2C */ PREFIX_ANY,       // SUB AL, imm8
    /* 2D */ PREFIX_ANY,       // SUB AX/EAX, imm16/32
    /* 2E */ PREFIX_NONE,      // CS prefix (acts as a prefix itself)
    /* 2F */ PREFIX_ANY,       // DAS (Invalid in 64-bit mode)

    /* 30-3F */
    /* 30 */ PREFIX_ANY,       // XOR r/m8, r8
    /* 31 */ PREFIX_ANY,       // XOR r/m16/32, r16/32
    /* 32 */ PREFIX_ANY,       // XOR r8, r/m8
    /* 33 */ PREFIX_ANY,       // XOR r16/32, r/m16/32
    /* 34 */ PREFIX_ANY,       // XOR AL, imm8
    /* 35 */ PREFIX_ANY,       // XOR AX/EAX, imm16/32
    /* 36 */ PREFIX_NONE,      // SS prefix (acts as a prefix itself)
    /* 37 */ PREFIX_ANY,       // AAA (Invalid in 64-bit mode)
    /* 38 */ PREFIX_ANY,       // CMP r/m8, r8
    /* 39 */ PREFIX_ANY,       // CMP r/m16/32, r16/32
    /* 3A */ PREFIX_ANY,       // CMP r8, r/m8
    /* 3B */ PREFIX_ANY,       // CMP r16/32, r/m16/32
    /* 3C */ PREFIX_ANY,       // CMP AL, imm8
    /* 3D */ PREFIX_ANY,       // CMP AX/EAX, imm16/32
    /* 3E */ PREFIX_NONE,      // DS prefix (acts as a prefix itself)
    /* 3F */ PREFIX_ANY,       // AAS (Invalid in 64-bit mode)

    /* 40-4F - REX prefixes in 64-bit mode */
    /* 40 */ PREFIX_NONE,      // REX prefix
    /* 41 */ PREFIX_NONE,      // REX.B prefix
    /* 42 */ PREFIX_NONE,      // REX.X prefix
    /* 43 */ PREFIX_NONE,      // REX.XB prefix
    /* 44 */ PREFIX_NONE,      // REX.R prefix
    /* 45 */ PREFIX_NONE,      // REX.RB prefix
    /* 46 */ PREFIX_NONE,      // REX.RX prefix
    /* 47 */ PREFIX_NONE,      // REX.RXB prefix
    /* 48 */ PREFIX_NONE,      // REX.W prefix
    /* 49 */ PREFIX_NONE,      // REX.WB prefix
    /* 4A */ PREFIX_NONE,      // REX.WX prefix
    /* 4B */ PREFIX_NONE,      // REX.WXB prefix
    /* 4C */ PREFIX_NONE,      // REX.WR prefix
    /* 4D */ PREFIX_NONE,      // REX.WRB prefix
    /* 4E */ PREFIX_NONE,      // REX.WRX prefix
    /* 4F */ PREFIX_NONE,      // REX.WRXB prefix

    /* 50-5F */
    /* 50 */ PREFIX_ANY,       // PUSH r16/32/64
    /* 51 */ PREFIX_ANY,       // PUSH r16/32/64
    /* 52 */ PREFIX_ANY,       // PUSH r16/32/64
    /* 53 */ PREFIX_ANY,       // PUSH r16/32/64
    /* 54 */ PREFIX_ANY,       // PUSH r16/32/64
    /* 55 */ PREFIX_ANY,       // PUSH r16/32/64
    /* 56 */ PREFIX_ANY,       // PUSH r16/32/64
    /* 57 */ PREFIX_ANY,       // PUSH r16/32/64
    /* 58 */ PREFIX_ANY,       // POP r16/32/64
    /* 59 */ PREFIX_ANY,       // POP r16/32/64
    /* 5A */ PREFIX_ANY,       // POP r16/32/64
    /* 5B */ PREFIX_ANY,       // POP r16/32/64
    /* 5C */ PREFIX_ANY,       // POP r16/32/64
    /* 5D */ PREFIX_ANY,       // POP r16/32/64
    /* 5E */ PREFIX_ANY,       // POP r16/32/64
    /* 5F */ PREFIX_ANY,       // POP r16/32/64

    /* 60-6F */
    /* 60 */ PREFIX_ANY,       // PUSHAD/PUSHA (Invalid in 64-bit mode)
    /* 61 */ PREFIX_ANY,       // POPAD/POPA (Invalid in 64-bit mode)
    /* 62 */ PREFIX_ANY,       // BOUND (Invalid in 64-bit mode)
    /* 63 */ PREFIX_ANY,       // MOVSXD/ARPL
    /* 64 */ PREFIX_NONE,      // FS prefix (acts as a prefix itself)
    /* 65 */ PREFIX_NONE,      // GS prefix (acts as a prefix itself)
    /* 66 */ PREFIX_NONE,      // Operand size prefix (acts as a prefix itself)
    /* 67 */ PREFIX_NONE,      // Address size prefix (acts as a prefix itself)
    /* 68 */ PREFIX_ANY,       // PUSH imm16/32
    /* 69 */ PREFIX_ANY,       // IMUL r16/32, r/m16/32, imm16/32
    /* 6A */ PREFIX_ANY,       // PUSH imm8
    /* 6B */ PREFIX_ANY,       // IMUL r16/32, r/m16/32, imm8
    /* 6C */ PREFIX_ANY,       // INSB
    /* 6D */ PREFIX_ANY,       // INSW/INSD
    /* 6E */ PREFIX_ANY,       // OUTSB
    /* 6F */ PREFIX_ANY,       // OUTSW/OUTSD

    /* 70-7F */
    /* 70 */ PREFIX_ANY,       // JO rel8
    /* 71 */ PREFIX_ANY,       // JNO rel8
    /* 72 */ PREFIX_ANY,       // JB/JC/JNAE rel8
    /* 73 */ PREFIX_ANY,       // JNB/JNC/JAE rel8
    /* 74 */ PREFIX_ANY,       // JE/JZ rel8
    /* 75 */ PREFIX_ANY,       // JNE/JNZ rel8
    /* 76 */ PREFIX_ANY,       // JBE/JNA rel8
    /* 77 */ PREFIX_ANY,       // JNBE/JA rel8
    /* 78 */ PREFIX_ANY,       // JS rel8
    /* 79 */ PREFIX_ANY,       // JNS rel8
    /* 7A */ PREFIX_ANY,       // JP/JPE rel8
    /* 7B */ PREFIX_ANY,       // JNP/JPO rel8
    /* 7C */ PREFIX_ANY,       // JL/JNGE rel8
    /* 7D */ PREFIX_ANY,       // JNL/JGE rel8
    /* 7E */ PREFIX_ANY,       // JLE/JNG rel8
    /* 7F */ PREFIX_ANY,       // JNLE/JG rel8

    /* 80-8F */
    /* 80 */ PREFIX_ANY,       // ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m8, imm8
    /* 81 */ PREFIX_ANY,       // ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m16/32, imm16/32
    /* 82 */ PREFIX_ANY,       // Invalid in 64-bit mode
    /* 83 */ PREFIX_ANY,       // ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m16/32, imm8
    /* 84 */ PREFIX_ANY,       // TEST r/m8, r8
    /* 85 */ PREFIX_ANY,       // TEST r/m16/32, r16/32
    /* 86 */ PREFIX_ANY,       // XCHG r/m8, r8
    /* 87 */ PREFIX_ANY,       // XCHG r/m16/32, r16/32
    /* 88 */ PREFIX_ANY,       // MOV r/m8, r8
    /* 89 */ PREFIX_ANY,       // MOV r/m16/32, r16/32
    /* 8A */ PREFIX_ANY,       // MOV r8, r/m8
    /* 8B */ PREFIX_ANY,       // MOV r16/32, r/m16/32
    /* 8C */ PREFIX_ANY,       // MOV r/m16, Sreg
    /* 8D */ PREFIX_ANY,       // LEA r16/32, m
    /* 8E */ PREFIX_ANY,       // MOV Sreg, r/m16
    /* 8F */ PREFIX_ANY,       // POP r/m16/32

    /* 90-9F */
    /* 90 */ PREFIX_ANY,       // NOP / XCHG rAX, rAX
    /* 91 */ PREFIX_ANY,       // XCHG rAX, rCX
    /* 92 */ PREFIX_ANY,       // XCHG rAX, rDX
    /* 93 */ PREFIX_ANY,       // XCHG rAX, rBX
    /* 94 */ PREFIX_ANY,       // XCHG rAX, rSP
    /* 95 */ PREFIX_ANY,       // XCHG rAX, rBP
    /* 96 */ PREFIX_ANY,       // XCHG rAX, rSI
    /* 97 */ PREFIX_ANY,       // XCHG rAX, rDI
    /* 98 */ PREFIX_ANY,       // CBW/CWDE/CDQE
    /* 99 */ PREFIX_ANY,       // CWD/CDQ/CQO
    /* 9A */ PREFIX_ANY,       // CALL far (Invalid in 64-bit mode)
    /* 9B */ PREFIX_ANY,       // WAIT/FWAIT
    /* 9C */ PREFIX_ANY,       // PUSHF/PUSHFD/PUSHFQ
    /* 9D */ PREFIX_ANY,       // POPF/POPFD/POPFQ
    /* 9E */ PREFIX_ANY,       // SAHF
    /* 9F */ PREFIX_ANY,       // LAHF

    /* A0-AF */
    /* A0 */ PREFIX_ADDR_SIZE, // MOV AL, moffs8
    /* A1 */ PREFIX_ADDR_SIZE, // MOV AX/EAX/RAX, moffs16/32/64
    /* A2 */ PREFIX_ADDR_SIZE, // MOV moffs8, AL
    /* A3 */ PREFIX_ADDR_SIZE, // MOV moffs16/32/64, AX/EAX/RAX
    /* A4 */ PREFIX_REP,       // MOVSB
    /* A5 */ PREFIX_REP,       // MOVSW/MOVSD/MOVSQ
    /* A6 */ PREFIX_REP,       // CMPSB
    /* A7 */ PREFIX_REP,       // CMPSW/CMPSD/CMPSQ
    /* A8 */ PREFIX_ANY,       // TEST AL, imm8
    /* A9 */ PREFIX_ANY,       // TEST AX/EAX/RAX, imm16/32
    /* AA */ PREFIX_REP,       // STOSB
    /* AB */ PREFIX_REP,       // STOSW/STOSD/STOSQ
    /* AC */ PREFIX_REP,       // LODSB
    /* AD */ PREFIX_REP,       // LODSW/LODSD/LODSQ
    /* AE */ PREFIX_REP,       // SCASB
    /* AF */ PREFIX_REP,       // SCASW/SCASD/SCASQ

    /* B0-BF */
    /* B0 */ PREFIX_ANY,       // MOV r8L, imm8
    /* B1 */ PREFIX_ANY,       // MOV r8H/r9L, imm8
    /* B2 */ PREFIX_ANY,       // MOV r8H/r10L, imm8
    /* B3 */ PREFIX_ANY,       // MOV r8H/r11L, imm8
    /* B4 */ PREFIX_ANY,       // MOV r8H/r12L, imm8
    /* B5 */ PREFIX_ANY,       // MOV r8H/r13L, imm8
    /* B6 */ PREFIX_ANY,       // MOV r8H/r14L, imm8
    /* B7 */ PREFIX_ANY,       // MOV r8H/r15L, imm8
    /* B8 */ PREFIX_ANY,       // MOV r16/32/64, imm16/32/64
    /* B9 */ PREFIX_ANY,       // MOV r16/32/64, imm16/32/64
    /* BA */ PREFIX_ANY,       // MOV r16/32/64, imm16/32/64
    /* BB */ PREFIX_ANY,       // MOV r16/32/64, imm16/32/64
    /* BC */ PREFIX_ANY,       // MOV r16/32/64, imm16/32/64
    /* BD */ PREFIX_ANY,       // MOV r16/32/64, imm16/32/64
    /* BE */ PREFIX_ANY,       // MOV r16/32/64, imm16/32/64
    /* BF */ PREFIX_ANY,       // MOV r16/32/64, imm16/32/64

    /* C0-CF */
    /* C0 */ PREFIX_ANY,       // ROL/ROR/RCL/RCR/SHL/SHR/SAR r/m8, imm8
    /* C1 */ PREFIX_ANY,       // ROL/ROR/RCL/RCR/SHL/SHR/SAR r/m16/32, imm8
    /* C2 */ PREFIX_ANY,       // RET imm16
    /* C3 */ PREFIX_ANY,       // RET
    /* C4 */ PREFIX_ANY,       // LES r16/32, m16:16/32 (VEX prefix in 64-bit mode)
    /* C5 */ PREFIX_ANY,       // LDS r16/32, m16:16/32 (VEX prefix in 64-bit mode)
    /* C6 */ PREFIX_ANY,       // MOV r/m8, imm8
    /* C7 */ PREFIX_ANY,       // MOV r/m16/32/64, imm16/32
    /* C8 */ PREFIX_ANY,       // ENTER imm16, imm8
    /* C9 */ PREFIX_ANY,       // LEAVE
    /* CA */ PREFIX_ANY,       // RET FAR imm16
    /* CB */ PREFIX_ANY,       // RET FAR
    /* CC */ PREFIX_ANY,       // INT 3
    /* CD */ PREFIX_ANY,       // INT imm8
    /* CE */ PREFIX_ANY,       // INTO (Invalid in 64-bit mode)
    /* CF */ PREFIX_ANY,       // IRET/IRETD/IRETQ

    /* D0-DF */
    /* D0 */ PREFIX_ANY,       // ROL/ROR/RCL/RCR/SHL/SHR/SAR r/m8, 1
    /* D1 */ PREFIX_ANY,       // ROL/ROR/RCL/RCR/SHL/SHR/SAR r/m16/32, 1
    /* D2 */ PREFIX_ANY,       // ROL/ROR/RCL/RCR/SHL/SHR/SAR r/m8, CL
    /* D3 */ PREFIX_ANY,       // ROL/ROR/RCL/RCR/SHL/SHR/SAR r/m16/32, CL
    /* D4 */ PREFIX_ANY,       // AAM imm8 (Invalid in 64-bit mode)
    /* D5 */ PREFIX_ANY,       // AAD imm8 (Invalid in 64-bit mode)
    /* D6 */ PREFIX_ANY,       // Reserved
    /* D7 */ PREFIX_ANY,       // XLAT/XLATB
    /* D8 */ PREFIX_ANY,       // FPU instructions
    /* D9 */ PREFIX_ANY,       // FPU instructions
    /* DA */ PREFIX_ANY,       // FPU instructions
    /* DB */ PREFIX_ANY,       // FPU instructions
    /* DC */ PREFIX_ANY,       // FPU instructions
    /* DD */ PREFIX_ANY,       // FPU instructions
    /* DE */ PREFIX_ANY,       // FPU instructions
    /* DF */ PREFIX_ANY,       // FPU instructions

    /* E0-EF */
    /* E0 */ PREFIX_ANY,       // LOOPNE/LOOPNZ rel8
    /* E1 */ PREFIX_ANY,       // LOOPE/LOOPZ rel8
    /* E2 */ PREFIX_ANY,       // LOOP rel8
    /* E3 */ PREFIX_ANY,       // JCXZ/JECXZ/JRCXZ rel8
    /* E4 */ PREFIX_ANY,       // IN AL, imm8
    /* E5 */ PREFIX_ANY,       // IN AX/EAX, imm8
    /* E6 */ PREFIX_ANY,       // OUT imm8, AL
    /* E7 */ PREFIX_ANY,       // OUT imm8, AX/EAX
    /* E8 */ PREFIX_ANY,       // CALL rel16/32
    /* E9 */ PREFIX_ANY,       // JMP rel16/32
    /* EA */ PREFIX_ANY,       // JMP far (Invalid in 64-bit mode)
    /* EB */ PREFIX_ANY,       // JMP rel8
    /* EC */ PREFIX_ANY,       // IN AL, DX
    /* ED */ PREFIX_ANY,       // IN AX/EAX, DX
    /* EE */ PREFIX_ANY,       // OUT DX, AL
    /* EF */ PREFIX_ANY,       // OUT DX, AX/EAX

    /* F0-FF */
    /* F0 */ PREFIX_NONE,      // LOCK prefix (acts as a prefix itself)
    /* F1 */ PREFIX_ANY,       // INT1/ICEBP
    /* F2 */ PREFIX_NONE,      // REPNE/REPNZ prefix (acts as a prefix itself)
    /* F3 */ PREFIX_NONE,      // REP/REPE/REPZ prefix (acts as a prefix itself)
    /* F4 */ PREFIX_ANY,       // HLT
    /* F5 */ PREFIX_ANY,       // CMC
    /* F6 */ PREFIX_ANY,       // TEST/NOT/NEG/MUL/IMUL/DIV/IDIV r/m8
    /* F7 */ PREFIX_ANY,       // TEST/NOT/NEG/MUL/IMUL/DIV/IDIV r/m16/32/64
    /* F8 */ PREFIX_ANY,       // CLC
    /* F9 */ PREFIX_ANY,       // STC
    /* FA */ PREFIX_ANY,       // CLI
    /* FB */ PREFIX_ANY,       // STI
    /* FC */ PREFIX_ANY,       // CLD
    /* FD */ PREFIX_ANY,       // STD
    /* FE */ PREFIX_ANY,       // INC/DEC r/m8
    /* FF */ PREFIX_ANY        // INC/DEC/CALL/CALL far/JMP/JMP far/PUSH r/m16/32/64
};
