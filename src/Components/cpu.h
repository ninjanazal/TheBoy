#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "emulatorController.h"
#include "instruction.h"

namespace TheBoy {
	class EmulatorController;
/*
	16-bit	Hi	Lo	Name/Function
	----------------------------------
	AF	A	-	Accumulator & Flags
	BC	B	C	BC
	DE	D	E	DE
	HL	H	L	HL
	SP	-	-	Stack Pointer
	PC	-	-	Program Counter/Pointer
	----------------------------------

As shown above, most registers can be accessed either as one 16-bit register,
 or as two separate 8-bit registers.
*/
	typedef struct {
		bit8 A;
		bit8 F;
		bit8 B;
		bit8 C;
		bit8 D;
		bit8 E;
		bit8 H;
		bit8 L;
		bit8 S;
		bit16 SP;
		bit16 PC;
	} Registres;

	/**
	 * @brief Holds the Cpu definitions
	 */
	class Cpu {
		public:
		/**
		 * @brief Construct a new Cpu object
		 */
		Cpu();


		/**
		 * @brief Destroy the Cpu object
		 */
		~Cpu() = default;

	private:

	/**
	 * @brief Shared pointer to the registers values
	 */
	std::shared_ptr<Registres> regs;

	/**
	 * @brief Contains information about the result of the most recent
	 * instruction that has affected flags
	 */
	bit8 regFlags;


	/**
	 * @brief Marks if the current cpu has been halted, its in idle mode
	 */
	bool cpuHLT;

	/**
	 * @brief Pointer to the current target instruction 
	 */
	Instruc *currInstruct;
	
	};
}

#endif