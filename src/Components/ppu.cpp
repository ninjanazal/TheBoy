#include "ppu.h"


namespace TheBoy {

	/**
	 * @brief Construct a new Ppu object
	 * @param ctrl Current emulator controller reference
	 */
	Ppu::Ppu(EmulatorController* ctrl) : emulCtrl(ctrl) {
		std::cout << "[PPU] ::: PPU has been created" << std::endl;

		oam_ram = new OamElement[40];
		vRam = new bit8[0x2000] { };
	}


	/**
	 * @brief Destroy the Ppu object
	 */
	Ppu::~Ppu() {
		delete [] vRam;
	}


	/**
	 * @brief Ppu interation
	 */
	void Ppu::step() {
	}


	/**
	 * @brief Writes a value to the OAM ram position
	 * @param addr Target address value
	 * @param val Defined value
	 */
	void Ppu::oamWrite(bit16 addr, bit8 val) {
		// Manage offset buffer position
		if(addr >= 0xFE00){
			addr -= 0xFE00;
		}
		// For memory location address, castring the arry to a bit8 pointer
		// This will produce a access to the target Oam entry value
		((bit8 *)oam_ram)[addr] = val;
	}


	/**
	 * @brief Reads from the OAM Ram on the defined addres
	 * @param addr Target addres
	 * @return bit8 Value on the addres
	 */
	bit8 Ppu::oamRead(bit16 addr) {
		// Manage offset buffer position
		if(addr >= 0xFE00){
			addr -= 0xFE00;
		}
		// For memory location address, castring the arry to a bit8 pointer
		// This will produce a access to the target Oam entry value
		return ((bit8 *)oam_ram)[addr];
	}


	/**
	 * @brief Writes to the vRam on the defined address
	 * @param addr Target address
	 * @param val Defined value
	 */
	void Ppu::write(bit16 addr, bit8 val) {
		// Tile data is stored in VRAM in the memory area at $8000-$97FF;
		vRam[addr - 0x8000] = val;

	}


	/**
	 * @brief Reads from the vRam on the defined address
	 * @param addr Target addres
	 * @return bit8 Value on the address
	 */
	bit8 Ppu::read(bit16 addr) {
		// Tile data is stored in VRAM in the memory area at $8000-$97FF;
		return vRam[addr - 0x8000];
	}

	
} // namespace TheBoy
