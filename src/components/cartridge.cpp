#include <fstream>
#include <components/Cartridge.h>


namespace TheBoy {
	/**
	 * @brief Construct a new Cartridge object
	 * @param path Path to the cartridge to be loaded
	 */
	Cartridge::Cartridge(std::string path) :path(path) {

	};


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
}