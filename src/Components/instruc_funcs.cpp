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
		 * @brief On a Instruction XOR resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instLD(Cpu* cpu){
			if(cpu->getDestenyIsMem()){
				// Form loads where the target is a memory location ex {0x02}
				// if is a 16bit value
				if(cpu->getCurrInstruct()->regTypeL >= RegisterType::REG_AF){
					cpu->requestCycles(1);
				}
				else{
					// for 8bit writes
					cpu->requestBusWrite(cpu->getMemoryDest(), cpu->getFetchedData());
				}
				return;
			}

			// For the Unique operation {F8}, loads the stack pointer to hl and increment by r8 
			if(cpu->getCurrInstruct()->opMode == OPMODE_HL_SPR){
				// Since this operation has the H & C flag set to flip on demand
				// Evaluates the Half Carry flag status, if adding the 
				bit8 hFlag = (cpu->getRegisterValue(cpu->getCurrInstruct()->regTypeR) & 0xF) +
						(cpu->getFetchedData() & 0xF) >= 0x10;
				// Evaluates the carry flag status
				bit8 cFlag = (cpu->getRegisterValue(cpu->getCurrInstruct()->regTypeR) & 0xFF) +
						(cpu->getFetchedData() & 0xFF) >=0x100;

				cpu->setFlags(0, 0, hFlag, cFlag);
				// Defines the HL Registor to the value of the StackPointer Reg + the 8bit fetched data
				cpu->setRegisterValue(
					cpu->getCurrInstruct()->regTypeL,
					cpu->getRegisterValue(cpu->getCurrInstruct()->regTypeR) + (bit8)cpu->getFetchedData()
				);
			}
			cpu->setRegisterValue(cpu->getCurrInstruct()->regTypeL, cpu->getFetchedData());
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
			[INST_LD] = instLD,
			[INST_DEC] = nullptr,
			[INST_JP] = instJP,
			[INST_DI] = instDI,
			[INST_EI] = instEI,
			[INST_INC] = nullptr,
			[INST_XOR] = instXOR,
			[INST_HALT] = nullptr
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
