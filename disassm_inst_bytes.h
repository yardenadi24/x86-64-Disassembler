#pragma once

#include <stdint.h>

/*
 * REX Prefix Byte Structure (64-bit mode only)
 *
 * Format: 0100 WRXB
 * - W (bit 3): 0 = Default operand size, 1 = 64-bit operand size
 * - R (bit 2): Extension of the ModR/M reg field
 * - X (bit 1): Extension of the SIB index field
 * - B (bit 0): Extension of the ModR/M r/m field or SIB base field
 */

 // REX prefix identification mask (0x40 to 0x4F)
#define REX_PREFIX_MASK       0xF0
#define REX_PREFIX_VALUE      0x40

// REX prefix bit positions
#define REX_W_BIT             0x08    // Bit 3
#define REX_R_BIT             0x04    // Bit 2
#define REX_X_BIT             0x02    // Bit 1
#define REX_B_BIT             0x01    // Bit 0

// Macros for checking REX bits
#define REX_IS_REX(byte)      (((byte) & REX_PREFIX_MASK) == REX_PREFIX_VALUE)
#define REX_W(byte)           (((byte) & REX_W_BIT) != 0)
#define REX_R(byte)           (((byte) & REX_R_BIT) != 0)
#define REX_X(byte)           (((byte) & REX_X_BIT) != 0)
#define REX_B(byte)           (((byte) & REX_B_BIT) != 0)

/*
 * ModR/M Byte Structure
 *
 * Format: mod reg r/m
 * - mod (bits 6-7): Addressing mode
 * - reg (bits 3-5): Register operand or opcode extension
 * - r/m (bits 0-2): Register or memory operand
 */

 // ModR/M field masks
#define MODRM_MOD_MASK        0xC0    // Bits 6-7
#define MODRM_REG_MASK        0x38    // Bits 3-5
#define MODRM_RM_MASK         0x07    // Bits 0-2

// ModR/M field shift values
#define MODRM_MOD_SHIFT       6
#define MODRM_REG_SHIFT       3
#define MODRM_RM_SHIFT        0

// ModR/M mod values
#define MODRM_MOD_INDIRECT    0       // [reg] or disp32 for r/m=5
#define MODRM_MOD_DISP8       1       // [reg]+disp8
#define MODRM_MOD_DISP32      2       // [reg]+disp32
#define MODRM_MOD_REGISTER    3       // reg (direct)

// ModR/M r/m special values
#define MODRM_RM_SIB          4       // SIB follows when mod != 3
#define MODRM_RM_DISP32       5       // disp32 when mod = 0

// Macros for extracting ModR/M fields
#define MODRM_MOD(byte)       (((byte) & MODRM_MOD_MASK) >> MODRM_MOD_SHIFT)
#define MODRM_REG(byte)       (((byte) & MODRM_REG_MASK) >> MODRM_REG_SHIFT)
#define MODRM_RM(byte)        (((byte) & MODRM_RM_MASK) >> MODRM_RM_SHIFT)

// With REX extensions
#define MODRM_REG_EX(modrm, rex) ((MODRM_REG(modrm)) | (REX_R(rex) ? 8 : 0))
#define MODRM_RM_EX(modrm, rex)  ((MODRM_RM(modrm))  | (REX_B(rex) ? 8 : 0))

/*
 * SIB Byte Structure
 *
 * Format: scale index base
 * - scale (bits 6-7): Scale factor (0=1, 1=2, 2=4, 3=8)
 * - index (bits 3-5): Index register
 * - base (bits 0-2): Base register
 */

 // SIB field masks
#define SIB_SCALE_MASK        0xC0    // Bits 6-7
#define SIB_INDEX_MASK        0x38    // Bits 3-5
#define SIB_BASE_MASK         0x07    // Bits 0-2

// SIB field shift values
#define SIB_SCALE_SHIFT       6
#define SIB_INDEX_SHIFT       3
#define SIB_BASE_SHIFT        0

// SIB special values
#define SIB_INDEX_NONE        4       // No index (esp/rsp, multiply by 1)
#define SIB_BASE_DISP         5       // Use disp32 when mod = 0

