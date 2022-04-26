#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <common.h>

namespace TheBoy {

	typedef struct {
		bit8 entry[4];
		bit8 logo[0x30];

		char title[16];
	};
	

	/**
	 * @brief Holds the cartridge logic information and definitions
	 */
	class Cartridge	{
	private:


	public:
		Cartridge(/* args */);
		~Cartridge() = default;
	};
	
	

	
} // namespace TheBoy



#endif