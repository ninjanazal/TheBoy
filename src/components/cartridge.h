#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "collections.h"

namespace TheBoy {

	/**
	 * @brief Defines the Cartridge state
	 */
	typedef struct {
		bit8 newCartType;	// New layout in use	(0x033)
		bit8 entry[4];		// Entry Point 			(0x100 -> 0x0103)
		bit8 logo[0x30]; 	// Nintendo Logo 		(0x0104 -> 0x0133)
		char title[16];		// game Title 			(0x0134 -> 0x0143)
		char manu_code[4]; 	// Manufacturer code 	(0x013F -> 0x0142)
		bit8 cgb_flag; 		// CGB flag 			(0x0143)
		bit16 lic_code; 	// New Licensse Code 	(0x0144 -> 0x0145)
		bit8 sgb_flag; 		// SGB flag 			(0x0146)
		bit8 cart_type;		// cartridge type		(0x0147)
		bit8 rom_size;		// ROM size				(0x0148)
		bit8 ram_size;		// RAM size				(0x0149)
		bit8 dest_code;		// Destination Code		(0x014A)
		bit8 old_lic_code;	// Old Licensee Code	(0x014B)
		bit8 rom_version;	// ROM Version number	(0x014C)
		bit8 h_checksum;	// header checksumn		(0x014D)
		bit16 checksum;		// Global Checksum		(0x014E -> 0x014F)
	} CartridgeState;
	

	/**
	 * @brief Holds the cartridge logic information and definitions
	 */
	class Cartridge	{
	public:
		/**
		 * @brief Construct a new Cartridge object
		 * @param path Path to the cartridge to be loaded
		 */
		Cartridge(const char* path);


		/**
		 * @brief Destroy the Cartridge object
		 */
		~Cartridge() = default;


		/**
		 * @brief Tries to load the cartridge from the defined path
		 * @return true If was able to load
		 * @return false If failed to load
		 */
		bool loadCartridgeFromFile();


		/**
		 * @brief Get the Cart Licensee Name object
		 * @return char* Cartridge Licensee Name
		 */
		const char* getCartLicenseeName();


		/**
		 * @brief Get the Cart Type Name object
		 * @return char* Cartridge Type name
		 */
		const char* getCartTypeName();


		/**
		 * @brief Gets if the current cartridge is using a new layout
		 * @return true Is using the new layout 
		 * @return false Is using the old layout
		 */
		bool isNewTypeCartridge();

	private:
		/**
		 * @brief Holds the cartridge path
		 */
		const char* path;

		/**
		 * @brief Holds the current rom Size (kiBytes)
		 */
		bit32 rom_size = 0;

		
		/**
		 * @brief Pointer to the loaded rom byte data
		 */
		//bit8 rom_data;
		bit8* rom_data = nullptr;


		/**
		 * @brief Holds the current cartridge state
		 */
		std::shared_ptr<CartridgeState> cart_state;


		/**
		 * @brief Prints the values from a loaded cartridge
		 */
		void printCartridgeValues();

		/**
		 * @brief Validates the 8bit checksum values on the $0134-014C header area
		 * @return true Valid cartridge
		 * @return false Invalid cartridge
		 */
		bool cartridgeCheckSum();

		/**
		 * @brief Assigned the loaded data to the correct struct values
		 */
		void assignCartData();
	};
} // namespace TheBoy
#endif