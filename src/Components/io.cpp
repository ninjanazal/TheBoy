#include "io.h"

namespace TheBoy {
	/**
	 * @brief Construct a new IO object
	 * @param ctrl Target emulator controller
	 */
	IO::IO(EmulatorController* ctrl) : emulCtrl(ctrl) {
		std::cout << "[IO] ::: IO has been created" << std::endl;
	}

	/**
	 * @brief Reads the IO valeu on the defined address
	 * @param addr Target read addres
	 * @return bit8 Value on the target addres 
	 */
	bit8 IO::read(bit16 addr) {
		/*
		Serial Data Transfer (Link Cable)
			 Communication between two Game Boy systems happens one byte at a time.
		
		FF01 - SB - Serial transfer data (R/W)
			 During a transfer, it has a blend of the outgoing and incoming bytes.
			 Each cycle, the leftmost bit is shifted out (and over the wire) and the incoming bit is
			 shifted in from the other side

		FF02 - SC - Serial Transfer Control (R/W)
			Bit 7 - Transfer Start Flag (0=No transfer is in progress or requested,
					1=Transfer in progress, or requested)
			Bit 1 - Clock Speed (0=Normal, 1=Fast) ** CGB Mode Only **
			Bit 0 - Shift Clock (0=External Clock, 1=Internal Clock)
		*/

		if(addr == 0xFF01) {
			return seriaData[0];
		}

		if(addr == 0xFF02) {
			return seriaData[1];
		}
	
		// Timer IO access
		if(BETWEEN(addr, 0xFF04, 0xFF07)){
			return emulCtrl->getTimer()->read(addr);
		}

		// FF0F - IF - Interrupt Flag (R/W)
		if (addr == 0xFF0F){
			return emulCtrl->getCpu()->getInterrFlags();
		}
		return 0x0;
	}


	/**
	 * @brief Write the io value on the defined addres
	 * @param addr Target Write addres
	 * @param val value to be written
	 */
	void IO::write(bit16 addr, bit8 val) {
		if(addr == 0xFF01)
			seriaData[0] = val;
			return;

		if(addr == 0xFF02){
			seriaData[1] = val;
			return;
		}
	
		// Timer IO access
		if(BETWEEN(addr, 0xFF04, 0xFF07)){
			emulCtrl->getTimer()->write(addr, val);
			return;
		}

		// FF0F - IF - Interrupt Flag (R/W)
		if (addr == 0xFF0F){
			emulCtrl->getCpu()->setInterrFlags(val);
			return;
		}

		// FF46 - DMA (DMA Transfer and Start Address)
		if (addr == 0xFF46) {
			emulCtrl->getDma()->start(val);
		}
	}
	
} // namespace TheBoy
