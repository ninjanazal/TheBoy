#include "emulatorController.h"
#include <SFML/Window.hpp>

namespace TheBoy {
	/**
	 * @brief Emulator internal loop
	 */
	void EmulatorController::_run() {
		std::cout << "[Emulator] ::: Starting the emulator update loop" << std::endl;

		while (emu_state.running) {
			manageEvents();

			comps.cpu->step();
			
			window->clear(sf::Color::Black);
			window->display();
		}
		
	}


	/**
	 * @brief Internal event manager function
	 */
	void EmulatorController::manageEvents() {
		sf::Event event;

		while (window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				emu_state.running = false;
				window->close();
				break;
			
			default:
				break;
			}
		}
	}


	/**
	 * @brief Construct a new Emulator Controller:: Emulator Controller object
	 * @param type Target emulation type
	 */
	EmulatorController::EmulatorController(EmuType type): emulationType(type) { 
			std::cout << "[Emulator] ::: controller was created" << std::endl;
		}


	/**
	 * @brief Initialize the Emulator with a defined size
	 * @param size Window size
	 */
	void EmulatorController::Start(const char* rom_path) {
		window = new sf::RenderWindow(
			sf::VideoMode(900, 500), "TheBoy Emulator"
		);
		
		comps.bus = std::make_shared<AddressBus>(AddressBus(this));
		comps.ram = std::make_shared<Ram>(Ram(this));

		comps.cpu = std::make_shared<Cpu>(Cpu(this));
		comps.cpu->setPCEntry(0x100);

		comps.cart = std::make_shared<Cartridge>(Cartridge(rom_path));
		if(!comps.cart->loadCartridgeFromFile()){
			std::cout << "[Emulator] ::: Fail to load cartridge!" << std::endl;
			return;
		}

		std::cout << "[Emulator] ::: Cartridge was loaded!" << std::endl;


		emu_state.reset();
		this->_run();
	}


	/**
	 * @brief Stops the emulation execution with a defined message
	 * @param msg Stop message
	 */
	void EmulatorController::forceEmuStop(const char* msg){
		emu_state.running = false;
		printf(msg);
		fflush(stdout);
	}


	/**
	 * @brief Emulates a defined number of cpu cycles
	 * @param cycles 
	 */
	void EmulatorController::emulCycles(const int& cycles) {
		// TODO : Implementation
	}


	/**
	 * @brief Get the Cartridge object
	 * @return std::shared_ptr<Cartridge> Shared pointer to the inUse cartridge
	 */
	std::shared_ptr<Cartridge> EmulatorController::getCartridge() {
		if(!comps.cart){
			std::cout << "[Emulator] ::: Get Cartridge on a null shared!" << std::endl;
		}
		return comps.cart;
	}

	/**
	 * @brief Get the Bus object
	 * @return std::shared_ptr<AddressBus> Shared pointer to the inUse AddressBus
	 */
	std::shared_ptr<AddressBus> EmulatorController::getBus() {
		if(!comps.bus) {
			std::cout << "[Emulator] ::: Get Bus on a null shared!" << std::endl;
		}
		return comps.bus;
	}

	/**
	 * @brief Get the Ram object
	 * @return std::shared_ptr<Ram> Shared pointer to the inUse Ram
	 */
	std::shared_ptr<Ram> EmulatorController::getRam() {
		if(!comps.ram) {
			std::cout << "[Emulator] ::: Get Ram on a null shared!" << std::endl;
		}
		return comps.ram;
	}
}

