#include <stdint.h>
#include <stdlib.h>
#include "Windows.h"
#include "disassm.h"
#include "disassm_table_op1.h"
#include "disassm_table_op2.h"
#include "disassm_table_groups.h"
#include "disassm_inst_bytes.h"
#include <string>




/*
 * Helper functions
 */

 // Check if an opcode is valid for the given prefixes
static int is_prefix_valid(uint8_t opcode, uint8_t prefix_flags) {
    if (opcode >= OPCODE_TABLE_SIZE) {
        return 0;
    }

    uint8_t allowed_prefixes = g_prefix_table[opcode];
    return (allowed_prefixes & prefix_flags) != 0;
}

// Check if a lock prefix is valid for this instruction
static int is_lock_valid(uint8_t opcode, uint8_t modrm, uint8_t opcode2, uint8_t mod) {
    // Lock prefix is only valid for memory operands
    if (mod == MODRM_MOD_REGISTER) {
        return 0;
    }

    // Check if the opcode allows lock prefix
    const uint8_t* table;
    size_t table_size;

    if (opcode2) {
        // 2-byte opcode
        table = g_lock_valid_2byte;
        table_size = sizeof(g_lock_valid_2byte) / sizeof(g_lock_valid_2byte[0]);
    }
    else {
        // 1-byte opcode
        table = g_lock_valid_1byte;
        table_size = sizeof(g_lock_valid_1byte) / sizeof(g_lock_valid_1byte[0]);
    }

    for (size_t i = 0; i < table_size; i++) {
        if (table[i] == opcode) {
            return 1;
        }
    }

    return 0;
}

// Check if the operand is valid for this instruction
static int is_operand_valid(uint8_t opcode, uint8_t opcode2, uint8_t modrm_reg, uint8_t mod) {
    // Special cases for certain opcodes
    if (!opcode2) {
        // 1-byte opcodes
        switch (opcode) {
        case 0x8C: // MOV Sreg, r/m
            return modrm_reg <= 5;
        case 0x8E: // MOV r/m, Sreg
            return modrm_reg != 1 && modrm_reg <= 5;
        }
    }
    else {
        // 2-byte opcodes
        switch (opcode) {
        case 0x20: // MOV r32, CRn
        case 0x22: // MOV CRn, r32
            return mod == MODRM_MOD_REGISTER && modrm_reg <= 4 && modrm_reg != 1;
        case 0x21: // MOV r32, DRn
        case 0x23: // MOV DRn, r32
            return mod == MODRM_MOD_REGISTER && modrm_reg != 4 && modrm_reg != 5;
        }
    }

    return 1;
}

// Check if the instruction requires ModR/M operand to be memory
static int requires_memory_operand(uint8_t opcode, uint8_t opcode2) {
    const uint8_t* table;
    size_t table_size;

    if (opcode2) {
        // 2-byte opcode
        table = g_memory_only_2byte;
        table_size = sizeof(g_memory_only_2byte) / sizeof(g_memory_only_2byte[0]);
    }
    else {
        // 1-byte opcode
        table = g_memory_only_1byte;
        table_size = sizeof(g_memory_only_1byte) / sizeof(g_memory_only_1byte[0]);
    }

    for (size_t i = 0; i < table_size; i++) {
        if (table[i] == opcode) {
            return 1;
        }
    }

    return 0;
}

/*
 * Main disassembler function
 */
