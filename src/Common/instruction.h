#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "common.h"

namespace TheBoy {
	
	/*
	OPCODES exemple
	-------------
	|	INS reg | 	← Instruction mnemonic (Instruction type + operation mode)
	|	2  8	| 	← Not using (length in bytes + cpu cycles)
	|	Z N H C	| 	← Affected flags
	------------

	[0x00] - NOP (no operation)
		-> {INST_NOP, OPMODE_NONE} 
	[0x01] - LD BC,d16 (load 16bit from address to register BC)
		-> {INST_LD, OPMODE_R_V16, REG_BC}
	*/


	/**
	 * @brief Defined enumeration for the available instructions types
	 * Exemple, load, Jump, Add operations
	 * This is the first element of an instruction
	 */
	typedef enum {
		INST_NONE,
		INST_NOP,
		INST_LD,
		INST_DEC,
		INST_JP,
		INST_DI,
		INST_EI,
		INST_INC,
		INST_XOR,
	} InstructType;


	/**
	 * @brief Holds enumeration for the availble registers
	 */
	typedef enum {
		REG_NONE,
		REG_A,
		REG_F,
		REG_B,
		REG_C,
		REG_D,
		REG_E,
		REG_H,
		REG_L,
		REG_AF,
		REG_BC,
		REG_DE,
		REG_HL,
		REG_SP,
		REG_PC
	} RegisterType;


	/**
	 * @brief Defines how the Instruction type should interact with memory
	 * EX {LD BC, D16} -> for this load operation a value with 16bit size is loaded to the register BC
	 * OPMODE_R_V16 -> _R (Registor) _V16 (16bit value)
	 */
	typedef enum {
		OPMODE_NONE,
		OPMODE_R,
		OPMODE_V16,
		OPMODE_R_V16,
		OPMODE_R_V8,
		OPMODE_AR_R
	} OperationMode;


	/**
	 * @brief Defines the available conditions, some opertions have a flag validations
	 * EX {JP NZ,a16} -> for this jump operation not Z flag is checked
	 */
	typedef enum {
		CONDTYPE_NONE,
		CONDTYPE_NZ,
		CONDTYPE_Z,
		CONDTYPE_NC,
		CONDTYPE_C
	} ConditionType;


	/**
	 * @brief Defines the instruction logic structure
	 */
	typedef struct {
		InstructType insType;
		OperationMode opMode;
		RegisterType regTypeL;
		RegisterType regTypeR;
		ConditionType conType;
	} Instruc;


	/**
	 * @brief Get the By Opcode object
	 * @param opCd Opcode value for the instructions
	 * @return Instruc* Pointer to Defined instruction value
	 */
	Instruc* getByOpcode(bit8 opCd);

} // namespace TheBoy


#endif