#ifndef EMULATORCONTROLLER_H
#define EMULATORCONTROLLER_H

#include <iostream>

#include <SFML/Graphics.hpp>
#include "Cartridge.h"
#include "cpu.h"
#include "ram.h"

/**
 * @brief Core Project Namespace 
 */
namespace TheBoy {
	class AddressBus;
	class Cpu;
	class Ram;

	
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

		/**
		 * @brief Emulator cpu object
		 */
		std::shared_ptr<Cpu> cpu;

		/**
		 * @brief Emulator Ram object
		 */
		std::shared_ptr<Ram> ram;

	} EmulatorComponents;
	


	/**
	 * @brief Base emulator controller class
	 */
	class EmulatorController {
	private:
	
		/**
		 * @brief Pointer to the inUse window
		 */
		std::shared_ptr<sf::RenderWindow> window;


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

		/**
		 * @brief Internal event manager function
		 */
		void manageEvents();

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
		 * @brief Stops the emulation execution with a defined message
		 * @param msg Stop message
		 */
		void forceEmuStop(const char* msg);

		/**
		 * @brief Emulates a defined number of cpu cycles
		 * @param cycles 
		 */
		void emulCycles(const int& cycles);
	

		/**
		 * @brief Get the Cartridge object
		 * @return std::shared_ptr<Cartridge> Shared pointer to the inUse cartridge
		 */
		std::shared_ptr<Cartridge> getCartridge();


		/**
		 * @brief Get the Bus object
		 * @return std::shared_ptr<AddressBus> Shared pointer to the inUse AddressBus
		 */
		std::shared_ptr<AddressBus> getBus();


		/**
		 * @brief Get the Ram object
		 * @return std::shared_ptr<Ram> Shared pointer to the inUse Ram
		 */
		std::shared_ptr<Ram> getRam();


		/**
		 * @brief Get the Cpu object
		 * @return std::shared_ptr<Cpu> Shared pointer to the inUse Cpu
		 */
		std::shared_ptr<Cpu> getCpu();
	};
	
} // namespace TheBoy


#endif