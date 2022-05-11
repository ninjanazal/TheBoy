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
		// Rom values
		if(addr<0x8000){
			return emuCtrl->getCartridge()->read(addr);
		}
		return 0x00;
	}

	
	/**
	 * @brief Reads the 16bit value from the defined address 
	 * @param addr Read address value
	 * @return bit16 Value on the defined address
	 */
	bit16 AddressBus::abRead16(bit16 addr){
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
		if(addr<0x8000){
			emuCtrl->getCartridge()->write(addr, val);
		 }
	}


	/**
	 * @brief Writes to the defined address a 16bit value
	 * @param addr Target address to be written
	 * @param val Value to be setted on the address
	 */
	void AddressBus::abWrite16(bit16 addr, bit16 val){
		emuCtrl->getCartridge()->write(addr + 1, (val >> 8) & 0xFF);
		emuCtrl->getCartridge()->write(addr, val & 0xFF);
	}

}