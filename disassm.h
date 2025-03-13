#pragma once
#include <stdint.h>
/*
 * Instruction Prefix Masks
 * These are used to check for the presence of specific prefixes
 */
typedef enum {
    PREFIX_NONE = 0x00,
    PREFIX_LOCK = 0x01,  // F0: Lock prefix
    PREFIX_REPNZ = 0x02,  // F2: REPNZ/REPNE prefix
    PREFIX_REP = 0x04,  // F3: REP/REPE/REPZ prefix
    PREFIX_OP_SIZE = 0x08,  // 66: Operand size override
    PREFIX_ADDR_SIZE = 0x10,  // 67: Address size override
    PREFIX_SEG_CS = 0x20,  // 2E: CS segment override
    PREFIX_SEG_SS = 0x40,  // 36: SS segment override
    PREFIX_SEG_DS = 0x80,  // 3E: DS segment override
    PREFIX_SEG_ES = 0x100, // 26: ES segment override
    PREFIX_SEG_FS = 0x200, // 64: FS segment override
    PREFIX_SEG_GS = 0x400, // 65: GS segment override
    PREFIX_REX = 0x800, // 40-4F: REX prefix

    // Combinations
    PREFIX_ANY_SEG = 0x7E0, // Any segment override
    PREFIX_ANY = 0xFFF  // Any prefix
} PrefixMask;

// Macros for checking prefixes - makes code more readable
#define HAS_PREFIX(flags, mask) (((flags) & (mask)) != 0)
#define HAS_ANY_PREFIX(flags, mask) (((flags) & (mask)) != 0)
#define HAS_ALL_PREFIX(flags, mask) (((flags) & (mask)) == (mask))

/*
 * Instruction Flag Masks
 * These are used to check for various instruction properties
 */
typedef enum {
    FLAG_NONE = 0x00000000,
    FLAG_MODRM = 0x00000001, // Has ModR/M byte
    FLAG_SIB = 0x00000002, // Has SIB byte
    FLAG_IMM8 = 0x00000004, // Has 8-bit immediate
    FLAG_IMM16 = 0x00000008, // Has 16-bit immediate
    FLAG_IMM32 = 0x00000010, // Has 32-bit immediate
    FLAG_IMM64 = 0x00000020, // Has 64-bit immediate
    FLAG_DISP8 = 0x00000040, // Has 8-bit displacement
    FLAG_DISP16 = 0x00000080, // Has 16-bit displacement
    FLAG_DISP32 = 0x00000100, // Has 32-bit displacement
    FLAG_RELATIVE = 0x00000200, // Relative addressing

    // Error flags
    FLAG_ERROR = 0x00001000, // General error
    FLAG_ERROR_OPCODE = 0x00002000, // Invalid opcode
    FLAG_ERROR_LENGTH = 0x00004000, // Instruction too long
    FLAG_ERROR_LOCK = 0x00008000, // Invalid lock prefix usage
    FLAG_ERROR_OPERAND = 0x00010000, // Invalid operand

    // Prefix flags (these will be set in the flags field)
    FLAG_PREFIX_REPNZ = 0x01000000,
    FLAG_PREFIX_REP = 0x02000000,
    FLAG_PREFIX_OP_SIZE = 0x04000000,
    FLAG_PREFIX_ADDR_SIZE = 0x08000000,
    FLAG_PREFIX_LOCK = 0x10000000,
    FLAG_PREFIX_SEG = 0x20000000,
    FLAG_PREFIX_REX = 0x40000000,

    // Common masks for checking multiple flags
    FLAG_MASK_ANY_IMM = 0x0000003C, // Any immediate value
    FLAG_MASK_ANY_DISP = 0x000001C0, // Any displacement
    FLAG_MASK_ANY_ERROR = 0x0001F000, // Any error
    FLAG_MASK_ANY_PREFIX = 0x7F000000 // Any prefix
} FlagMask;

