#include "interrupt.h"

namespace TheBoy {
	namespace InterruptFuncs {

		/**
		 * @brief Aux function to push the current PC value to the stack and assign the interrup addres
		 * @param cpu Target Cpu
		 * @param addr Interrupt jump address
		 */
		void manage_interrupt(Cpu* cpu, bit16 addr) {
			cpu->push16(cpu->getRegisterValue(REG_PC));
			cpu->setRegisterValue(REG_PC, addr);
		}


		/**
		 * @brief Requests a Interrupt on the defined cpu
		 * @param cpu Target Cpu
		 * @param iType Interrupt type
		 */
		void request_interrupt(Cpu* cpu, InterruptType iType) {
			if(cpu->getInterrFlags() & INTR_VBLANK && cpu->getCpuIERegister() & INTR_VBLANK) {
				manage_interrupt(cpu, 0x40);
			}
		}


		/**
		 * @brief Handles a interrupt on the defined cpu
		 * @param cpu Target Cpu
		 */
		void handle_interrupt(Cpu* cpu) {
			
		}

	} // namespace InterruptFuncs	
} // namespace TheBoy