#include "ppu.h"


namespace TheBoy {

	/**
	 * @brief Construct a new Ppu object
	 * @param ctrl Current emulator controller reference
	 */
	Ppu::Ppu(EmulatorController* ctrl) : emulCtrl(ctrl) {
		std::cout << "[PPU] ::: PPU has been created" << std::endl;
	}

	
} // namespace TheBoy
