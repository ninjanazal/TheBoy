#include "instruction.h"

namespace TheBoy {

	/**
	 * @brief Defined instruction set for the LR35902 (GameBoy CPU)
	 *  
	 */
	static Instruc instrucSet[0x100] = {
		[0x00] = {INST_NOP, OPMODE_NONE},
		[0x01] = {INST_LD, OPMODE_R_V16, REG_BC},
		[0x02] = {INST_LD, OPMODE_AR_R, REG_BC, REG_A},
		[0x03] = {INST_INC, OPMODE_R, REG_BC},
		[0x04] = {INST_INC, OPMODE_R, REG_B},
		[0x05] = {INST_DEC, OPMODE_R, REG_B},
		[0x06] = {INST_LD, OPMODE_R_V8, REG_B},
		[0x07] = { },
		[0x08] = {INST_LD, OPMODE_A16_R, REG_NONE, REG_SP},
		[0x09] = {INST_ADD, OPMODE_R_R, REG_HL, REG_BC},
		[0x0A] = {INST_LD, OPMODE_R_AR, REG_A, REG_BC},
		[0x0B] = {INST_DEC, OPMODE_R, REG_BC},
		[0x0C] = {INST_INC, OPMODE_R, REG_C},
		[0x0D] = {INST_DEC, OPMODE_R, REG_C},
		[0x0E] = {INST_LD, OPMODE_R_V8, REG_C},
		[0x0F] = { },
		[0x10] = { },
		[0x11] = {INST_LD, OPMODE_R_V16, REG_DE},
		[0x12] = {INST_LD, OPMODE_AR_R, REG_DE, REG_A},
		[0x13] = {INST_INC, OPMODE_R, REG_DE},
		[0x14] = {INST_INC, OPMODE_R, REG_D},
		[0x15] = {INST_DEC, OPMODE_R, REG_D},
		[0x16] = {INST_LD, OPMODE_R_V8, REG_D},
		[0x17] = { },
		[0x18] = {INST_JR, OPMODE_V8},
		[0x19] = {INST_ADD, OPMODE_R_R, REG_HL, REG_DE},
		[0x1A] = {INST_LD, OPMODE_R_AR, REG_A, REG_DE},
		[0x1B] = {INST_DEC, OPMODE_R, REG_DE},
		[0x1C] = {INST_INC, OPMODE_R, REG_E},
		[0x1D] = {INST_DEC, OPMODE_R, REG_E},
		[0x1E] = {INST_LD, OPMODE_R_V8, REG_E},
		[0x1F] = { },
		[0x20] = {INST_JR, OPMODE_V8, REG_NONE, REG_NONE, CONDTYPE_NZ},
		[0x21] = {INST_LD, OPMODE_R_V16, REG_HL},
		[0x22] = {INST_LD, OPMODE_HLI_R, REG_HL, REG_A},
		[0x23] = {INST_INC, OPMODE_R, REG_HL},
		[0x24] = {INST_INC, OPMODE_R, REG_H},
		[0x25] = {INST_DEC, OPMODE_R, REG_H},
		[0x26] = {INST_LD, OPMODE_R_V8, REG_H},
		[0x27] = { },
		[0x28] = {INST_JR, OPMODE_V8, REG_NONE, REG_NONE, CONDTYPE_Z},
		[0x29] = {INST_ADD, OPMODE_R_R, REG_HL, REG_HL},
		[0x2A] = {INST_LD, OPMODE_R_HLI, REG_A, REG_HL},
		[0x2B] = {INST_DEC, OPMODE_R, REG_HL},
		[0x2C] = {INST_INC, OPMODE_R, REG_L},
		[0x2D] = {INST_DEC, OPMODE_R, REG_L},
		[0x2E] = {INST_LD, OPMODE_R_V8, REG_L},
		[0x2F] = { },
		[0x30] = {INST_JR, OPMODE_V8, REG_NONE, REG_NONE, CONDTYPE_NC},
		[0x31] = {INST_LD, OPMODE_R_V16, REG_SP},
		[0x32] = {INST_LD, OPMODE_HLD_R, REG_HL, REG_A},
		[0x33] = {INST_INC, OPMODE_R, REG_SP},
		[0x34] = {INST_INC, OPMODE_R, REG_HL},
		[0x35] = {INST_DEC, OPMODE_AR, REG_HL},
		[0x36] = {INST_LD, OPMODE_AR_V8, REG_HL},
		[0x37] = { },
		[0x38] = {INST_JR, OPMODE_V8, REG_NONE, REG_NONE, CONDTYPE_C},
		[0x39] = {INST_ADD, OPMODE_R_R, REG_HL, REG_SP},
		[0x3A] = {INST_LD, OPMODE_R_HLD, REG_A, REG_HL},
		[0x3B] = {INST_DEC, OPMODE_R, REG_SP},
		[0x3C] = {INST_INC, OPMODE_R, REG_A},
		[0x3D] = {INST_DEC, OPMODE_R, REG_A},
		[0x3E] = {INST_LD, OPMODE_R_V8, REG_A},
		[0x3F] = { },
		[0x40] = {INST_LD, OPMODE_R_R, REG_B, REG_B},
		[0x41] = {INST_LD, OPMODE_R_R, REG_B, REG_C},
		[0x42] = {INST_LD, OPMODE_R_R, REG_B, REG_D},
		[0x43] = {INST_LD, OPMODE_R_R, REG_B, REG_E},
		[0x44] = {INST_LD, OPMODE_R_R, REG_B, REG_H},
		[0x45] = {INST_LD, OPMODE_R_R, REG_B, REG_L},
		[0x46] = {INST_LD, OPMODE_R_AR, REG_B, REG_HL},
		[0x47] = {INST_LD, OPMODE_R_R, REG_B, REG_A},
		[0x48] = {INST_LD, OPMODE_R_R, REG_C, REG_B},
		[0x49] = {INST_LD, OPMODE_R_R, REG_C, REG_C},
		[0x4A] = {INST_LD, OPMODE_R_R, REG_C, REG_D},
		[0x4B] = {INST_LD, OPMODE_R_R, REG_C, REG_E},
		[0x4C] = {INST_LD, OPMODE_R_R, REG_C, REG_H},
		[0x4D] = {INST_LD, OPMODE_R_R, REG_C, REG_L},
		[0x4E] = {INST_LD, OPMODE_R_AR, REG_C, REG_HL},
		[0x4F] = {INST_LD, OPMODE_R_R, REG_C, REG_A},
		[0x50] = {INST_LD, OPMODE_R_R, REG_D, REG_B},
		[0x51] = {INST_LD, OPMODE_R_R, REG_D, REG_C},
		[0x52] = {INST_LD, OPMODE_R_R, REG_D, REG_D},
		[0x53] = {INST_LD, OPMODE_R_R, REG_D, REG_E},
		[0x54] = {INST_LD, OPMODE_R_R, REG_D, REG_H},
		[0x55] = {INST_LD, OPMODE_R_R, REG_D, REG_L},
		[0x56] = {INST_LD, OPMODE_R_AR, REG_D, REG_HL},
		[0x57] = {INST_LD, OPMODE_R_R, REG_D, REG_A},
		[0x58] = {INST_LD, OPMODE_R_R, REG_E, REG_B},
		[0x59] = {INST_LD, OPMODE_R_R, REG_E, REG_C},
		[0x5A] = {INST_LD, OPMODE_R_R, REG_E, REG_D},
		[0x5B] = {INST_LD, OPMODE_R_R, REG_E, REG_E},
		[0x5C] = {INST_LD, OPMODE_R_R, REG_E, REG_H},
		[0x5D] = {INST_LD, OPMODE_R_R, REG_E, REG_L},
		[0x5E] = {INST_LD, OPMODE_R_AR, REG_E, REG_HL},
		[0x5F] = {INST_LD, OPMODE_R_R, REG_E, REG_A},
		[0x60] = {INST_LD, OPMODE_R_R, REG_H, REG_B},
		[0x61] = {INST_LD, OPMODE_R_R, REG_H, REG_C},
		[0x62] = {INST_LD, OPMODE_R_R, REG_H, REG_D},
		[0x63] = {INST_LD, OPMODE_R_R, REG_H, REG_E},
		[0x64] = {INST_LD, OPMODE_R_R, REG_H, REG_H},
		[0x65] = {INST_LD, OPMODE_R_R, REG_H, REG_L},
		[0x66] = {INST_LD, OPMODE_R_AR, REG_H, REG_HL},
		[0x67] = {INST_LD, OPMODE_R_R, REG_H, REG_A},
		[0x68] = {INST_LD, OPMODE_R_R, REG_L, REG_B},
		[0x69] = {INST_LD, OPMODE_R_R, REG_L, REG_C},
		[0x6A] = {INST_LD, OPMODE_R_R, REG_L, REG_D},
		[0x6B] = {INST_LD, OPMODE_R_R, REG_L, REG_E},
		[0x6C] = {INST_LD, OPMODE_R_R, REG_L, REG_H},
		[0x6D] = {INST_LD, OPMODE_R_R, REG_L, REG_L},
		[0x6E] = {INST_LD, OPMODE_R_AR, REG_L, REG_HL},
		[0x6F] = {INST_LD, OPMODE_R_R, REG_L, REG_A},
		[0x70] = {INST_LD, OPMODE_AR_R, REG_HL, REG_B},
		[0x71] = {INST_LD, OPMODE_AR_R, REG_HL, REG_C},
		[0x72] = {INST_LD, OPMODE_AR_R, REG_HL, REG_D},
		[0x73] = {INST_LD, OPMODE_AR_R, REG_HL, REG_E},
		[0x74] = {INST_LD, OPMODE_AR_R, REG_HL, REG_H},
		[0x75] = {INST_LD, OPMODE_AR_R, REG_HL, REG_L},
		[0x76] = {INST_HALT},
		[0x77] = {INST_LD, OPMODE_AR_R, REG_HL, REG_A},
		[0x78] = {INST_LD, OPMODE_R_R, REG_A, REG_B},
		[0x79] = {INST_LD, OPMODE_R_R, REG_A, REG_C},
		[0x7A] = {INST_LD, OPMODE_R_R, REG_A, REG_D},
		[0x7B] = {INST_LD, OPMODE_R_R, REG_A, REG_E},
		[0x7C] = {INST_LD, OPMODE_R_R, REG_A, REG_H},
		[0x7D] = {INST_LD, OPMODE_R_R, REG_A, REG_L},
		[0x7E] = {INST_LD, OPMODE_R_AR, REG_A, REG_HL},
		[0x7F] = {INST_LD, OPMODE_R_R, REG_A, REG_A},
		[0x80] = {INST_ADD, OPMODE_R_R, REG_A, REG_B},
		[0x81] = {INST_ADD, OPMODE_R_R, REG_A, REG_C},
		[0x82] = {INST_ADD, OPMODE_R_R, REG_A, REG_D},
		[0x83] = {INST_ADD, OPMODE_R_R, REG_A, REG_E},
		[0x84] = {INST_ADD, OPMODE_R_R, REG_A, REG_H},
		[0x85] = {INST_ADD, OPMODE_R_R, REG_A, REG_L},
		[0x86] = {INST_ADD, OPMODE_R_AR, REG_A, REG_HL},
		[0x87] = {INST_ADD, OPMODE_R_R, REG_A, REG_A},
		[0x88] = {INST_ADC, OPMODE_R_R, REG_A, REG_B},
		[0x89] = {INST_ADC, OPMODE_R_R, REG_A, REG_C},
		[0x8A] = {INST_ADC, OPMODE_R_R, REG_A, REG_D},
		[0x8B] = {INST_ADC, OPMODE_R_R, REG_A, REG_E},
		[0x8C] = {INST_ADC, OPMODE_R_R, REG_A, REG_H},
		[0x8D] = {INST_ADC, OPMODE_R_R, REG_A, REG_L},
		[0x8E] = {INST_ADC, OPMODE_R_AR, REG_A, REG_HL},
		[0x8F] = {INST_ADC, OPMODE_R_R, REG_A, REG_A},
		// For sub instructions, declare the 1st Registor to be A
		// Keep uniform
		[0x90] = {INST_SUB, OPMODE_R_R, REG_A, REG_B},
		[0x91] = {INST_SUB, OPMODE_R_R, REG_A, REG_C},
		[0x92] = {INST_SUB, OPMODE_R_R, REG_A, REG_D},
		[0x93] = {INST_SUB, OPMODE_R_R, REG_A, REG_E},
		[0x94] = {INST_SUB, OPMODE_R_R, REG_A, REG_H},
		[0x95] = {INST_SUB, OPMODE_R_R, REG_A, REG_L},
		[0x96] = {INST_SUB, OPMODE_R_AR, REG_A, REG_HL},
		[0x97] = {INST_SUB, OPMODE_R_R, REG_A, REG_A},
		[0x98] = {INST_SBC, OPMODE_R_R, REG_A, REG_B},
		[0x99] = {INST_SBC, OPMODE_R_R, REG_A, REG_C},
		[0x9A] = {INST_SBC, OPMODE_R_R, REG_A, REG_D},
		[0x9B] = {INST_SBC, OPMODE_R_R, REG_A, REG_E},
		[0x9C] = {INST_SBC, OPMODE_R_R, REG_A, REG_H},
		[0x9D] = {INST_SBC, OPMODE_R_R, REG_A, REG_L},
		[0x9E] = {INST_SBC, OPMODE_R_AR, REG_A, REG_HL},
		[0x9F] = {INST_SBC, OPMODE_R_R, REG_A, REG_A},
		[0xA0] = {INST_AND, OPMODE_R_R, REG_A, REG_B},
		[0xA1] = {INST_AND, OPMODE_R_R, REG_A, REG_C},
		[0xA2] = {INST_AND, OPMODE_R_R, REG_A, REG_D},
		[0xA3] = {INST_AND, OPMODE_R_R, REG_A, REG_E},
		[0xA4] = {INST_AND, OPMODE_R_R, REG_A, REG_H},
		[0xA5] = {INST_AND, OPMODE_R_R, REG_A, REG_L},
		[0xA6] = {INST_AND, OPMODE_R_AR, REG_A, REG_HL},
		[0xA7] = {INST_AND, OPMODE_R_R, REG_A, REG_A},
		[0xA8] = {INST_XOR, OPMODE_R_R, REG_A, REG_B},
		[0xA9] = {INST_XOR, OPMODE_R_R, REG_A, REG_C},
		[0xAA] = {INST_XOR, OPMODE_R_R, REG_A, REG_D},
		[0xAB] = {INST_XOR, OPMODE_R_R, REG_A, REG_E},
		[0xAC] = {INST_XOR, OPMODE_R_R, REG_A, REG_H},
		[0xAD] = {INST_XOR, OPMODE_R_R, REG_A, REG_L},
		[0xAE] = {INST_XOR, OPMODE_R_AR, REG_A, REG_HL},
		[0xAF] = {INST_XOR, OPMODE_R_R, REG_A, REG_A},
		[0xB0] = {INST_OR, OPMODE_R_R, REG_A, REG_B},
		[0xB1] = {INST_OR, OPMODE_R_R, REG_A, REG_C},
		[0xB2] = {INST_OR, OPMODE_R_R, REG_A, REG_D},
		[0xB3] = {INST_OR, OPMODE_R_R, REG_A, REG_E},
		[0xB4] = {INST_OR, OPMODE_R_R, REG_A, REG_H},
		[0xB5] = {INST_OR, OPMODE_R_R, REG_A, REG_L},
		[0xB6] = {INST_OR, OPMODE_R_AR, REG_A, REG_HL},
		[0xB7] = {INST_OR, OPMODE_R_R, REG_A, REG_A},
		[0xB8] = {INST_CP, OPMODE_R_R, REG_A, REG_B},
		[0xB9] = {INST_CP, OPMODE_R_R, REG_A, REG_C},
		[0xBA] = {INST_CP, OPMODE_R_R, REG_A, REG_D},
		[0xBB] = {INST_CP, OPMODE_R_R, REG_A, REG_E},
		[0xBC] = {INST_CP, OPMODE_R_R, REG_A, REG_H},
		[0xBD] = {INST_CP, OPMODE_R_R, REG_A, REG_L},
		[0xBE] = {INST_CP, OPMODE_R_AR, REG_A, REG_HL},
		[0xBF] = {INST_CP, OPMODE_R_R, REG_A, REG_A},
		[0xC0] = {INST_RET, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NZ},
		[0xC1] = {INST_POP, OPMODE_NONE, REG_BC},
		[0xC2] = {INST_JP, OPMODE_V16, REG_NONE, REG_NONE, CONDTYPE_NZ},
		[0xC3] = { INST_JP, OPMODE_V16},
		[0xC4] = {INST_CALL, OPMODE_V16, REG_NONE, REG_NONE, CONDTYPE_NZ},
		[0xC5] = {INST_PUSH, OPMODE_NONE, REG_BC},
		[0xC6] = {INST_ADD, OPMODE_R_V8, REG_A},
		[0xC7] = {INST_RST, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NONE, 0x00},
		[0xC8] = {INST_RET, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_Z},
		[0xC9] = {INST_RET},
		[0xCA] = {INST_JP, OPMODE_V16, REG_NONE, REG_NONE, CONDTYPE_Z},
		[0xCB] = {INST_PRECB, OPMODE_V8},
		[0xCC] = {INST_CALL, OPMODE_V16, REG_NONE, REG_NONE, CONDTYPE_Z},
		[0xCD] = {INST_CALL, OPMODE_V16},
		[0xCE] = {INST_ADC, OPMODE_R_V8, REG_A},
		[0xCF] = {INST_RST, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NONE, 0x08},
		[0xD0] = {INST_RET, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NC},
		[0xD1] = {INST_POP, OPMODE_NONE, REG_BC},
		[0xD2] = {INST_JP, OPMODE_V16, REG_NONE, REG_NONE, CONDTYPE_NC},
		[0xD3] = { }, // NO DEFINITION
		[0xD4] = {INST_CALL, OPMODE_V16, REG_NONE, REG_NONE, CONDTYPE_NC},
		[0xD5] = {INST_PUSH, OPMODE_NONE, REG_DE},
		[0xD6] = {INST_SUB, OPMODE_R_V8, REG_A},
		[0xD7] = {INST_RST, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NONE, 0x10},
		[0xD8] = {INST_RET, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_C},
		[0xD9] = {INST_RETI},
		[0xDA] = {INST_JP, OPMODE_V16, REG_NONE, REG_NONE, CONDTYPE_C},
		[0xDB] = { }, // NO DEFINITION
		[0xDC] = {INST_CALL, OPMODE_V16, REG_NONE, REG_NONE, CONDTYPE_C},
		[0xDD] = { }, // NO DEFINITION
		[0xDE] = {INST_SBC, OPMODE_R_V8, REG_A},
		[0xDF] = {INST_RST, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NONE, 0x18},
		[0xE0] = {INST_LDH, OPMODE_A8_R, REG_NONE, REG_A},
		[0xE1] = {INST_POP, OPMODE_NONE, REG_HL},
		[0xE2] = {INST_LD, OPMODE_AR_R, REG_C, REG_A},
		[0xE3] = { }, // NO DEFINITION
		[0xE4] = { }, // NO DEFINITION
		[0xE5] = {INST_PUSH, OPMODE_NONE, REG_HL},
		[0xE6] = {INST_AND, OPMODE_R_V8, REG_A},
		[0xE7] = {INST_RST, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NONE, 0x20},
		[0xE8] = {INST_ADD, OPMODE_R_V8, REG_SP}, 
		[0xE9] = {INST_JP, OPMODE_AR, REG_HL},
		[0xEA] = {INST_LD, OPMODE_A16_R, REG_NONE, REG_A},
		[0xEB] = { }, // NO DEFINITION
		[0xEC] = { }, // NO DEFINITION
		[0xED] = { }, // NO DEFINITION
		[0xEE] = {INST_XOR, OPMODE_R_V8, REG_A},
		[0xEF] = {INST_RST, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NONE, 0x28},
		[0xF0] = {INST_LDH, OPMODE_R_A8, REG_A},
		[0xF1] = {INST_POP, OPMODE_NONE, REG_AF},
		[0xF2] = {INST_LD, OPMODE_R_AR, REG_A, REG_C},
		[0xF3] = {INST_DI, OPMODE_NONE},
		[0xF4] = { }, // NO DEFINITION
		[0xF5] = {INST_PUSH, OPMODE_NONE, REG_AF},
		[0xF6] = {INST_OR, OPMODE_R_V8, REG_A},
		[0xF7] = {INST_RST, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NONE, 0x30},
		[0xF8] = {INST_EI, OPMODE_NONE},
		[0xF9] = { },
		[0xFA] = {INST_LD, OPMODE_R_A16, REG_A},
		[0xFB] = { },
		[0xFC] = { }, // NO DEFINITION
		[0xFD] = { }, // NO DEFINITION
		[0xFE] = {INST_CP, OPMODE_R_V8, REG_A},
		[0xFF] = {INST_RST, OPMODE_NONE, REG_NONE, REG_NONE, CONDTYPE_NONE, 0x38}

	};

	/**
	 * @brief Get the By Opcode object
	 * @param opCd Opcode value for the instructions
	 * @return Instruc* Pointer to Defined instruction value
	 */
	Instruc* getByOpcode(bit8 opCd){
		return &instrucSet[opCd];
	}
	
} // namespace TheBoy