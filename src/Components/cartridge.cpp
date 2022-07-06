#include <fstream>
#include <iomanip>
#include <cstring>

#include "cartridge.h"
#include "emulatorController.h"


namespace TheBoy {
	/**
	 * @brief Construct a new Cartridge object
	 * @param path Path to the cartridge to be loaded
	 */
	Cartridge::Cartridge(EmulatorController* ctrl, const char* path) : emulCtrl(ctrl) {
		this->path = path;
		cart_state = std::make_shared<CartridgeState>();
		std::cout << "[CARTRIDGE] :: Created with path " << this->path << std::endl;
	};


	/**
	 * @brief Tries to load the cartridge from the defined path
	 * @return true If was able to load
	 * @return false If failed to load
	 */
	bool Cartridge::loadCartridgeFromFile() {
		std::fstream loadStream(path, std::ios::in | std::ifstream::ate | std::ifstream::binary);

		if(!loadStream.is_open()){
			std::cout << "[CARTRIDGE] :: Failed to load the cartridge from ( " <<
				path << " )" << std::endl;
			return false;
		}
		std::cout << "[CARTRIDGE] :: Valid cartridge path, reading ..." << std::endl;

		rom_size = static_cast<bit32>(loadStream.tellg());
		loadStream.seekg(0, std::ios_base::beg);

		char* temp_data(new char[rom_size + 1] {});
		loadStream.read(temp_data, rom_size);
		loadStream.close();
		
		temp_data[rom_size] = 0;

		memcpy(&cart_state->newCartType, temp_data + size_t(0x33), 1);

		rom_data = new bit8[rom_size] {};
		memcpy(rom_data, temp_data, rom_size);
		delete[] temp_data;

		assignCartData();
		bool checkSum = cartridgeCheckSum();

		std::cout << "[CARTRIDGE] :: " << (checkSum ?
			 "Valid Cartridge checksum result!" : "Failed on Checksum confirmation") << std::endl;

		return checkSum;
	}


	/**
	 * @brief Get the Cart Licensee Name object
	 * @return char* Cartridge Licensee Name
	 */
	const char* Cartridge::getCartLicenseeName() {
		if(LIC_CODE.count(cart_state->lic_code) != 0){
			if(isNewTypeCartridge())
				return const_cast<char*>(& *LIC_CODE.at(cart_state->old_lic_code).begin());

			return const_cast<char*>(& *LIC_CODE.at(cart_state->lic_code).begin());
		}
		return "UNDEFINED";
	}


	/**
	 * @brief Get the Cart Type Name object
	 * @return char* Cartridge Type name
	 */
	const char* Cartridge::getCartTypeName() {
		if(CARTRIDGE_TYPE.count(cart_state->cart_type) != 0){
			return &*CARTRIDGE_TYPE.at(cart_state->cart_type).begin();
		}
		return "UNDEFINED";
	}

	/**
	 * @brief Gets if the current cartridge is using a new layout
	 * @return true Is using the new layout 
	 * @return false Is using the old layout
	 */
	bool Cartridge::isNewTypeCartridge() {
		if(rom_size == 0) return false;
		return (cart_state->newCartType != 0x0);
	}


	/**
	 * @brief Reads data from the loaded cartridge
	 * @param addr Cartridge address
	 * @return bit8 Valeu on the defined address
	 */
	bit8 Cartridge::read(bit16 addr) {
		return rom_data[addr];
	}


	/**
	 * @brief Writes data to the defined cartridge address
	 * @param addr Target address
	 * @param val Value to be written
	 */
	void Cartridge::write(bit16 addr, bit8 val) {
#if VERBOSE
		std::cout <<"[CARTRIDGE] :: Cartridge write not implemented!" << std::endl;
#endif
		return;
	}


	/**
	 * @brief Prints the values from a loaded cartridge
	 */
	void Cartridge::printCartridgeValues() {
		char* msgBuffer(new char[256] {});

		sprintf_s(msgBuffer, 256,
			"-> Title     : %s\n"
			"-> Type      : %2.2X (%s)\n"
			"-> ROM Size  : %d KiB\n"
			"-> RAM Size  : %2.2X\n"
			"-> Lice Code : %2.2X (%s)\n"
			"-> ROM Vers  : %2.2X", cart_state->title, cart_state->cart_type, getCartTypeName(),
			0x20 << cart_state->rom_size, cart_state->ram_size, cart_state->lic_code, getCartLicenseeName(), cart_state->rom_version
		);


		emulCtrl->getView()->setCartInfo(msgBuffer);
		delete[] msgBuffer;
	}


	/**
	 * @brief Validates the 8bit checksum values on the $0134-014C header area
	 * @return true Valid cartridge
	 * @return false Invalid cartridge
	 */
	bool Cartridge::cartridgeCheckSum(){
		/* From the dbdev.io webpage, the header checksum function
		*	x = 0
		*	i = $0134
		*	while i <= $014C
		*		x = x - [i] - 1
		*/

		bit16 x = 0;
		for (bit16 i = 0x0134; i <= 0x014C; i++){
			x = x - rom_data[i] - 1;
		}

		char* msgBuf(new char[64] {});
		sprintf_s(msgBuf, 64, "[CARTRIDGE] :: Checksum Result : %2.2X (%X)\n", cart_state->checksum, (x & 0xFF));
		
		emulCtrl->getView()->setCartChecksum(msgBuf);
		delete[] msgBuf;

		return (x & 0xFF);
	}


	/**
	 * @brief Assigned the loaded data to the correct struct values
	 */
	void Cartridge::assignCartData(){
		memcpy(cart_state->entry, rom_data + size_t(0x100), 4);
		memcpy(cart_state->logo, rom_data + size_t(0x104), 48);
		memcpy(cart_state->title, rom_data + size_t(0x134), 16);
		memcpy(cart_state->manu_code, rom_data + size_t(0x13F), 4);
		memcpy(&cart_state->cgb_flag, rom_data + size_t(0x143), 1);
		memcpy(&cart_state->lic_code, rom_data + size_t(0x144), 2);
		memcpy(&cart_state->sgb_flag, rom_data + size_t(0x146), 1);
		memcpy(&cart_state->cart_type, rom_data + size_t(0x147), 1);
		memcpy(&cart_state->rom_size, rom_data + size_t(0x148), 1);
		memcpy(&cart_state->ram_size, rom_data + size_t(0x149), 1);
		memcpy(&cart_state->dest_code, rom_data + size_t(0x14A), 1);
		memcpy(&cart_state->old_lic_code, rom_data + size_t(0x14B), 1);
		memcpy(&cart_state->rom_version, rom_data + size_t(0x14C), 1);
		memcpy(&cart_state->h_checksum, rom_data + size_t(0x14D), 1);
		memcpy(&cart_state->checksum, rom_data + size_t(0x14E), 2);

		std::cout <<"[CARTRIDGE] :: Cartridge Headers has been mapped!" << std::endl;

		printCartridgeValues();
	}
}