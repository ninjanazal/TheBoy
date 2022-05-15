#include "addressbus.h"

namespace TheBoy {
	/**
	 * @brief Construct a new Address Bus object
	 */
	AddressBus::AddressBus(EmulatorController* ctrl): emuCtrl(ctrl) {
		std::cout << "[ADDRESSBUS] ::: AddressBus has been created" << std::endl;
	}



	/**
	 * @brief Reads the value from the defined address
	 * @param addr Read address value
	 * @return bit8 Value on the defined address
	 */
	bit8 AddressBus::abRead(bit16 addr) {
		//printf("[ADDRESSBUS] ::: Reading from addr: %2.2X\n", addr);
		//fflush(stdout);

		// From cartridge, fixed bank and switchable via mapper
		if(addr < 0x8000) {
			return emuCtrl->getCartridge()->read(addr);
		}
		// switchable bank (0/1) video Ram
		else if(addr < 0xA000) {
			// TODO
			return 0x00;
		}
		// From cartridge External RAM
		else if(addr < 0xC000) {
			return emuCtrl->getCartridge()->read(addr);
		}
		// Work RAM 4kiB and switchable banks (4kiB)
		else if(addr < 0xE000) {
			return emuCtrl->getRam()->wRead(addr);
		}
		// Echo RAM, Nintendo says use of this area is prohibited
		else if(addr < 0xFE00) {
			std::cout << "[ADDRESSBUS] ::: Echo RAM read request, locked" << std::endl;
			return 0x00;
		}
		// OAM Sprite attribute table
		else if(addr < 0xFEA0) {
			// TODO
			return 0x00;
		}
		// Not Usable
		else if(addr < 0xFF00) {
			std::cout << "[ADDRESSBUS] ::: Reserver area read request, locked" << std::endl;
			return 0x00;
		}
		// I/O Registers
		else if(addr < 0xFF80) {
			// TODO
			std::cout << "[ADDRESSBUS] ::: Reading to I/O Registers" << std::endl;
			return 0x00;
		}
		// Interrupt Enable register
		else if(addr == 0xFFFF ) {
			return emuCtrl->getCpu()->getCpuIERegister();
		}
		// FF80 -> FFFE High RAM
		return emuCtrl->getRam()->hRead(addr);
		
	}

	
	/**
	 * @brief Reads the 16bit value from the defined address 
	 * @param addr Read address value
	 * @return bit16 Value on the defined address
	 */
	bit16 AddressBus::abRead16(bit16 addr){
		//printf("[ADDRESSBUS] ::: Reading from addr: %2.2X -> %2.2X\n", addr, addr + 0x1);
		//fflush(stdout);

		bit16 l = abRead(addr);
		bit16 h = abRead(addr + 0x1);

		return l | (h << 8);
	}


	/**
	 * @brief Writes to a defined address
	 * @param addr Target address to be written
	 * @param val Value to be setted on the address 
	 */
	void AddressBus::abWrite(bit16 addr, bit8 val){
		// Rom values
		//printf("[ADDRESSBUS] ::: Writing to addr: %2.2X\n", addr);
		//fflush(stdout);

		// From cartridge, fixed bank and switchable via mapper
		if(addr < 0x8000){
			emuCtrl->getCartridge()->write(addr, val);
		}
		// switchable bank (0/1) video Ram
		else if(addr < 0xA000) {
			// TODO
		}
		// From cartridge External RAM
		else if(addr < 0xC000) {
			emuCtrl->getCartridge()->write(addr, val);
		}
		// Work RAM 4kiB and switchable banks (4kiB)
		else if(addr < 0xE000) {
			emuCtrl->getRam()->wWrite(addr, val);
		}
		// Echo RAM, Nintendo says use of this area is prohibited
		else if(addr < 0xFE00) {
			// TODO
		}
		// OAM Sprite attribute table
		else if(addr < 0xFEA0) {
			// TODO
		}
		// Not Usable
		else if(addr < 0xFF00) {
			std::cout << "[ADDRESSBUS] ::: Reserver area write request, locked" << std::endl;
		}
		// I/O Registers
		else if(addr < 0xFF80) {
			// TODO
			std::cout << "[ADDRESSBUS] ::: Reading to I/O Registers" << std::endl;
		}
		// Interrupt Enable register
		else if(addr == 0xFFFF ) {
			emuCtrl->getCpu()->setCpuIERegister(val);
		}
		// High RAM (HRAM)
		else {
			emuCtrl->getRam()->hWrite(addr, val);
		}
	}


	/**
	 * @brief Writes to the defined address a 16bit value
	 * @param addr Target address to be written
	 * @param val Value to be setted on the address
	 */
	void AddressBus::abWrite16(bit16 addr, bit16 val){
		//printf("[ADDRESSBUS] ::: Writing 16bits to addr: %2.2X\n", addr);
		//fflush(stdout);

		emuCtrl->getCartridge()->write(addr + 1, (val >> 8) & 0xFF);
		emuCtrl->getCartridge()->write(addr, val & 0xFF);
	}

}