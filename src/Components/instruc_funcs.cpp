#include "instruc_funcs.h"

namespace TheBoy{
	namespace CpuFuncs {

		/**
		 * @brief On a Instruction None resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instNone(Cpu* cpu){
			std::cout << "[INSTFUNCS] ::: Not a valid Instructions" << std::endl;
		}


		/**
		 * @brief On a Instruction NOP resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instNOP(Cpu* cpu) {
			std::cout << "[INSTFUNCS] ::: NOP Operation, continue" << std::endl;
		}
		

		/**
		 * @brief On a Instruction JP resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instJP(Cpu* cpu){
			if(validateCondition(cpu)){
				cpu->updatePCtoFetched();
				cpu->requestCycles(1);
			}
		}


		/**
		 * @brief On a Instruction DI resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instDI(Cpu* cpu) {
			cpu->setInterruptState(false);
		}


		/**
		 * @brief On a Instruction DI resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instEI(Cpu* cpu) {
			cpu->setInterruptState(true);
		}


		/**
		 * @brief On a Instruction XOR resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instXOR(Cpu* cpu) {
			cpu->setRegisterValue( 
				REG_A,
				cpu->getRegisterValue(REG_A) ^ (cpu->getFetchedData() & 0xFF)
			);
			cpu->setFlags(cpu->getRegisterValue(REG_A), 0, 0, 0);
		}


		/**
		 * @brief Evaluates a flag condition check
		 * @param cpu Pointer to the target Cpu object
		 * @return true If the condition passes
		 * @return false If the condition fails
		 */
		bool validateCondition(Cpu* cpu){
			bool z_val = cpu->getZFlag();
			bool c_val = cpu->getCFlag();

			switch (cpu->getCurrInstruct()->conType) {
				case CONDTYPE_NONE: return true;
				case CONDTYPE_C: return c_val;
				case CONDTYPE_NC: return !c_val;
				case CONDTYPE_Z: return z_val;
				case CONDTYPE_NZ: return !z_val;
			}
			return false;
		}

		/**
		 * @brief Defines Instruction type to the resolvers
		 */
		static INST_FUNC instructResolvers[] = {
			[INST_NONE] = instNone,
			[INST_NOP] = instNOP,
			[INST_LD] = nullptr,
			[INST_DEC] = nullptr,
			[INST_JP] = instJP,
			[INST_DI] = instDI,
			[INST_EI] = instEI,
			[INST_INC] = nullptr,
			[INST_XOR] = instXOR
		};
		

		/**
		 * @brief Get the Instruct Process function to a given instruction type
		 * @param type Defined instruction type
		 * @return INST_FUNC Pointer to the instruction resolver
		 */
		INST_FUNC getInstructProcess(InstructType type){
			return instructResolvers[type];
		}
	} // namespace CpuFuncs
} // namespace TheBoy
