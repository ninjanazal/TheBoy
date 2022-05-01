#include <fstream>
#include <iomanip>
#include <cstring>
#include "cartridge.h"


namespace TheBoy {
	/**
	 * @brief Construct a new Cartridge object
	 * @param path Path to the cartridge to be loaded
	 */
	Cartridge::Cartridge(const char* path) {
		this->path = path;
		std::cout << "[CARTRIDGE] :: Created with path " << this->path << std::endl;
	};

	/**
	 * @brief Destroy the Cartridge object
	 */
	Cartridge::~Cartridge(){
		//delete path;
		//delete rom_data;
	}


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

		rom_size = loadStream.tellg();
		loadStream.seekg(0, std::ios_base::beg);

		char* temp_data(new char[rom_size + size_t(1)] {});
		loadStream.read(temp_data, rom_size);
		loadStream.close();
		
		temp_data[rom_size] = 0;

		rom_data = new bit8[rom_size] {};
		std::memcpy(rom_data, (temp_data + size_t(1) * 0x100), rom_size);
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
			return &*LIC_CODE.at(cart_state->lic_code).begin();
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
	 * - - - - - - - - - - - - - - - - - - - -
	 *                  PUBLIC
	 * - - - - - - - - - - - - - - - - - - - -
	 */

	/**
	 * @brief Prints the values from a loaded cartridge
	 */
	void Cartridge::printCartridgeValues() {
		std::cout <<"[CARTRIDGE] :: Cartridge Information" << std::endl;
		std::printf("\t Title 		:	%s\n", cart_state->title);
		std::printf("\t Type 		:	%2.2X (%s)\n", cart_state->cart_type, getCartTypeName());
		std::printf("\t ROM Size	:	%d KiB\n", 0x20 << cart_state->rom_size);
		std::printf("\t RAM Size	:	%2.2X\n", cart_state->ram_size);
		std::printf("\t Lice Code	:	%2.2X (%s)\n", cart_state->lic_code, getCartLicenseeName());
		std::printf("\t ROM Vers	: 	%2.2X\n", cart_state->rom_version);
		fflush(stdout);
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

		std::printf("[CARTRIDGE] :: Checksum Result : %2.2X (%X)\n", cart_state->checksum, (x & 0xFF));
		fflush(stdout);
		return (x & 0xFF);
	}


	/**
	 * @brief Assigned the loaded data to the correct struct values
	 */
	void Cartridge::assignCartData(){
		cart_state = std::make_shared<CartridgeState>();

		std::memcpy(cart_state->entry, rom_data, size_t(0x4));
		std::memcpy(cart_state->logo, rom_data + size_t(0x4), 0x30);
		std::memcpy(cart_state->title, rom_data + size_t(0x34), 0x10);
		std::memcpy(cart_state->manu_code, rom_data + size_t(0x3F), 0x4);
		printCartridgeValues();
	}
}