#include <fstream>
#include <components/Cartridge.h>


namespace TheBoy {
	/**
	 * - - - - - - - - - - - - - - - - - - - -
	 *                  PUBLIC
	 * - - - - - - - - - - - - - - - - - - - -
	 */


	/**
	 * @brief Construct a new Cartridge object
	 * @param path Path to the cartridge to be loaded
	 */
	Cartridge::Cartridge(std::string path) :path(path) {
		std::cout << "[CARTRIDGE] :: Created" << std::endl;
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

		rom_size = loadStream.tellg();
		loadStream.seekg(0, std::ios_base::beg);

		char *temp_data(new char[rom_size] {});
		
		loadStream.read(temp_data, rom_size);
		loadStream.close();

		rom_data = std::make_shared<bit8>(*temp_data);
		delete[] temp_data;

		cart_state = (CartridgeState *)(rom_data.get() + 0x100);
		cart_state->title[15] = 0;
		
		printCartridgeValues();
		bool checkSum = cartridgeCheckSum();

		std::cout << (checkSum ?
			 "Valid Cartridge checksum result!" : "Failed on Checksum confirmation") <<
		std::endl;

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
	const char *Cartridge::getCartTypeName() {
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
		std::cout << "[CARTRIDGE] :: Cartridge Information" << std::endl <<
		("\t Title 		:	%s", cart_state->title) << std::endl <<
		("\t Type 		:	%2.2X (%s)", cart_state->cart_type, getCartTypeName()) << std::endl <<
		("\t ROM Size	:	%d KiB", 0x20 << cart_state->rom_size) << std::endl <<
		("\t RAM Size	:	%2.2X", cart_state->ram_size) << std::endl <<
		("\t Lice Code	:	%2.2X (%s)", cart_state->lic_code, getCartLicenseeName()) << std::endl <<
		("\t ROM Vers	: 	%2.2X", cart_state->rom_version) << std::endl;
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
			x = x - rom_data.get()[i] - 1;
		}

		std::cout << ("[CARTRIDGE] :: Checksum Result : %2.2X (%2.2X)", cart_state->checksum, (x & 0xFF));
		return (x & 0xFF);
	}
}