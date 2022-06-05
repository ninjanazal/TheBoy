#ifndef EMULATORCONTROLLER_H
#define EMULATORCONTROLLER_H

#include <iostream>

#include <SFML/Graphics.hpp>
#include "Cartridge.h"
#include "cpu.h"
#include "ram.h"
#include "ppu.h"
#include "io.h"
#include "timer.h"

#include "emulView.h"

/**
 * @brief Core Project Namespace 
 */
namespace TheBoy {
	class AddressBus;
	class Cpu;
	class Ram;
	class Ppu;
	class IO;
	class Timer;
	class EmulView;

	
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

		/**
		 * @brief Emulator Ppu object
		 */
		std::shared_ptr<Ppu> ppu;


		/**
		 * @brief Emulator IO object
		 */
		std::shared_ptr<IO> io;


		/**
		 * @brief Emulator Timer object
		 */
		std::shared_ptr<Timer> timer;

		/**
		 * @brief Current emulator controller view
		 */
		std::shared_ptr<EmulView> view;

	} EmulatorComponents;
	


	/**
	 * @brief Base emulator controller class
	 */
	class EmulatorController {
	private:
	

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
		//void manageEvents();

	public:
		/**
		 * @brief Construct a new Emulator Controller object
		 */
		EmulatorController();

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
		 * @brief Get the Ppu object
		 * @return std::shared_ptr<Ppu> Shared pointer to the inUse Ppu
		 */
		std::shared_ptr<Ppu> getPpu();


		/**
		 * @brief Get the Cpu object
		 * @return std::shared_ptr<Cpu> Shared pointer to the inUse Cpu
		 */
		std::shared_ptr<Cpu> getCpu();

		/**
		 * @brief Gets the IO object
		 * @return std::shared_ptr<IO> Shared pointer to the inUse IO
		 */
		std::shared_ptr<IO> getIO();


		/**
		 * @brief Get the Timer object
		 * @return std::shared_ptr<Timer> Shared pointer to the inUse Timer
		 */
		std::shared_ptr<Timer> getTimer();


		/**
		 * @brief Get the Timer object
		 * @return std::shared_ptr<Timer> Shared pointer to the inUse Timer
		 */
		std::shared_ptr<EmulView> getView();
	};
	
} // namespace TheBoy


#endif