// Macros for extracting SIB fields
#define SIB_SCALE(byte)       (((byte) & SIB_SCALE_MASK) >> SIB_SCALE_SHIFT)
#define SIB_INDEX(byte)       (((byte) & SIB_INDEX_MASK) >> SIB_INDEX_SHIFT)
#define SIB_BASE(byte)        (((byte) & SIB_BASE_MASK) >> SIB_BASE_SHIFT)

// Get the actual scale multiplier (1, 2, 4, or 8)
#define SIB_SCALE_FACTOR(scale) (1 << (scale))

// With REX extensions
#define SIB_INDEX_EX(sib, rex) ((SIB_INDEX(sib)) | (REX_X(rex) ? 8 : 0))
#define SIB_BASE_EX(sib, rex)  ((SIB_BASE(sib))  | (REX_B(rex) ? 8 : 0))

/*
 * Register Encoding in x86-64
 *
 * 16-bit mode:
 *   0 = AX, 1 = CX, 2 = DX, 3 = BX, 4 = SP, 5 = BP, 6 = SI, 7 = DI
 *
 * 32-bit mode:
 *   0 = EAX, 1 = ECX, 2 = EDX, 3 = EBX, 4 = ESP, 5 = EBP, 6 = ESI, 7 = EDI
 *
 * 64-bit mode (with REX.B/REX.X/REX.R = 0):
 *   0 = RAX, 1 = RCX, 2 = RDX, 3 = RBX, 4 = RSP, 5 = RBP, 6 = RSI, 7 = RDI
 *
 * 64-bit mode (with REX.B/REX.X/REX.R = 1):
 *   8 = R8, 9 = R9, 10 = R10, 11 = R11, 12 = R12, 13 = R13, 14 = R14, 15 = R15
 */

 // Register encoding
enum RegisterEncoding {
    // 8-bit registers (no REX)
    REG_AL = 0, REG_CL, REG_DL, REG_BL, REG_AH, REG_CH, REG_DH, REG_BH,
    // 8-bit registers (with REX)
    REG_R8L, REG_R9L, REG_R10L, REG_R11L, REG_R12L, REG_R13L, REG_R14L, REG_R15L,

    // 16-bit registers
    REG_AX = 0, REG_CX, REG_DX, REG_BX, REG_SP, REG_BP, REG_SI, REG_DI,
    REG_R8W, REG_R9W, REG_R10W, REG_R11W, REG_R12W, REG_R13W, REG_R14W, REG_R15W,

    // 32-bit registers
    REG_EAX = 0, REG_ECX, REG_EDX, REG_EBX, REG_ESP, REG_EBP, REG_ESI, REG_EDI,
    REG_R8D, REG_R9D, REG_R10D, REG_R11D, REG_R12D, REG_R13D, REG_R14D, REG_R15D,

    // 64-bit registers
    REG_RAX = 0, REG_RCX, REG_RDX, REG_RBX, REG_RSP, REG_RBP, REG_RSI, REG_RDI,
    REG_R8, REG_R9, REG_R10, REG_R11, REG_R12, REG_R13, REG_R14, REG_R15,

    // Segment registers
    REG_ES = 0, REG_CS, REG_SS, REG_DS, REG_FS, REG_GS,

    // Control registers
    REG_CR0 = 0, REG_CR1, REG_CR2, REG_CR3, REG_CR4, REG_CR5, REG_CR6, REG_CR7,
    REG_CR8, REG_CR9, REG_CR10, REG_CR11, REG_CR12, REG_CR13, REG_CR14, REG_CR15,

    // Debug registers
    REG_DR0 = 0, REG_DR1, REG_DR2, REG_DR3, REG_DR4, REG_DR5, REG_DR6, REG_DR7,
    REG_DR8, REG_DR9, REG_DR10, REG_DR11, REG_DR12, REG_DR13, REG_DR14, REG_DR15
};

/*
 * Lock prefix validity table for 1-byte opcodes
 * These opcodes allow the LOCK prefix with memory operands
 */
const uint8_t g_lock_valid_1byte[] = {
    0x00, 0x01, 0x02, 0x03, // ADD
    0x08, 0x09, 0x0A, 0x0B, // OR
    0x10, 0x11, 0x12, 0x13, // ADC
    0x18, 0x19, 0x1A, 0x1B, // SBB
    0x20, 0x21, 0x22, 0x23, // AND
    0x28, 0x29, 0x2A, 0x2B, // SUB
    0x30, 0x31, 0x32, 0x33, // XOR
    0x80, 0x81, 0x83,       // Group 1
    0x86, 0x87,             // XCHG
    0xF6, 0xF7,             // Group 3
    0xFE, 0xFF              // Group 4/5
};

