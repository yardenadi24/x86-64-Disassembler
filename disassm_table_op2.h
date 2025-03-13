#pragma once
#include "disassm.h"

// Size of opcode tables
#define OPCODE_TABLE_SIZE 256

/*
 * Secondary opcode attribute table (2-byte opcodes 0F xx)
 *
 * Each byte indicates the attributes of the corresponding 0F-prefixed opcode:
 * - Whether it has a ModR/M byte
 * - What kind of immediate value it has
 * - Whether it's a relative jump/call
 * - Whether it belongs to a group
 */
static const uint8_t g_opcode2_table[OPCODE_TABLE_SIZE] = {
    /* 00 */ OPATTR_MODRM | OPATTR_GROUP,    /* Group 6: SLDT/STR/LLDT/LTR/VERR/VERW */
    /* 01 */ OPATTR_MODRM | OPATTR_GROUP,    /* Group 7: SGDT/SIDT/LGDT/LIDT/SMSW/LMSW/INVLPG/SWAPGS */
    /* 02 */ OPATTR_MODRM,    /* LAR r16/32/64, r/m16/32 */
    /* 03 */ OPATTR_MODRM,    /* LSL r16/32/64, r/m16/32 */
    /* 04 */ OPATTR_ERROR,    /* Invalid */
    /* 05 */ OPATTR_NONE,     /* SYSCALL */
    /* 06 */ OPATTR_NONE,     /* CLTS */
    /* 07 */ OPATTR_NONE,     /* SYSRET */
    /* 08 */ OPATTR_NONE,     /* INVD */
    /* 09 */ OPATTR_NONE,     /* WBINVD */
    /* 0A */ OPATTR_ERROR,    /* Invalid */
    /* 0B */ OPATTR_NONE,     /* UD2 */
    /* 0C */ OPATTR_ERROR,    /* Invalid */
    /* 0D */ OPATTR_MODRM,    /* Group P: prefetch */
    /* 0E */ OPATTR_NONE,     /* FEMMS */
    /* 0F */ OPATTR_MODRM,    /* 3DNow! escape */

    /* 10 */ OPATTR_MODRM,    /* MOVUPS/MOVSS/MOVUPD/MOVSD */
    /* 11 */ OPATTR_MODRM,    /* MOVUPS/MOVSS/MOVUPD/MOVSD */
    /* 12 */ OPATTR_MODRM,    /* MOVLPS/MOVHLPS/MOVLPD/MOVSLDUP/MOVDDUP */
    /* 13 */ OPATTR_MODRM,    /* MOVLPS/MOVLPD */
    /* 14 */ OPATTR_MODRM,    /* UNPCKLPS/UNPCKLPD */
    /* 15 */ OPATTR_MODRM,    /* UNPCKHPS/UNPCKHPD */
    /* 16 */ OPATTR_MODRM,    /* MOVHPS/MOVSHDUP/MOVHPD */
    /* 17 */ OPATTR_MODRM,    /* MOVHPS/MOVHPD */
    /* 18 */ OPATTR_MODRM | OPATTR_GROUP,    /* Group 16: prefetch/nop/reserved */
    /* 19 */ OPATTR_MODRM,    /* NOP r/m16/32 */
    /* 1A */ OPATTR_MODRM,    /* NOP r/m16/32 */
    /* 1B */ OPATTR_MODRM,    /* NOP r/m16/32 */
    /* 1C */ OPATTR_MODRM,    /* NOP r/m16/32 */
    /* 1D */ OPATTR_MODRM,    /* NOP r/m16/32 */
    /* 1E */ OPATTR_MODRM,    /* NOP r/m16/32 */
    /* 1F */ OPATTR_MODRM,    /* NOP r/m16/32 */

    /* 20 */ OPATTR_MODRM,    /* MOV r/m32, CR0-CR7 */
    /* 21 */ OPATTR_MODRM,    /* MOV r/m32, DR0-DR7 */
    /* 22 */ OPATTR_MODRM,    /* MOV CR0-CR7, r/m32 */
    /* 23 */ OPATTR_MODRM,    /* MOV DR0-DR7, r/m32 */
    /* 24 */ OPATTR_ERROR,    /* Invalid */
    /* 25 */ OPATTR_ERROR,    /* Invalid */
    /* 26 */ OPATTR_ERROR,    /* Invalid */
    /* 27 */ OPATTR_ERROR,    /* Invalid */
    /* 28 */ OPATTR_MODRM,    /* MOVAPS/MOVAPD */
    /* 29 */ OPATTR_MODRM,    /* MOVAPS/MOVAPD */
    /* 2A */ OPATTR_MODRM,    /* CVTPI2PS/CVTSI2SS/CVTPI2PD/CVTSI2SD */
    /* 2B */ OPATTR_MODRM,    /* MOVNTPS/MOVNTPD */
    /* 2C */ OPATTR_MODRM,    /* CVTTPS2PI/CVTTSS2SI/CVTTPD2PI/CVTTSD2SI */
    /* 2D */ OPATTR_MODRM,    /* CVTPS2PI/CVTSS2SI/CVTPD2PI/CVTSD2SI */
    /* 2E */ OPATTR_MODRM,    /* UCOMISS/UCOMISD */
    /* 2F */ OPATTR_MODRM,    /* COMISS/COMISD */

    /* 30 */ OPATTR_NONE,     /* WRMSR */
    /* 31 */ OPATTR_NONE,     /* RDTSC */
    /* 32 */ OPATTR_NONE,     /* RDMSR */
    /* 33 */ OPATTR_NONE,     /* RDPMC */
    /* 34 */ OPATTR_NONE,     /* SYSENTER */
    /* 35 */ OPATTR_NONE,     /* SYSEXIT */
    /* 36 */ OPATTR_ERROR,    /* Invalid */
    /* 37 */ OPATTR_NONE,     /* GETSEC */
    /* 38 */ OPATTR_MODRM,    /* SSE3 escape */
    /* 39 */ OPATTR_ERROR,    /* Reserved */
    /* 3A */ OPATTR_MODRM,    /* SSE3 escape */
    /* 3B */ OPATTR_ERROR,    /* Reserved */
    /* 3C */ OPATTR_ERROR,    /* Reserved */
    /* 3D */ OPATTR_ERROR,    /* Reserved */
    /* 3E */ OPATTR_ERROR,    /* Reserved */
    /* 3F */ OPATTR_ERROR,    /* Reserved */

    /* 40 */ OPATTR_MODRM,    /* CMOVO r16/32/64, r/m16/32/64 */
    /* 41 */ OPATTR_MODRM,    /* CMOVNO r16/32/64, r/m16/32/64 */
    /* 42 */ OPATTR_MODRM,    /* CMOVB/CMOVC/CMOVNAE r16/32/64, r/m16/32/64 */
    /* 43 */ OPATTR_MODRM,    /* CMOVAE/CMOVNB/CMOVNC r16/32/64, r/m16/32/64 */
    /* 44 */ OPATTR_MODRM,    /* CMOVE/CMOVZ r16/32/64, r/m16/32/64 */
    /* 45 */ OPATTR_MODRM,    /* CMOVNE/CMOVNZ r16/32/64, r/m16/32/64 */
    /* 46 */ OPATTR_MODRM,    /* CMOVBE/CMOVNA r16/32/64, r/m16/32/64 */
    /* 47 */ OPATTR_MODRM,    /* CMOVA/CMOVNBE r16/32/64, r/m16/32/64 */
    /* 48 */ OPATTR_MODRM,    /* CMOVS r16/32/64, r/m16/32/64 */
    /* 49 */ OPATTR_MODRM,    /* CMOVNS r16/32/64, r/m16/32/64 */
    /* 4A */ OPATTR_MODRM,    /* CMOVP/CMOVPE r16/32/64, r/m16/32/64 */
    /* 4B */ OPATTR_MODRM,    /* CMOVNP/CMOVPO r16/32/64, r/m16/32/64 */
    /* 4C */ OPATTR_MODRM,    /* CMOVL/CMOVNGE r16/32/64, r/m16/32/64 */
    /* 4D */ OPATTR_MODRM,    /* CMOVGE/CMOVNL r16/32/64, r/m16/32/64 */
    /* 4E */ OPATTR_MODRM,    /* CMOVLE/CMOVNG r16/32/64, r/m16/32/64 */
    /* 4F */ OPATTR_MODRM,    /* CMOVG/CMOVNLE r16/32/64, r/m16/32/64 */

    /* 50 */ OPATTR_MODRM,    /* MOVMSKPS/MOVMSKPD */
    /* 51 */ OPATTR_MODRM,    /* SQRTPS/SQRTSS/SQRTPD/SQRTSD */
    /* 52 */ OPATTR_MODRM,    /* RSQRTPS/RSQRTSS */
    /* 53 */ OPATTR_MODRM,    /* RCPPS/RCPSS */
    /* 54 */ OPATTR_MODRM,    /* ANDPS/ANDPD */
    /* 55 */ OPATTR_MODRM,    /* ANDNPS/ANDNPD */
    /* 56 */ OPATTR_MODRM,    /* ORPS/ORPD */
    /* 57 */ OPATTR_MODRM,    /* XORPS/XORPD */
    /* 58 */ OPATTR_MODRM,    /* ADDPS/ADDSS/ADDPD/ADDSD */
    /* 59 */ OPATTR_MODRM,    /* MULPS/MULSS/MULPD/MULSD */
    /* 5A */ OPATTR_MODRM,    /* CVTPS2PD/CVTSS2SD/CVTPD2PS/CVTSD2SS */
    /* 5B */ OPATTR_MODRM,    /* CVTDQ2PS/CVTPS2DQ/CVTTPD2DQ */
    /* 5C */ OPATTR_MODRM,    /* SUBPS/SUBSS/SUBPD/SUBSD */
    /* 5D */ OPATTR_MODRM,    /* MINPS/MINSS/MINPD/MINSD */
    /* 5E */ OPATTR_MODRM,    /* DIVPS/DIVSS/DIVPD/DIVSD */
    /* 5F */ OPATTR_MODRM,    /* MAXPS/MAXSS/MAXPD/MAXSD */

    /* 60 */ OPATTR_MODRM,    /* PUNPCKLBW */
    /* 61 */ OPATTR_MODRM,    /* PUNPCKLWD */
    /* 62 */ OPATTR_MODRM,    /* PUNPCKLDQ */
    /* 63 */ OPATTR_MODRM,    /* PACKSSWB */
    /* 64 */ OPATTR_MODRM,    /* PCMPGTB */
    /* 65 */ OPATTR_MODRM,    /* PCMPGTW */
    /* 66 */ OPATTR_MODRM,    /* PCMPGTD */
    /* 67 */ OPATTR_MODRM,    /* PACKUSWB */
    /* 68 */ OPATTR_MODRM,    /* PUNPCKHBW */
    /* 69 */ OPATTR_MODRM,    /* PUNPCKHWD */
    /* 6A */ OPATTR_MODRM,    /* PUNPCKHDQ */
    /* 6B */ OPATTR_MODRM,    /* PACKSSDW */
    /* 6C */ OPATTR_MODRM,    /* PUNPCKLQDQ */
    /* 6D */ OPATTR_MODRM,    /* PUNPCKHQDQ */
    /* 6E */ OPATTR_MODRM,    /* MOVD/MOVQ */
    /* 6F */ OPATTR_MODRM,    /* MOVQ/MOVDQA/MOVDQU */

    /* 70 */ OPATTR_MODRM | OPATTR_IMM8, /* PSHUFW/PSHUFD/PSHUFHW/PSHUFLW */
    /* 71 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM8, /* Group 12: PSRLW/PSRAW/PSLLW */
    /* 72 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM8, /* Group 13: PSRLD/PSRAD/PSLLD */
    /* 73 */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM8, /* Group 14: PSRLQ/PSRLDQ/PSLLQ/PSLLDQ */
    /* 74 */ OPATTR_MODRM,    /* PCMPEQB */
    /* 75 */ OPATTR_MODRM,    /* PCMPEQW */
    /* 76 */ OPATTR_MODRM,    /* PCMPEQD */
    /* 77 */ OPATTR_NONE,     /* EMMS */
    /* 78 */ OPATTR_MODRM,    /* VMREAD */
    /* 79 */ OPATTR_MODRM,    /* VMWRITE */
    /* 7A */ OPATTR_ERROR,    /* Reserved */
    /* 7B */ OPATTR_ERROR,    /* Reserved */
    /* 7C */ OPATTR_MODRM,    /* HADDPD/HADDPS */
    /* 7D */ OPATTR_MODRM,    /* HSUBPD/HSUBPS */
    /* 7E */ OPATTR_MODRM,    /* MOVD/MOVQ */
    /* 7F */ OPATTR_MODRM,    /* MOVQ/MOVDQA/MOVDQU */

    /* 80 */ OPATTR_REL32,    /* JO rel16/32 */
    /* 81 */ OPATTR_REL32,    /* JNO rel16/32 */
    /* 82 */ OPATTR_REL32,    /* JB/JNAE/JC rel16/32 */
    /* 83 */ OPATTR_REL32,    /* JNB/JAE/JNC rel16/32 */
    /* 84 */ OPATTR_REL32,    /* JZ/JE rel16/32 */
    /* 85 */ OPATTR_REL32,    /* JNZ/JNE rel16/32 */
    /* 86 */ OPATTR_REL32,    /* JBE/JNA rel16/32 */
    /* 87 */ OPATTR_REL32,    /* JNBE/JA rel16/32 */
    /* 88 */ OPATTR_REL32,    /* JS rel16/32 */
    /* 89 */ OPATTR_REL32,    /* JNS rel16/32 */
    /* 8A */ OPATTR_REL32,    /* JP/JPE rel16/32 */
    /* 8B */ OPATTR_REL32,    /* JNP/JPO rel16/32 */
    /* 8C */ OPATTR_REL32,    /* JL/JNGE rel16/32 */
    /* 8D */ OPATTR_REL32,    /* JNL/JGE rel16/32 */
    /* 8E */ OPATTR_REL32,    /* JLE/JNG rel16/32 */
    /* 8F */ OPATTR_REL32,    /* JNLE/JG rel16/32 */

    /* 90 */ OPATTR_MODRM,    /* SETO r/m8 */
    /* 91 */ OPATTR_MODRM,    /* SETNO r/m8 */
    /* 92 */ OPATTR_MODRM,    /* SETB/SETNAE/SETC r/m8 */
    /* 93 */ OPATTR_MODRM,    /* SETNB/SETAE/SETNC r/m8 */
    /* 94 */ OPATTR_MODRM,    /* SETZ/SETE r/m8 */
    /* 95 */ OPATTR_MODRM,    /* SETNZ/SETNE r/m8 */
    /* 96 */ OPATTR_MODRM,    /* SETBE/SETNA r/m8 */
    /* 97 */ OPATTR_MODRM,    /* SETNBE/SETA r/m8 */
    /* 98 */ OPATTR_MODRM,    /* SETS r/m8 */
    /* 99 */ OPATTR_MODRM,    /* SETNS r/m8 */
    /* 9A */ OPATTR_MODRM,    /* SETP/SETPE r/m8 */
    /* 9B */ OPATTR_MODRM,    /* SETNP/SETPO r/m8 */
    /* 9C */ OPATTR_MODRM,    /* SETL/SETNGE r/m8 */
    /* 9D */ OPATTR_MODRM,    /* SETNL/SETGE r/m8 */
    /* 9E */ OPATTR_MODRM,    /* SETLE/SETNG r/m8 */
    /* 9F */ OPATTR_MODRM,    /* SETNLE/SETG r/m8 */

    /* A0 */ OPATTR_NONE,     /* PUSH FS */
    /* A1 */ OPATTR_NONE,     /* POP FS */
    /* A2 */ OPATTR_NONE,     /* CPUID */
    /* A3 */ OPATTR_MODRM,    /* BT r/m16/32/64, r16/32/64 */
    /* A4 */ OPATTR_MODRM | OPATTR_IMM8, /* SHLD r/m16/32/64, r16/32/64, imm8 */
    /* A5 */ OPATTR_MODRM,    /* SHLD r/m16/32/64, r16/32/64, CL */
    /* A6 */ OPATTR_ERROR,    /* Reserved */
    /* A7 */ OPATTR_ERROR,    /* Reserved */
    /* A8 */ OPATTR_NONE,     /* PUSH GS */
    /* A9 */ OPATTR_NONE,     /* POP GS */
    /* AA */ OPATTR_NONE,     /* RSM */
    /* AB */ OPATTR_MODRM,    /* BTS r/m16/32/64, r16/32/64 */
    /* AC */ OPATTR_MODRM | OPATTR_IMM8, /* SHRD r/m16/32/64, r16/32/64, imm8 */
    /* AD */ OPATTR_MODRM,    /* SHRD r/m16/32/64, r16/32/64, CL */
    /* AE */ OPATTR_MODRM | OPATTR_GROUP, /* Group 15: FXSAVE/FXRSTOR/LDMXCSR/STMXCSR/XSAVE/XRSTOR/CLFLUSH... */
    /* AF */ OPATTR_MODRM,    /* IMUL r16/32/64, r/m16/32/64 */

    // ... continuing for opcodes 0F B0-FF ...

    /* B0 */ OPATTR_MODRM,    /* CMPXCHG r/m8, r8 */
    /* B1 */ OPATTR_MODRM,    /* CMPXCHG r/m16/32/64, r16/32/64 */
    /* B2 */ OPATTR_MODRM,    /* LSS r16/32/64, m16:16/32/64 */
    /* B3 */ OPATTR_MODRM,    /* BTR r/m16/32/64, r16/32/64 */
    /* B4 */ OPATTR_MODRM,    /* LFS r16/32/64, m16:16/32/64 */
    /* B5 */ OPATTR_MODRM,    /* LGS r16/32/64, m16:16/32/64 */
    /* B6 */ OPATTR_MODRM,    /* MOVZX r16/32/64, r/m8 */
    /* B7 */ OPATTR_MODRM,    /* MOVZX r16/32/64, r/m16 */
    /* B8 */ OPATTR_ERROR,    /* JMPE (IA-64) or POPCNT */
    /* B9 */ OPATTR_MODRM | OPATTR_GROUP, /* Group 10: UD1/UD2/POPCNT */
    /* BA */ OPATTR_MODRM | OPATTR_GROUP | OPATTR_IMM8, /* Group 8: BT/BTS/BTR/BTC r/m16/32/64, imm8 */
    /* BB */ OPATTR_MODRM,    /* BTC r/m16/32/64, r16/32/64 */
    /* BC */ OPATTR_MODRM,    /* BSF r16/32/64, r/m16/32/64 */
    /* BD */ OPATTR_MODRM,    /* BSR r16/32/64, r/m16/32/64 */
    /* BE */ OPATTR_MODRM,    /* MOVSX r16/32/64, r/m8 */
    /* BF */ OPATTR_MODRM,    /* MOVSX r16/32/64, r/m16 */

    /* C0 */ OPATTR_MODRM,    /* XADD r/m8, r8 */
    /* C1 */ OPATTR_MODRM,    /* XADD r/m16/32/64, r16/32/64 */
    /* C2 */ OPATTR_MODRM | OPATTR_IMM8, /* CMPPS/CMPSS/CMPPD/CMPSD xmm, r/m128, imm8 */
    /* C3 */ OPATTR_MODRM,    /* MOVNTI m32/64, r32/64 */
    /* C4 */ OPATTR_MODRM | OPATTR_IMM8, /* PINSRW mm/xmm, r32/m16, imm8 */
    /* C5 */ OPATTR_MODRM | OPATTR_IMM8, /* PEXTRW r32, mm/xmm, imm8 */
    /* C6 */ OPATTR_MODRM | OPATTR_IMM8, /* SHUFPS/SHUFPD xmm, r/m128, imm8 */
    /* C7 */ OPATTR_MODRM | OPATTR_GROUP, /* Group 9: CMPXCHG8B/CMPXCHG16B/VMPTRLD/VMCLEAR/VMXON... */
    /* C8 */ OPATTR_NONE,     /* BSWAP EAX/RAX */
    /* C9 */ OPATTR_NONE,     /* BSWAP ECX/RCX */
    /* CA */ OPATTR_NONE,     /* BSWAP EDX/RDX */
    /* CB */ OPATTR_NONE,     /* BSWAP EBX/RBX */
    /* CC */ OPATTR_NONE,     /* BSWAP ESP/RSP */
    /* CD */ OPATTR_NONE,     /* BSWAP EBP/RBP */
    /* CE */ OPATTR_NONE,     /* BSWAP ESI/RSI */
    /* CF */ OPATTR_NONE,     /* BSWAP EDI/RDI */

    /* D0 */ OPATTR_MODRM,    /* ADDSUBPD/ADDSUBPS */
    /* D1 */ OPATTR_MODRM,    /* PSRLW mm, mm/m64 */
    /* D2 */ OPATTR_MODRM,    /* PSRLD mm, mm/m64 */
    /* D3 */ OPATTR_MODRM,    /* PSRLQ mm, mm/m64 */
    /* D4 */ OPATTR_MODRM,    /* PADDQ mm, mm/m64 */
    /* D5 */ OPATTR_MODRM,    /* PMULLW mm, mm/m64 */
    /* D6 */ OPATTR_MODRM,    /* MOVQ2DQ/MOVQ/MOVDQ2Q */
    /* D7 */ OPATTR_MODRM,    /* PMOVMSKB r32, mm/xmm */
    /* D8 */ OPATTR_MODRM,    /* PSUBUSB mm, mm/m64 */
    /* D9 */ OPATTR_MODRM,    /* PSUBUSW mm, mm/m64 */
    /* DA */ OPATTR_MODRM,    /* PMINUB mm, mm/m64 */
    /* DB */ OPATTR_MODRM,    /* PAND mm, mm/m64 */
    /* DC */ OPATTR_MODRM,    /* PADDUSB mm, mm/m64 */
    /* DD */ OPATTR_MODRM,    /* PADDUSW mm, mm/m64 */
    /* DE */ OPATTR_MODRM,    /* PMAXUB mm, mm/m64 */
    /* DF */ OPATTR_MODRM,    /* PANDN mm, mm/m64 */

    /* E0 */ OPATTR_MODRM,    /* PAVGB mm, mm/m64 */
    /* E1 */ OPATTR_MODRM,    /* PSRAW mm, mm/m64 */
    /* E2 */ OPATTR_MODRM,    /* PSRAD mm, mm/m64 */
    /* E3 */ OPATTR_MODRM,    /* PAVGW mm, mm/m64 */
    /* E4 */ OPATTR_MODRM,    /* PMULHUW mm, mm/m64 */
    /* E5 */ OPATTR_MODRM,    /* PMULHW mm, mm/m64 */
    /* E6 */ OPATTR_MODRM,    /* CVTDQ2PD/CVTTPD2DQ/CVTPD2DQ */
    /* E7 */ OPATTR_MODRM,    /* MOVNTQ/MOVNTDQ */
    /* E8 */ OPATTR_MODRM,    /* PSUBSB mm, mm/m64 */
    /* E9 */ OPATTR_MODRM,    /* PSUBSW mm, mm/m64 */
    /* EA */ OPATTR_MODRM,    /* PMINSW mm, mm/m64 */
    /* EB */ OPATTR_MODRM,    /* POR mm, mm/m64 */
    /* EC */ OPATTR_MODRM,    /* PADDSB mm, mm/m64 */
    /* ED */ OPATTR_MODRM,    /* PADDSW mm, mm/m64 */
    /* EE */ OPATTR_MODRM,    /* PMAXSW mm, mm/m64 */
    /* EF */ OPATTR_MODRM,    /* PXOR mm, mm/m64 */

    /* F0 */ OPATTR_MODRM,    /* LDDQU xmm, m128 */
    /* F1 */ OPATTR_MODRM,    /* PSLLW mm, mm/m64 */
    /* F2 */ OPATTR_MODRM,    /* PSLLD mm, mm/m64 */
    /* F3 */ OPATTR_MODRM,    /* PSLLQ mm, mm/m64 */
    /* F4 */ OPATTR_MODRM,    /* PMULUDQ mm, mm/m64 */
    /* F5 */ OPATTR_MODRM,    /* PMADDWD mm, mm/m64 */
    /* F6 */ OPATTR_MODRM,    /* PSADBW mm, mm/m64 */
    /* F7 */ OPATTR_MODRM,    /* MASKMOVQ/MASKMOVDQU */
    /* F8 */ OPATTR_MODRM,    /* PSUBB mm, mm/m64 */
    /* F9 */ OPATTR_MODRM,    /* PSUBW mm, mm/m64 */
    /* FA */ OPATTR_MODRM,    /* PSUBD mm, mm/m64 */
    /* FB */ OPATTR_MODRM,    /* PSUBQ mm, mm/m64 */
    /* FC */ OPATTR_MODRM,    /* PADDB mm, mm/m64 */
    /* FD */ OPATTR_MODRM,    /* PADDW mm, mm/m64 */
    /* FE */ OPATTR_MODRM,    /* PADDD mm, mm/m64 */
    /* FF */ OPATTR_ERROR,    /* Reserved */
};

/*
 * Lock prefix validity table for 2-byte opcodes
 * These opcodes allow the LOCK prefix with memory operands
 */
static const uint8_t g_lock_valid_2byte[] = {
    0xB0, 0xB1,       // CMPXCHG
    0xC0, 0xC1,       // XADD
    0xC7              // Group 9 (CMPXCHG8B/CMPXCHG16B)
};

/*
 * Memory-only instruction table for 2-byte opcodes
 * These opcodes only allow memory operands, not register operands
 */
static const uint8_t g_memory_only_2byte[] = {
    0x00, 0x01,       // SLDT/SGDT/etc.
    0x12, 0x13,       // MOVLPS
    0x16, 0x17,       // MOVHPS
    0x2B,             // MOVNTPS
    0xB2, 0xB4, 0xB5, // LSS/LFS/LGS
    0xC3,             // MOVNTI
    0xC7,             // CMPXCHG8B/CMPXCHG16B
    0xE7              // MOVNTQ
};