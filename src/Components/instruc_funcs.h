#ifndef INSTRUC_FUNCS_H
#define INSTRUC_FUNCS_H

#include "instruction.h"
#include "cpu.h"

namespace TheBoy {
	class Cpu;

	namespace CpuFuncs {
	/**
	 * @brief Gets the bit n state from value
	 */
	#define GETBIT(from, n) (from & (1 << n)) >> n

		/**
		 * @brief Definition to the instruction function, this holds a pointer to a function
		 * that recieves a Cpu pointer, this function will resolve an instruction to the defined Cpu
		 */
		typedef void (*INST_FUNC)(Cpu*);

		/**
		 * @brief Get the Instruct Process function to a given instruction type
		 * @param type Defined instruction type
		 * @return INST_FUNC Pointer to the instruction resolver
		 */
		INST_FUNC getInstructProcess(InstructType type);


		/**
		 * @brief Evaluates a flag condition check
		 * @param cpu Pointer to the target Cpu object
		 * @return true If the condition passes
		 * @return false If the condition fails
		 */
		bool validateCondition(Cpu* cpu);

		
	} // namespace CpuFuncs	
} // namespace TheBoy


#endif