/*
 * Memory-only instruction table for 1-byte opcodes
 * These opcodes only allow memory operands, not register operands
 */
const uint8_t g_memory_only_1byte[] = {
    0xA0, 0xA1, 0xA2, 0xA3, // MOV mem<->AL/AX/EAX/RAX
    0xA4, 0xA5, 0xA6, 0xA7, // MOVS/CMPS
    0xAA, 0xAB, 0xAC, 0xAD, // STOS/LODS
    0xAE, 0xAF,             // SCAS/REP SCAS
    0xC4, 0xC5,             // VEX prefixes (when mod != 3)
    0xC6, 0xC7              // MOV mem, imm
};

/*
 * Lock prefix validity table for 2-byte opcodes
 * These opcodes allow the LOCK prefix with memory operands
 */
const uint8_t g_lock_valid_2byte[] = {
    0xB0, 0xB1,       // CMPXCHG
    0xC0, 0xC1,       // XADD
    0xC7              // Group 9 (CMPXCHG8B/CMPXCHG16B)
};

/*
 * Memory-only instruction table for 2-byte opcodes
 * These opcodes only allow memory operands, not register operands
 */
const uint8_t g_memory_only_2byte[] = {
    0x00, 0x01,       // SLDT/SGDT/etc.
    0x12, 0x13,       // MOVLPS
    0x16, 0x17,       // MOVHPS
    0x2B,             // MOVNTPS
    0xB2, 0xB4, 0xB5, // LSS/LFS/LGS
    0xC3,             // MOVNTI
    0xC7,             // CMPXCHG8B/CMPXCHG16B
    0xE7              // MOVNTQ
};

// 8-bit register names (legacy names without REX)
static const char* g_reg8_names[8] = {
    "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"
};

// 8-bit register names (with REX prefix)
static const char* g_reg8_rex_names[16] = {
    "al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil",
    "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b"
};

// 16-bit register names
static const char* g_reg16_names[16] = {
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di",
    "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"
};

// 32-bit register names
static const char* g_reg32_names[16] = {
    "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi",
    "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"
};

// 64-bit register names
static const char* g_reg64_names[16] = {
    "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
};

// Segment register names
static const char* g_segreg_names[8] = {
    "es", "cs", "ss", "ds", "fs", "gs", "reserved", "reserved"
};

// Control register names
static const char* g_creg_names[16] = {
    "cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7",
    "cr8", "cr9", "cr10", "cr11", "cr12", "cr13", "cr14", "cr15"
};

// Debug register names
static const char* g_dreg_names[16] = {
    "dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7",
    "dr8", "dr9", "dr10", "dr11", "dr12", "dr13", "dr14", "dr15"
};

// MMX register names
static const char* g_mmx_names[8] = {
    "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7"
};

// XMM register names
static const char* g_xmm_names[16] = {
    "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7",
    "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15"
};

// Helper function to get register name based on encoding
static inline const char* get_register_name(int reg_num, int size, int has_rex) {
    switch (size) {
    case 1: // 8-bit
        return has_rex ? g_reg8_rex_names[reg_num] : g_reg8_names[reg_num & 0x7];
    case 2: // 16-bit
        return g_reg16_names[reg_num];
    case 4: // 32-bit
        return g_reg32_names[reg_num];
    case 8: // 64-bit
        return g_reg64_names[reg_num];
    default:
        return "unknown";
    }
}

// Helper function to get segment register name
static inline const char* get_segment_register_name(int reg_num) {
    return g_segreg_names[reg_num & 0x7];
}

// Helper function to get control register name
static inline const char* get_control_register_name(int reg_num) {
    return g_creg_names[reg_num & 0xF];
}

// Helper function to get debug register name
static inline const char* get_debug_register_name(int reg_num) {
    return g_dreg_names[reg_num & 0xF];
}

// Helper function to get MMX register name
static inline const char* get_mmx_register_name(int reg_num) {
    return g_mmx_names[reg_num & 0x7];
}

// Helper function to get XMM register name
static inline const char* get_xmm_register_name(int reg_num) {
    return g_xmm_names[reg_num & 0xF];
}