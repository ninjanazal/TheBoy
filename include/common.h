#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <map>
#include <cstdint>
#include <memory>

namespace TheBoy{

	// - - - - - - - - - - - - - - - - - - - -
	//                 MACROS               //
	// - - - - - - - - - - - - - - - - - - - -


	/**
	 * @brief Defines the GameBoy ScreenSize
	 */
	#define BASE_SCREEN (int[]){160, 144}


	// - - - - - - - - - - - - - - - - - - - -
	//               TypeDefs               //
	// - - - - - - - - - - - - - - - - - - - -


	/**
	 * @brief Defined types for the current project type
	 * Available types bit8(uint8_t), bit16(uint16_t), bit32(uint32_t) and bit64(uint_64_t)
	 */
	typedef uint8_t bit8;
	typedef uint16_t bit16;
	typedef uint32_t bit32;
	typedef uint64_t bit64;


	/**
	 * @brief Defines teh Emulator state
	 */
	typedef struct {
		/**
		 * @brief Marks if the current emulator is paused
		 */
		bool paused;


		/**
		 * @brief marks if the current emulator is running
		 */
		bool running;


		/**
		 * @brief Holds the processor/Timer ticks count
		 */
		bit64 ticks;

		/**
		 * @brief Resets the Emulator state values
		 */
		void reset(){
			paused = false;
			running = true;
			ticks = 0;
		};

	} EmulatorState;



	// - - - - - - - - - - - - - - - - - - - -
	//             Enumerations             //
	// - - - - - - - - - - - - - - - - - - - -


	/**
	 * @brief Enumeration holding the availabe Emulator types
	 */
	enum EmuType {
		//TODO GBBase,
		GBColor
	};


}

#endif