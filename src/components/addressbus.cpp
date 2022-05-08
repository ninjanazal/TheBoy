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
	 * @brief Writes to a defined address
	 * @param addr Target address to be written
	 * @param val Value to be setted on the address 
	 */
	void AddressBus::abWrite(bit16 addr, bit8 val){
		return;
		//if(addr<0x8000){ }
	}

}