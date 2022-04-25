#ifndef EMULATORCONTROLLER_H
#define EMULATORCONTROLLER_H

#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>
#include "globalTypes.h"

/**
 * @brief Core Project Namespace 
 */
namespace TheBoy {

	/**
	 * @brief Base emulator controller class
	 */
	class EmulatorController {
	private:
	
		/**
		 * @brief Pointer to the inUse window
		 */
		sf::RenderWindow *window;


		/**
		 * @brief Holds the inUse emulation type
		 */
		EmuType emulationType;


		/**
		 * @brief Defined window size
		 */
		sf::Vector2u *winSize = new sf::Vector2u();

	private:

		/**
		 * @brief Emulator internal loop
		 */
		void _run();

	public:
		/**
		 * @brief Construct a new Emulator Controller object
		 * @param type Target emulation type
		 */
		EmulatorController(EmuType type);

		/**
		 * @brief Destroy the Emulator Controller object
		 */
		~EmulatorController() = default;

		/**
		 * @brief Initialize the Emulator with a defined size
		 * @param size Window size
		 */
		void Start(sf::Vector2u *size);
	};
	
} // namespace TheBoy


#endif