unsigned int x86_disasm(const void* code, InstructionInfo* info) {
    uint8_t* p = (uint8_t*)code;
    uint8_t* start = p;
    uint8_t c, prefix_flags = 0;
    uint8_t opattr = 0;
    uint8_t disp_size = 0;
    // Clear the output structure
    memset(info, 0, sizeof(InstructionInfo));

    // Step 1: Parse prefixes
    for (int i = 0; i < 16 && p - start < 15; i++) {
        c = *p;
        switch (c) {
        case 0xF0: // LOCK
            info->prefix_lock = c;
            prefix_flags |= PREFIX_LOCK;
            break;
        case 0xF2: // REPNZ/REPNE
            info->prefix_rep = c;
            prefix_flags |= PREFIX_REPNZ;
            break;
        case 0xF3: // REP/REPE/REPZ
            info->prefix_rep = c;
            prefix_flags |= PREFIX_REP;
            break;
        case 0x26: // ES segment
            info->prefix_seg = c;
            prefix_flags |= PREFIX_SEG_ES;
            break;
        case 0x2E: // CS segment
            info->prefix_seg = c;
            prefix_flags |= PREFIX_SEG_CS;
            break;
        case 0x36: // SS segment
            info->prefix_seg = c;
            prefix_flags |= PREFIX_SEG_SS;
            break;
        case 0x3E: // DS segment
            info->prefix_seg = c;
            prefix_flags |= PREFIX_SEG_DS;
            break;
        case 0x64: // FS segment
            info->prefix_seg = c;
            prefix_flags |= PREFIX_SEG_FS;
            break;
        case 0x65: // GS segment
            info->prefix_seg = c;
            prefix_flags |= PREFIX_SEG_GS;
            break;
        case 0x66: // Operand size override
            info->prefix_66 = c;
            prefix_flags |= PREFIX_OP_SIZE;
            break;
        case 0x67: // Address size override
            info->prefix_67 = c;
            prefix_flags |= PREFIX_ADDR_SIZE;
            break;
        default:
            // Not a prefix, move to next step
            goto prefix_end;
        }
        p++;
    }

prefix_end:
    // Update flags with prefix information
    if (prefix_flags & PREFIX_REPNZ) {
        info->flags |= FLAG_PREFIX_REPNZ;
    }
    if (prefix_flags & PREFIX_REP) {
        info->flags |= FLAG_PREFIX_REP;
    }
    if (prefix_flags & PREFIX_OP_SIZE) {
        info->flags |= FLAG_PREFIX_OP_SIZE;
    }
    if (prefix_flags & PREFIX_ADDR_SIZE) {
        info->flags |= FLAG_PREFIX_ADDR_SIZE;
    }
    if (prefix_flags & PREFIX_LOCK) {
        info->flags |= FLAG_PREFIX_LOCK;
    }
    if (prefix_flags & (PREFIX_SEG_CS | PREFIX_SEG_SS | PREFIX_SEG_DS |
        PREFIX_SEG_ES | PREFIX_SEG_FS | PREFIX_SEG_GS)) {
        info->flags |= FLAG_PREFIX_SEG;
    }

    // If no prefixes were found, set the NONE flag
    if (!prefix_flags) {
        prefix_flags = PREFIX_NONE;
    }

    // Step 2: Parse REX prefix (64-bit mode only)
    int has_rex = 0;
    int op64 = 0;

    if (REX_IS_REX(c)) {
        has_rex = 1;
        SET_FLAG(info->flags, FLAG_PREFIX_REX);
        info->rex = c;
        info->rex_w = REX_W(c) ? 1 : 0;
        info->rex_r = REX_R(c) ? 1 : 0;
        info->rex_x = REX_X(c) ? 1 : 0;
        info->rex_b = REX_B(c) ? 1 : 0;

        // Check for 64-bit operand
        if (info->rex_w && (*p & 0xF8) == 0xB8) {
            op64 = 1;
        }

        p++;
        c = *p;

        // Invalid: two REX prefixes
        if (REX_IS_REX(c)) {
            SET_FLAG(info->flags, FLAG_ERROR | FLAG_ERROR_OPCODE);
            info->opcode = c;
            p++;
            goto done;
        }
    }

    // Step 3: Get opcode
    info->opcode = c;
    p++;

    // Check for 2-byte opcodes (0F xx)
    if (c == 0x0F) {
        if (p - start >= 15) {
            info->flags |= FLAG_ERROR | FLAG_ERROR_LENGTH;
            goto done;
        }

        info->opcode2 = *p;
        p++;
    }
    else if (c >= 0xA0 && c <= 0xA3) {
        // Special cases for MOV instructions
        op64 = 1;

        // Adjust prefixes for memory addressing
        if (prefix_flags & PREFIX_ADDR_SIZE) {
            prefix_flags |= PREFIX_OP_SIZE;
        }
        else {
            prefix_flags &= ~PREFIX_OP_SIZE;
        }
    }

    // Step 4: Get opcode attributes
    opattr = 0;
    if (info->opcode2) {
        opattr = g_opcode2_table[info->opcode2];
    }
    else {
        opattr = g_opcode_table[info->opcode];
    }

    // Check for invalid opcode
    if (opattr == OPATTR_ERROR) {
        info->flags |= FLAG_ERROR | FLAG_ERROR_OPCODE;

        // Special case for INT3, INTO, etc.
        if ((info->opcode & ~0x03) == 0xCC) {
            opattr = OPATTR_NONE;
        }
        else {
            goto done;
        }
    }

    // Check for opcode groups (like Grp1, Grp2, etc.)
    if (HAS_ATTR(opattr, OPATTR_GROUP)) {
        uint8_t group_index = g_group_index_table[info->opcode];
        uint8_t group_opattr = g_group_opattr_table[group_index][info->modrm_reg];
        opattr = group_opattr;
    }

    // Step 5: Parse ModR/M byte if present
    disp_size = 0;

    if (HAS_ATTR(opattr, OPATTR_MODRM)) {
        if (p - start >= 15) {
            SET_FLAG(info->flags, FLAG_ERROR | FLAG_ERROR_LENGTH);
            goto done;
        }

        SET_FLAG(info->flags, FLAG_MODRM);
        info->modrm = *p++;
        info->modrm_mod = MODRM_MOD(info->modrm);
        info->modrm_reg = MODRM_REG(info->modrm);
        info->modrm_rm = MODRM_RM(info->modrm);

        // Apply REX extensions
        if (has_rex) {
            if (info->rex_r) {
                info->modrm_reg |= 0x08;  // Apply REX.R extension
            }
            if (info->rex_b) {
                info->modrm_rm |= 0x08;   // Apply REX.B extension
            }
        }

        // Validate operands
        if (!is_operand_valid(info->opcode, info->opcode2, info->modrm_reg, info->modrm_mod)) {
            info->flags |= FLAG_ERROR | FLAG_ERROR_OPERAND;
        }

        // Check if lock prefix is valid
        if ((prefix_flags & PREFIX_LOCK) &&
            !is_lock_valid(info->opcode, info->modrm, info->opcode2, info->modrm_mod)) {
            info->flags |= FLAG_ERROR | FLAG_ERROR_LOCK;
        }

        // Check for memory-only instructions
        if (info->modrm_mod == 3 && requires_memory_operand(info->opcode, info->opcode2)) {
            info->flags |= FLAG_ERROR | FLAG_ERROR_OPERAND;
        }

        // Handle FPU instructions (D8-DF)
        if (!info->opcode2 && info->opcode >= 0xD8 && info->opcode <= 0xDF) {
            uint8_t fpu_index = info->opcode - 0xD8;
            uint8_t fpu_opattr;

            if (info->modrm_mod == 3) {
                fpu_opattr = g_fpu_mod3_table[fpu_index][info->modrm_reg];
            }
            else {
                fpu_opattr = g_fpu_mod01_table[fpu_index];
            }

            if (fpu_opattr == OPATTR_ERROR) {
                info->flags |= FLAG_ERROR | FLAG_ERROR_OPCODE;
            }
        }

        // Process SIB byte if needed
        if (info->modrm_mod != MODRM_MOD_REGISTER && info->modrm_rm == MODRM_RM_SIB) {
            if (p - start >= 15) {
                SET_FLAG(info->flags, FLAG_ERROR | FLAG_ERROR_LENGTH);
                goto done;
            }

            SET_FLAG(info->flags, FLAG_SIB);
            info->sib = *p++;
            info->sib_scale = SIB_SCALE(info->sib);
            info->sib_index = SIB_INDEX(info->sib);
            info->sib_base = SIB_BASE(info->sib);

            // Apply REX extensions
            if (has_rex) {
                if (info->rex_x) {
                    info->sib_index |= 0x08;  // Apply REX.X extension
                }
                if (info->rex_b) {
                    info->sib_base |= 0x08;   // Apply REX.B extension
                }
            }

            // No base or base is EBP/RBP/R13: needs displacement
            if (info->sib_base == SIB_BASE_DISP && info->modrm_mod == MODRM_MOD_INDIRECT) {
                disp_size = 4;
            }
        }

        // Calculate displacement size
        switch (info->modrm_mod) {
        case MODRM_MOD_INDIRECT:
            // No displacement except for special cases
            if (info->modrm_rm == MODRM_RM_DISP32) {
                // [EBP/RBP/R13] or RIP-relative
                disp_size = HAS_PREFIX(prefix_flags, PREFIX_ADDR_SIZE) ? 2 : 4;
            }
            break;

        case MODRM_MOD_DISP8:
            // 8-bit displacement
            disp_size = 1;
            break;

        case MODRM_MOD_DISP32:
            // 16/32-bit displacement
            disp_size = HAS_PREFIX(prefix_flags, PREFIX_ADDR_SIZE) ? 2 : 4;
            break;
        }

        // Process displacement
        if (disp_size > 0) {
            if (p - start + disp_size > 15) {
                SET_FLAG(info->flags, FLAG_ERROR | FLAG_ERROR_LENGTH);
                goto done;
            }

            switch (disp_size) {
            case 1:
                SET_FLAG(info->flags, FLAG_DISP8);
                info->displacement.disp8 = *p;
                break;

            case 2:
                SET_FLAG(info->flags, FLAG_DISP16);
                info->displacement.disp16 = *(uint16_t*)p;
                break;

            case 4:
                SET_FLAG(info->flags, FLAG_DISP32);
                info->displacement.disp32 = *(uint32_t*)p;
                break;
            }

            p += disp_size;
        }
    }
    else if (HAS_PREFIX(prefix_flags, PREFIX_LOCK)) {
        // Lock prefix without ModR/M is invalid
        SET_FLAG(info->flags, FLAG_ERROR | FLAG_ERROR_LOCK);
    }

    // Step 6: Process immediate values
    if (opattr & OPATTR_IMM_P66) {
        // Size depends on prefixes and mode
        if (opattr & OPATTR_REL32) {
            // Relative jump/call
            if (prefix_flags & PREFIX_OP_SIZE) {
                // 16-bit offset
                info->flags |= FLAG_IMM16 | FLAG_RELATIVE;
                info->immediate.imm16 = *(uint16_t*)p;
                p += 2;
            }
            else {
                // 32-bit offset
                info->flags |= FLAG_IMM32 | FLAG_RELATIVE;
                info->immediate.imm32 = *(uint32_t*)p;
                p += 4;
            }
        }
        else {
            // Regular immediate
            if (op64) {
                // 64-bit immediate
                info->flags |= FLAG_IMM64;
                info->immediate.imm64 = *(uint64_t*)p;
                p += 8;
            }
            else if (!(prefix_flags & PREFIX_OP_SIZE)) {
                // 32-bit immediate
                info->flags |= FLAG_IMM32;
                info->immediate.imm32 = *(uint32_t*)p;
                p += 4;
            }
            else {
                // 16-bit immediate
                info->flags |= FLAG_IMM16;
                info->immediate.imm16 = *(uint16_t*)p;
                p += 2;
            }
        }
    }
    else if (opattr & OPATTR_IMM16) {
        // 16-bit immediate
        info->flags |= FLAG_IMM16;
        info->immediate.imm16 = *(uint16_t*)p;
        p += 2;
    }
    else if (opattr & OPATTR_IMM8) {
        // 8-bit immediate
        info->flags |= FLAG_IMM8;
        info->immediate.imm8 = *p++;
    }
    else if (opattr & OPATTR_REL32) {
        // 32-bit relative offset
        info->flags |= FLAG_IMM32 | FLAG_RELATIVE;
        info->immediate.imm32 = *(uint32_t*)p;
        p += 4;
    }
    else if (opattr & OPATTR_REL8) {
        // 8-bit relative offset
        info->flags |= FLAG_IMM8 | FLAG_RELATIVE;
        info->immediate.imm8 = *p++;
    }

done:
    info->length = (uint8_t)(p - start);

    // Check for oversized instructions
    if (info->length > 15) {
        SET_FLAG(info->flags, FLAG_ERROR | FLAG_ERROR_LENGTH);
        info->length = 15;
    }

    // Copy the instruction bytes
    memcpy(info->bytes, start, info->length);

    return info->length;

done:
    // Calculate instruction length
    info->length = (uint8_t)(p - start);

    // Check for oversized instructions
    if (info->length > 15) {
        SET_FLAG(info->flags, FLAG_ERROR | FLAG_ERROR_LENGTH);
        info->length = 15;
    }

    // Copy the instruction bytes
    memcpy(info->bytes, start, info->length);

    return info->length;
}