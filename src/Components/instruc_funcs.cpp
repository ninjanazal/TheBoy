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
			jumpToAddress(cpu, cpu->getFetchedData(), false);
		}

		/**
		 * @brief On a Instruction JR resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instJR(Cpu* cpu){
			// Jump relative can be a increment/decrement for the current Value, and the bit8 definition is unsigned
			// casting to char
			// fetchdata holds a 2Byte value, and only 1B should be used
			char relative = (char)(cpu->getFetchedData() & 0xFF);
			bit16 addr = cpu->getRegisterValue(REG_PC) + relative;
			jumpToAddress(cpu, addr, false);
		}


		/**
		 * @brief On a Instruction CALL resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instCALL(Cpu* cpu){
			jumpToAddress(cpu, cpu->getFetchedData(), true);
		}


		/**
		 * @brief On a Instruction RET resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instRET(Cpu* cpu){
			// check if is a return with conditions
			if (cpu->getCurrInstruct()->conType != CONDTYPE_NONE) {
				cpu->requestCycles(1);
			}

			if(validateCondition(cpu)){
				bit16 lo = cpu->pop();
				cpu->requestCycles(1);

				bit16 hi = cpu->pop();
				cpu->requestCycles(1);

				cpu->setRegisterValue(REG_PC, lo | (hi << 8));
				cpu->requestCycles(1);
			}
		}

		/**
		 * @brief On a Instruction RETI resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instRETI(Cpu* cpu){
			// Reactivate the master interrupt flag
			cpu->setInterruptMasterState(true);
			// Same as the default RET
			instRET(cpu);
		}


		/**
		 * @brief On a Instruction DI resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instDI(Cpu* cpu) {
			cpu->setInterruptMasterState(false);
		}


		/**
		 * @brief On a Instruction DI resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instEI(Cpu* cpu) {
			cpu->setInterruptMasterState(true);
		}


		/**
		 * @brief On a Instruction INC resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instINC(Cpu* cpu) {
			bit16 val = cpu->getFetchedData() + 0x1;

			// For 16bit registors
			if(cpu->getCurrInstruct()->regTypeL >= RegisterType::REG_AF) {
				cpu->requestCycles(1);
			}

			// For the specific INC (HL) Operation {34}
			if(cpu->getCurrInstruct()->regTypeL == REG_HL &&
					cpu->getCurrInstruct()->opMode == OPMODE_AR){
				val = cpu->getRegisterValue(REG_HL) + 0x1;
				// Write the incremented value, write uses 8bit
				// Preventing overflow
				cpu->requestBusWrite(REG_HL, val & 0xFF);
			}
			else {
				cpu->setRegisterValue(cpu->getCurrInstruct()->regTypeL, val);
				// Redefine for check 
				val = cpu->getRegisterValue(cpu->getCurrInstruct()->regTypeL);
			}

			// Only INC opCode instructions with a 0x03 termination dont update any flags
			// EX {03, 13, 23 & 33}
			if((cpu->getCurrentOPCode() & 0x03) != 0x03) {
				// Flags check Z 0 H -
				cpu->setFlags(val == 0, 0, (val & 0xF) >= 16, -1);
			} 
		}

		/**
		 * @brief On a Instruction DEC resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instDEC(Cpu* cpu) {
			bit16 val = cpu->getFetchedData() - 0x1;

			// For 16bit registors
			if(cpu->getCurrInstruct()->regTypeL >= RegisterType::REG_AF) {
				cpu->requestCycles(1);
			}

			// For the specific INC (HL) Operation {34}
			if(cpu->getCurrInstruct()->regTypeL == REG_HL &&
					cpu->getCurrInstruct()->opMode == OPMODE_AR){
				val = cpu->getRegisterValue(REG_HL) - 0x1;
				// Write the incremented value, write uses 8bit
				// Preventing overflow
				cpu->requestBusWrite(REG_HL, val & 0xFF);
			}
			else {
				cpu->setRegisterValue(cpu->getCurrInstruct()->regTypeL, val);
				// Redefine for check 
				val = cpu->getRegisterValue(cpu->getCurrInstruct()->regTypeL);
			}

			// Only INC opCode instructions with a 0x0B termination dont update any flags
			// EX {0B, 1B, 2B & 3B}
			if((cpu->getCurrentOPCode() & 0x0B) != 0x0B) {
				// Flags check Z 0 H -
				cpu->setFlags(val == 0, 1, (val & 0x0F) >= 16, -1);
			} 
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
		 * @brief On a Instruction LDH resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instLDH(Cpu* cpu){
			// For the operation {F0}
			if(cpu->getCurrInstruct()->regTypeL == REG_A){
				// Loads from high ram, since this operation has a 8bit memory address on the 2nd operand,
				// and high Ram, a or opperation with 0xFF00 needs to be done to place correctly this pointer
				cpu->setRegisterValue( REG_A,
					cpu->requestBusRead(0xFF00 | cpu->getFetchedData())
				);
			}
			// For the operation {E0}
			else {
				// Loads to the highRam, the value on the defined register, since the fetch for this operation is a 8Bit
				// value and to be setted on the high RAM range, a 0xFF00 or operation is need 
				cpu->requestBusWrite(0xFF00 | cpu->getFetchedData(),
					cpu->getRegisterValue(cpu->getCurrInstruct()->regTypeR)
				);
			}
			cpu->requestCycles(1);
		}


		/**
		 * @brief On a Instruction POP resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instPOP(Cpu* cpu){
			bit16 l = cpu->pop();
			cpu->requestCycles(1);
			bit16 h = cpu->pop();

			cpu->requestCycles(1);
			cpu->setRegisterValue(cpu->getCurrInstruct()->regTypeL, (l | (h << 8)));

			// For the Operation {F1}
			if(cpu->getCurrInstruct()->regTypeL == REG_AF) {
				/*
					POP AF completely replaces the F register
					value, so all flags are changed based on the 8-bit data that is read from memor
					This if is just a fail safe condition
				*/
				cpu->setRegisterValue(cpu->getCurrInstruct()->regTypeL,
					(l | (h << 8)) & 0xFFF0
				);
			}
		}


		/**
		 * @brief On a Instruction PUSH resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instPUSH(Cpu* cpu){
			// Keep in mind the order of write/read, left to right
			// push uses a 8bit value, soo 
			// EX REG_BC -> getCurrInst = bit16 BC
			// Writing (left to right) B needs to go first so BC >> 8 == 0x00(B) & 0xFF = (B)
			bit16 hi = (cpu->getRegisterValue(cpu->getCurrInstruct()->regTypeL) >> 8) & 0xFF;
			cpu->requestCycles(1);
			cpu->push(hi);

			bit16 lo = cpu->getRegisterValue(cpu->getCurrInstruct()->regTypeL) & 0xFF;
			cpu->requestCycles(1);
			cpu->push(lo);

			cpu->requestCycles(1);
		}


		/**
		 * @brief On a Instruction PUSH resolver
		 * @param cpu Requester cpu pointer
		 */
		static void instRST(Cpu* cpu) {
			jumpToAddress(cpu, cpu->getCurrInstruct()->param_val, true);
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
		 * @brief Generic Jump instruction, this can be used on the JP instruction and the Call isntruction
		 * Since this is the same as JP but updates the Program Counter register
		 * @param cpu Pointer to the target Cpu object
		 * @param addre Address to be setted on the program Counter register
		 * @param pushPC Marks if should push the PC Registed to the stack pointer
		 */
		void jumpToAddress(Cpu* cpu, bit16 addr, bool pushPC) {
			if(validateCondition(cpu)){
				if(pushPC) {
					// 2 cycles for a bit16 push
					cpu->requestCycles(2);
					cpu->push16(cpu->getRegisterValue(REG_PC));
				}
				cpu->setRegisterValue(REG_PC, addr);
				cpu->requestCycles(1);
			}
		}

		/**
		 * @brief Defines Instruction type to the resolvers
		 */
		static INST_FUNC instructResolvers[] = {
			[INST_NONE] = instNone,
			[INST_NOP] = instNOP,
			[INST_LD] = instLD,
			[INST_DEC] = instDEC,
			[INST_JP] = instJP,
			[INST_DI] = instDI,
			[INST_EI] = instEI,
			[INST_INC] = instINC,
			[INST_XOR] = instXOR,
			[INST_HALT] = nullptr,
			[INST_LDH] = instLDH,
			[INST_POP] = instPOP,
			[INST_PUSH] = instPUSH,
			[INST_CALL] = instCALL,
			[INST_RET] = instRET,
			[INST_RETI] = instRETI,
			[INST_JR] = instJR,
			[INST_RST] = instRST
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
