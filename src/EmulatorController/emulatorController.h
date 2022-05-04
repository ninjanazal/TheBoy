#ifndef EMULATORCONTROLLER_H
#define EMULATORCONTROLLER_H

#include <iostream>

#include <SFML/Graphics.hpp>
#include "Cartridge.h"
#include "addressbus.h"

/**
 * @brief Core Project Namespace 
 */
namespace TheBoy {
	class AddressBus;

	
	/**
	 * @brief Defines the Emulator components
	 */
	typedef struct {
		/**
		 * @brief Holds reference to the current cartriddge
		 */
		std::shared_ptr<Cartridge> cart;

		/**
		 * @brief Emulator Address bus object
		 */
		std::shared_ptr<AddressBus> bus;

	} EmulatorComponents;
	


	/**
	 * @brief Base emulator controller class
	 */
	class EmulatorController {
	private:
	
		/**
		 * @brief Pointer to the inUse window
		 */
		sf::RenderWindow* window;


		/**
		 * @brief Holds the inUse emulation type
		 */
		EmuType emulationType;

		/**
		 * @brief Holds the current emulator state values
		 */
		EmulatorState emu_state;

		/**
		 * @brief Holds references for the Emulator controller components
		 */
		EmulatorComponents comps;


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
		 * @param rom_path Path to the target rom
		 */
		void Start(const char* rom_path);

		/**
		 * @brief Get the Cartridge object
		 * @return std::shared_ptr<Cartridge> Shared pointer to the inUse cartridge
		 */
		std::shared_ptr<Cartridge> getCartridge();
	};
	
} // namespace TheBoy


#endif