// Macros for checking flags - makes code more readable
#define HAS_FLAG(flags, mask) (((flags) & (mask)) != 0)
#define HAS_ANY_FLAG(flags, mask) (((flags) & (mask)) != 0)
#define HAS_ALL_FLAGS(flags, mask) (((flags) & (mask)) == (mask))
#define SET_FLAG(flags, mask) ((flags) |= (mask))
#define CLEAR_FLAG(flags, mask) ((flags) &= ~(mask))

/*
 * Opcode Attributes
 * Define the characteristics and components of each instruction
 */
typedef enum {
    OPATTR_NONE = 0x00,
    OPATTR_MODRM = 0x01, // Instruction has ModR/M byte
    OPATTR_IMM8 = 0x02, // 8-bit immediate
    OPATTR_IMM16 = 0x04, // 16-bit immediate

    /* OPATTR_IMM_P66: Variable-size immediate value that depends on prefixes
     *
     * This attribute indicates that the immediate value size depends on:
     * 1. Whether the 0x66 (operand size override) prefix is present
     * 2. Whether REX.W (64-bit operand size) is present in 64-bit mode
     * 3. The default operand size (16/32-bit) of the current mode
     *
     * Size determination logic:
     * - 64-bit mode with REX.W set: 64-bit immediate (e.g., MOV RAX, imm64)
     * - 66 prefix present: 16-bit immediate (e.g., MOV AX, imm16)
     * - Otherwise: 32-bit immediate (e.g., MOV EAX, imm32)
     */
    OPATTR_IMM_P66 = 0x10,

    OPATTR_REL8 = 0x20, // 8-bit relative offset
    OPATTR_REL32 = 0x40, // 32-bit relative offset
    OPATTR_GROUP = 0x80, // Instruction is part of a group
    OPATTR_ERROR = 0xFF  // Invalid opcode
} OpcodeAttribute;

// Macros for checking opcode attributes
#define HAS_ATTR(attr, mask) (((attr) & (mask)) != 0)

/*
 * Instruction result structure
 */
typedef struct {
    // Length and raw bytes
    uint8_t length;         // Total instruction length in bytes

    // Prefix bytes
    uint8_t prefix_lock;    // F0: Lock prefix
    uint8_t prefix_rep;     // F2/F3: REP/REPNZ prefix
    uint8_t prefix_seg;     // Segment override
    uint8_t prefix_66;      // 66: Operand size override
    uint8_t prefix_67;      // 67: Address size override

    // REX prefix
    uint8_t rex;            // REX prefix byte
    uint8_t rex_w;          // REX.W field (64-bit operand)
    uint8_t rex_r;          // REX.R field (ModR/M reg extension)
    uint8_t rex_x;          // REX.X field (SIB index extension)
    uint8_t rex_b;          // REX.B field (ModR/M rm extension or SIB base extension)

    // Opcode
    uint8_t opcode;         // Primary opcode
    uint8_t opcode2;        // Secondary opcode (for 2-byte opcodes)

    // ModR/M
    uint8_t modrm;          // ModR/M byte
    uint8_t modrm_mod;      // ModR/M mod field
    uint8_t modrm_reg;      // ModR/M reg field
    uint8_t modrm_rm;       // ModR/M r/m field

    // SIB
    uint8_t sib;            // SIB byte
    uint8_t sib_scale;      // SIB scale field
    uint8_t sib_index;      // SIB index field
    uint8_t sib_base;       // SIB base field

    // Immediate value
    union {
        uint8_t  imm8;
        uint16_t imm16;
        uint32_t imm32;
        uint64_t imm64;
    } immediate;

    // Displacement
    union {
        uint8_t  disp8;
        uint16_t disp16;
        uint32_t disp32;
    } displacement;

    // Flags for instruction properties
    uint32_t flags;

    // For convenience, the full instruction bytes
    uint8_t bytes[16];
} InstructionInfo;

/*
 * Function to disassemble an instruction
 */
unsigned int x86_disasm(const void* code, InstructionInfo* info);
