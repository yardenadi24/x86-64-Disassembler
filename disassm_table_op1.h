#pragma once

#include <stdint.h>
#include "disassm.h"

// Size of opcode tables
#define OPCODE_TABLE_SIZE 256


/*
 * Primary opcode attribute table (1-byte opcodes)
 *
 * Each byte in this table indicates the attributes of the corresponding opcode:
 * - Whether it has a ModR/M byte
 * - What kind of immediate value it has
 * - Whether it's a relative jump/call
 * - Whether it belongs to a group
 */
    static const uint8_t g_opcode_table[OPCODE_TABLE_SIZE] = {
    /* 00 */ OPATTR_MODRM,    /* ADD r/m8, r8 */
    /* 01 */ OPATTR_MODRM,    /* ADD r/m16/32, r16/32 */
    /* 02 */ OPATTR_MODRM,    /* ADD r8, r/m8 */
    /* 03 */ OPATTR_MODRM,    /* ADD r16/32, r/m16/32 */
    /* 04 */ OPATTR_IMM8,     /* ADD AL, imm8 */
    /* 05 */ OPATTR_IMM_P66,  /* ADD AX/EAX, imm16/32 */
    /* 06 */ OPATTR_NONE,     /* PUSH ES (invalid in 64-bit mode) */
    /* 07 */ OPATTR_NONE,     /* POP ES (invalid in 64-bit mode) */
    /* 08 */ OPATTR_MODRM,    /* OR r/m8, r8 */
    /* 09 */ OPATTR_MODRM,    /* OR r/m16/32, r16/32 */
    /* 0A */ OPATTR_MODRM,    /* OR r8, r/m8 */
    /* 0B */ OPATTR_MODRM,    /* OR r16/32, r/m16/32 */
    /* 0C */ OPATTR_IMM8,     /* OR AL, imm8 */
    /* 0D */ OPATTR_IMM_P66,  /* OR AX/EAX, imm16/32 */
    /* 0E */ OPATTR_NONE,     /* PUSH CS (invalid in 64-bit mode) */
    /* 0F */ OPATTR_NONE,     /* Two-byte escape */
    /* 10 */ OPATTR_MODRM,    /* ADC r/m8, r8 */
    /* 11 */ OPATTR_MODRM,    /* ADC r/m16/32, r16/32 */
    /* 12 */ OPATTR_MODRM,    /* ADC r8, r/m8 */
    /* 13 */ OPATTR_MODRM,    /* ADC r16/32, r/m16/32 */
    /* 14 */ OPATTR_IMM8,     /* ADC AL, imm8 */
    /* 15 */ OPATTR_IMM_P66,  /* ADC AX/EAX, imm16/32 */
    /* 16 */ OPATTR_NONE,     /* PUSH SS (invalid in 64-bit mode) */
    /* 17 */ OPATTR_NONE,     /* POP SS (invalid in 64-bit mode) */
    /* 18 */ OPATTR_MODRM,    /* SBB r/m8, r8 */
    /* 19 */ OPATTR_MODRM,    /* SBB r/m16/32, r16/32 */
    /* 1A */ OPATTR_MODRM,    /* SBB r8, r/m8 */
    /* 1B */ OPATTR_MODRM,    /* SBB r16/32, r/m16/32 */
    /* 1C */ OPATTR_IMM8,     /* SBB AL, imm8 */
    /* 1D */ OPATTR_IMM_P66,  /* SBB AX/EAX, imm16/32 */
    /* 1E */ OPATTR_NONE,     /* PUSH DS (invalid in 64-bit mode) */
    /* 1F */ OPATTR_NONE,     /* POP DS (invalid in 64-bit mode) */
    /* 20 */ OPATTR_MODRM,    /* AND r/m8, r8 */
    /* 21 */ OPATTR_MODRM,    /* AND r/m16/32, r16/32 */
    /* 22 */ OPATTR_MODRM,    /* AND r8, r/m8 */
    /* 23 */ OPATTR_MODRM,    /* AND r16/32, r/m16/32 */
    /* 24 */ OPATTR_IMM8,     /* AND AL, imm8 */
    /* 25 */ OPATTR_IMM_P66,  /* AND AX/EAX, imm16/32 */
    /* 26 */ OPATTR_NONE,     /* ES segment override prefix */
    /* 27 */ OPATTR_NONE,     /* DAA (invalid in 64-bit mode) */
    /* 28 */ OPATTR_MODRM,    /* SUB r/m8, r8 */
    /* 29 */ OPATTR_MODRM,    /* SUB r/m16/32, r16/32 */
    /* 2A */ OPATTR_MODRM,    /* SUB r8, r/m8 */
    /* 2B */ OPATTR_MODRM,    /* SUB r16/32, r/m16/32 */
    /* 2C */ OPATTR_IMM8,     /* SUB AL, imm8 */
    /* 2D */ OPATTR_IMM_P66,  /* SUB AX/EAX, imm16/32 */
    /* 2E */ OPATTR_NONE,     /* CS segment override prefix */
    /* 2F */ OPATTR_NONE,     /* DAS (invalid in 64-bit mode) */
    /* 30 */ OPATTR_MODRM,    /* XOR r/m8, r8 */
    /* 31 */ OPATTR_MODRM,    /* XOR r/m16/32, r16/32 */
    /* 32 */ OPATTR_MODRM,    /* XOR r8, r/m8 */
    /* 33 */ OPATTR_MODRM,    /* XOR r16/32, r/m16/32 */
    /* 34 */ OPATTR_IMM8,     /* XOR AL, imm8 */
    /* 35 */ OPATTR_IMM_P66,  /* XOR AX/EAX, imm16/32 */
    /* 36 */ OPATTR_NONE,     /* SS segment override prefix */
    /* 37 */ OPATTR_NONE,     /* AAA (invalid in 64-bit mode) */
    /* 38 */ OPATTR_MODRM,    /* CMP r/m8, r8 */
    /* 39 */ OPATTR_MODRM,    /* CMP r/m16/32, r16/32 */
    /* 3A */ OPATTR_MODRM,    /* CMP r8, r/m8 */
    /* 3B */ OPATTR_MODRM,    /* CMP r16/32, r/m16/32 */
    /* 3C */ OPATTR_IMM8,     /* CMP AL, imm8 */
    /* 3D */ OPATTR_IMM_P66,  /* CMP AX/EAX, imm16/32 */
    /* 3E */ OPATTR_NONE,     /* DS segment override prefix */
    /* 3F */ OPATTR_NONE,     /* AAS (invalid in 64-bit mode) */

    /* 40-4F REX prefixes in 64-bit mode, INC/DEC r16/r32 in 32-bit mode */
    /* 40 */ OPATTR_NONE,     /* REX / INC rAX */
    /* 41 */ OPATTR_NONE,     /* REX.B / INC rCX */
    /* 42 */ OPATTR_NONE,     /* REX.X / INC rDX */
    /* 43 */ OPATTR_NONE,     /* REX.XB / INC rBX */
    /* 44 */ OPATTR_NONE,     /* REX.R / INC rSP */
    /* 45 */ OPATTR_NONE,     /* REX.RB / INC rBP */
    /* 46 */ OPATTR_NONE,     /* REX.RX / INC rSI */
    /* 47 */ OPATTR_NONE,     /* REX.RXB / INC rDI */
    /* 48 */ OPATTR_NONE,     /* REX.W / DEC rAX */
    /* 49 */ OPATTR_NONE,     /* REX.WB / DEC rCX */
    /* 4A */ OPATTR_NONE,     /* REX.WX / DEC rDX */
    /* 4B */ OPATTR_NONE,     /* REX.WXB / DEC rBX */
    /* 4C */ OPATTR_NONE,     /* REX.WR / DEC rSP */
    /* 4D */ OPATTR_NONE,     /* REX.WRB / DEC rBP */
    /* 4E */ OPATTR_NONE,     /* REX.WRX / DEC rSI */
    /* 4F */ OPATTR_NONE,     /* REX.WRXB / DEC rDI */

    /* 50-5F Push/Pop registers */
    /* 50 */ OPATTR_NONE,     /* PUSH rAX/r8 */
    /* 51 */ OPATTR_NONE,     /* PUSH rCX/r9 */
    /* 52 */ OPATTR_NONE,     /* PUSH rDX/r10 */
    /* 53 */ OPATTR_NONE,     /* PUSH rBX/r11 */
    /* 54 */ OPATTR_NONE,     /* PUSH rSP/r12 */
    /* 55 */ OPATTR_NONE,     /* PUSH rBP/r13 */
    /* 56 */ OPATTR_NONE,     /* PUSH rSI/r14 */
    /* 57 */ OPATTR_NONE,     /* PUSH rDI/r15 */
    /* 58 */ OPATTR_NONE,     /* POP rAX/r8 */
    /* 59 */ OPATTR_NONE,     /* POP rCX/r9 */
    /* 5A */ OPATTR_NONE,     /* POP rDX/r10 */
    /* 5B */ OPATTR_NONE,     /* POP rBX/r11 */
    /* 5C */ OPATTR_NONE,     /* POP rSP/r12 */
    /* 5D */ OPATTR_NONE,     /* POP rBP/r13 */
    /* 5E */ OPATTR_NONE,     /* POP rSI/r14 */
    /* 5F */ OPATTR_NONE,     /* POP rDI/r15 */

    /* 60-6F */
    /* 60 */ OPATTR_NONE,     /* PUSHA/PUSHAD (invalid in 64-bit mode) */
    /* 61 */ OPATTR_NONE,     /* POPA/POPAD (invalid in 64-bit mode) */
    /* 62 */ OPATTR_MODRM,    /* BOUND r16/32,m16/32&16/32 (invalid in 64-bit mode) */
    /* 63 */ OPATTR_MODRM,    /* MOVSXD r64,r/m32 (64-bit) / ARPL r/m16,r16 (32-bit) */
    /* 64 */ OPATTR_NONE,     /* FS segment override prefix */
    /* 65 */ OPATTR_NONE,     /* GS segment override prefix */
    /* 66 */ OPATTR_NONE,     /* Operand size override prefix */
    /* 67 */ OPATTR_NONE,     /* Address size override prefix */
    /* 68 */ OPATTR_IMM_P66,  /* PUSH imm16/32 */
    /* 69 */ OPATTR_MODRM | OPATTR_IMM_P66, /* IMUL r16/32,r/m16/32,imm16/32 */
    /* 6A */ OPATTR_IMM8,     /* PUSH imm8 */
    /* 6B */ OPATTR_MODRM | OPATTR_IMM8, /* IMUL r16/32,r/m16/32,imm8 */
    /* 6C */ OPATTR_NONE,     /* INSB */
    /* 6D */ OPATTR_NONE,     /* INSW/INSD */
    /* 6E */ OPATTR_NONE,     /* OUTSB */
    /* 6F */ OPATTR_NONE,     /* OUTSW/OUTSD */

    /* 70-7F Conditional jumps */
    /* 70 */ OPATTR_REL8,     /* JO rel8 */
    /* 71 */ OPATTR_REL8,     /* JNO rel8 */
    /* 72 */ OPATTR_REL8,     /* JB/JNAE/JC rel8 */
    /* 73 */ OPATTR_REL8,     /* JNB/JAE/JNC rel8 */
    /* 74 */ OPATTR_REL8,     /* JZ/JE rel8 */
    /* 75 */ OPATTR_REL8,     /* JNZ/JNE rel8 */
    /* 76 */ OPATTR_REL8,     /* JBE/JNA rel8 */
    /* 77 */ OPATTR_REL8,     /* JNBE/JA rel8 */
    /* 78 */ OPATTR_REL8,     /* JS rel8 */
    /* 79 */ OPATTR_REL8,     /* JNS rel8 */
    /* 7A */ OPATTR_REL8,     /* JP/JPE rel8 */
    /* 7B */ OPATTR_REL8,     /* JNP/JPO rel8 */
    /* 7C */ OPATTR_REL8,     /* JL/JNGE rel8 */
    /* 7D */ OPATTR_REL8,     /* JNL/JGE rel8 */
    /* 7E */ OPATTR_REL8,     /* JLE/JNG rel8 */
    /* 7F */ OPATTR_REL8,     /* JNLE/JG rel8 */

    /* 80-8F */
    /* 80 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM8, /* Group 1: ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m8, imm8 */
    /* 81 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM_P66, /* Group 1: ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m16/32, imm16/32 */
    /* 82 */ OPATTR_ERROR,    /* Invalid in 64-bit mode */
    /* 83 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM8, /* Group 1: ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m16/32, imm8 */
    /* 84 */ OPATTR_MODRM,    /* TEST r/m8, r8 */
    /* 85 */ OPATTR_MODRM,    /* TEST r/m16/32, r16/32 */
    /* 86 */ OPATTR_MODRM,    /* XCHG r/m8, r8 */
    /* 87 */ OPATTR_MODRM,    /* XCHG r/m16/32, r16/32 */
    /* 88 */ OPATTR_MODRM,    /* MOV r/m8, r8 */
    /* 89 */ OPATTR_MODRM,    /* MOV r/m16/32, r16/32 */
    /* 8A */ OPATTR_MODRM,    /* MOV r8, r/m8 */
    /* 8B */ OPATTR_MODRM,    /* MOV r16/32, r/m16/32 */
    /* 8C */ OPATTR_MODRM,    /* MOV r/m16, Sreg */
    /* 8D */ OPATTR_MODRM,    /* LEA r16/32, m */
    /* 8E */ OPATTR_MODRM,    /* MOV Sreg, r/m16 */
    /* 8F */ OPATTR_MODRM | OPATTR_GROUP, /* Group 1A: POP r/m16/32/64 */

    /* 90-9F */
    /* 90 */ OPATTR_NONE,     /* NOP / XCHG rAX, rAX */
    /* 91 */ OPATTR_NONE,     /* XCHG rAX, rCX */
    /* 92 */ OPATTR_NONE,     /* XCHG rAX, rDX */
    /* 93 */ OPATTR_NONE,     /* XCHG rAX, rBX */
    /* 94 */ OPATTR_NONE,     /* XCHG rAX, rSP */
    /* 95 */ OPATTR_NONE,     /* XCHG rAX, rBP */
    /* 96 */ OPATTR_NONE,     /* XCHG rAX, rSI */
    /* 97 */ OPATTR_NONE,     /* XCHG rAX, rDI */
    /* 98 */ OPATTR_NONE,     /* CBW/CWDE/CDQE */
    /* 99 */ OPATTR_NONE,     /* CWD/CDQ/CQO */
    /* 9A */ OPATTR_NONE,     /* CALL far (invalid in 64-bit mode) */
    /* 9B */ OPATTR_NONE,     /* FWAIT/WAIT */
    /* 9C */ OPATTR_NONE,     /* PUSHF/PUSHFD/PUSHFQ */
    /* 9D */ OPATTR_NONE,     /* POPF/POPFD/POPFQ */
    /* 9E */ OPATTR_NONE,     /* SAHF */
    /* 9F */ OPATTR_NONE,     /* LAHF */

    /* A0-AF */
    /* A0 */ OPATTR_NONE,     /* MOV AL, moffs8 */
    /* A1 */ OPATTR_NONE,     /* MOV AX/EAX/RAX, moffs16/32/64 */
    /* A2 */ OPATTR_NONE,     /* MOV moffs8, AL */
    /* A3 */ OPATTR_NONE,     /* MOV moffs16/32/64, AX/EAX/RAX */
    /* A4 */ OPATTR_NONE,     /* MOVSB */
    /* A5 */ OPATTR_NONE,     /* MOVSW/MOVSD/MOVSQ */
    /* A6 */ OPATTR_NONE,     /* CMPSB */
    /* A7 */ OPATTR_NONE,     /* CMPSW/CMPSD/CMPSQ */
    /* A8 */ OPATTR_IMM8,     /* TEST AL, imm8 */
    /* A9 */ OPATTR_IMM_P66,  /* TEST AX/EAX/RAX, imm16/32 */
    /* AA */ OPATTR_NONE,     /* STOSB */
    /* AB */ OPATTR_NONE,     /* STOSW/STOSD/STOSQ */
    /* AC */ OPATTR_NONE,     /* LODSB */
    /* AD */ OPATTR_NONE,     /* LODSW/LODSD/LODSQ */
    /* AE */ OPATTR_NONE,     /* SCASB */
    /* AF */ OPATTR_NONE,     /* SCASW/SCASD/SCASQ */

    /* B0-BF */
    /* B0 */ OPATTR_IMM8,     /* MOV r8L, imm8 */
    /* B1 */ OPATTR_IMM8,     /* MOV r8H/r9L, imm8 */
    /* B2 */ OPATTR_IMM8,     /* MOV r8H/r10L, imm8 */
    /* B3 */ OPATTR_IMM8,     /* MOV r8H/r11L, imm8 */
    /* B4 */ OPATTR_IMM8,     /* MOV r8H/r12L, imm8 */
    /* B5 */ OPATTR_IMM8,     /* MOV r8H/r13L, imm8 */
    /* B6 */ OPATTR_IMM8,     /* MOV r8H/r14L, imm8 */
    /* B7 */ OPATTR_IMM8,     /* MOV r8H/r15L, imm8 */
    /* B8 */ OPATTR_IMM_P66,  /* MOV r16/32/64, imm16/32/64 */
    /* B9 */ OPATTR_IMM_P66,  /* MOV r16/32/64, imm16/32/64 */
    /* BA */ OPATTR_IMM_P66,  /* MOV r16/32/64, imm16/32/64 */
    /* BB */ OPATTR_IMM_P66,  /* MOV r16/32/64, imm16/32/64 */
    /* BC */ OPATTR_IMM_P66,  /* MOV r16/32/64, imm16/32/64 */
    /* BD */ OPATTR_IMM_P66,  /* MOV r16/32/64, imm16/32/64 */
    /* BE */ OPATTR_IMM_P66,  /* MOV r16/32/64, imm16/32/64 */
    /* BF */ OPATTR_IMM_P66,  /* MOV r16/32/64, imm16/32/64 */

    /* C0-CF */
    /* C0 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM8, /* Group 2: ROL/ROR/RCL/RCR/SHL/SAL/SHR/SAR r/m8, imm8 */
    /* C1 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM8, /* Group 2: ROL/ROR/RCL/RCR/SHL/SAL/SHR/SAR r/m16/32, imm8 */
    /* C2 */ OPATTR_IMM16,    /* RET imm16 */
    /* C3 */ OPATTR_NONE,     /* RET */
    /* C4 */ OPATTR_MODRM,    /* LES r16/32, m16:16/32 (invalid in 64-bit mode) / VEX 3-byte prefix */
    /* C5 */ OPATTR_MODRM,    /* LDS r16/32, m16:16/32 (invalid in 64-bit mode) / VEX 2-byte prefix */
    /* C6 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM8, /* Group 11: MOV r/m8, imm8 */
    /* C7 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM_P66, /* Group 11: MOV r/m16/32/64, imm16/32 */
    /* C8 */ OPATTR_IMM16 | OPATTR_IMM8, /* ENTER imm16, imm8 */
    /* C9 */ OPATTR_NONE,     /* LEAVE */
    /* CA */ OPATTR_IMM16,    /* RET FAR imm16 */
    /* CB */ OPATTR_NONE,     /* RET FAR */
    /* CC */ OPATTR_NONE,     /* INT 3 */
    /* CD */ OPATTR_IMM8,     /* INT imm8 */
    /* CE */ OPATTR_NONE,     /* INTO (invalid in 64-bit mode) */
    /* CF */ OPATTR_NONE,     /* IRET/IRETD/IRETQ */

    /* D0-DF */
    /* D0 */ OPATTR_MODRM | OPATTR_GROUP, /* Group 2: ROL/ROR/RCL/RCR/SHL/SAL/SHR/SAR r/m8, 1 */
    /* D1 */ OPATTR_MODRM | OPATTR_GROUP, /* Group 2: ROL/ROR/RCL/RCR/SHL/SAL/SHR/SAR r/m16/32, 1 */
    /* D2 */ OPATTR_MODRM | OPATTR_GROUP, /* Group 2: ROL/ROR/RCL/RCR/SHL/SAL/SHR/SAR r/m8, CL */
    /* D3 */ OPATTR_MODRM | OPATTR_GROUP, /* Group 2: ROL/ROR/RCL/RCR/SHL/SAL/SHR/SAR r/m16/32, CL */
    /* D4 */ OPATTR_IMM8,     /* AAM imm8 (invalid in 64-bit mode) */
    /* D5 */ OPATTR_IMM8,     /* AAD imm8 (invalid in 64-bit mode) */
    /* D6 */ OPATTR_NONE,     /* Reserved */
    /* D7 */ OPATTR_NONE,     /* XLAT/XLATB */
    /* D8 */ OPATTR_MODRM,    /* ESC (FPU instructions) */
    /* D9 */ OPATTR_MODRM,    /* ESC (FPU instructions) */
    /* DA */ OPATTR_MODRM,    /* ESC (FPU instructions) */
    /* DB */ OPATTR_MODRM,    /* ESC (FPU instructions) */
    /* DC */ OPATTR_MODRM,    /* ESC (FPU instructions) */
    /* DD */ OPATTR_MODRM,    /* ESC (FPU instructions) */
    /* DE */ OPATTR_MODRM,    /* ESC (FPU instructions) */
    /* DF */ OPATTR_MODRM,    /* ESC (FPU instructions) */

    /* E0-EF */
    /* E0 */ OPATTR_REL8,     /* LOOPNE/LOOPNZ rel8 */
    /* E1 */ OPATTR_REL8,     /* LOOPE/LOOPZ rel8 */
    /* E2 */ OPATTR_REL8,     /* LOOP rel8 */
    /* E3 */ OPATTR_REL8,     /* JCXZ/JECXZ/JRCXZ rel8 */
    /* E4 */ OPATTR_IMM8,     /* IN AL, imm8 */
    /* E5 */ OPATTR_IMM8,     /* IN AX/EAX, imm8 */
    /* E6 */ OPATTR_IMM8,     /* OUT imm8, AL */
    /* E7 */ OPATTR_IMM8,     /* OUT imm8, AX/EAX */
    /* E8 */ OPATTR_REL32,    /* CALL rel16/32 */
    /* E9 */ OPATTR_REL32,    /* JMP rel16/32 */
    /* EA */ OPATTR_NONE,     /* JMP far (invalid in 64-bit mode) */
    /* EB */ OPATTR_REL8,     /* JMP rel8 */
    /* EC */ OPATTR_NONE,     /* IN AL, DX */
    /* ED */ OPATTR_NONE,     /* IN AX/EAX, DX */
    /* EE */ OPATTR_NONE,     /* OUT DX, AL */
    /* EF */ OPATTR_NONE,     /* OUT DX, AX/EAX */

    /* F0-FF */
    /* F0 */ OPATTR_NONE,     /* LOCK prefix */
    /* F1 */ OPATTR_NONE,     /* Reserved (generates #UD) */
    /* F2 */ OPATTR_NONE,     /* REPNE/REPNZ prefix */
    /* F3 */ OPATTR_NONE,     /* REP/REPE/REPZ prefix */
    /* F4 */ OPATTR_NONE,     /* HLT */
    /* F5 */ OPATTR_NONE,     /* CMC */
    /* F6 */ OPATTR_MODRM | OPATTR_GROUP, /* Group 3: TEST/???/NOT/NEG/MUL/IMUL/DIV/IDIV r/m8 */
    /* F7 */ OPATTR_MODRM | OPATTR_GROUP, /* Group 3: TEST/???/NOT/NEG/MUL/IMUL/DIV/IDIV r/m16/32/64 */
    /* F8 */ OPATTR_NONE,     /* CLC */
    /* F9 */ OPATTR_NONE,     /* STC */
    /* FA */ OPATTR_NONE,     /* CLI */
    /* FB */ OPATTR_NONE,     /* STI */
    /* FC */ OPATTR_NONE,     /* CLD */
    /* FD */ OPATTR_NONE,     /* STD */
    /* FE */ OPATTR_MODRM | OPATTR_GROUP, /* Group 4: INC/DEC r/m8 */
    /* FF */ OPATTR_MODRM | OPATTR_GROUP, /* Group 5: INC/DEC/CALL/CALL far/JMP/JMP far/PUSH r/m16/32/64 */
};

