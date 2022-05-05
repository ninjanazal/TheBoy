#include "emulatorController.h"
#include <SFML/Window.hpp>

namespace TheBoy {
	/**
	 * @brief Emulator internal loop
	 */
	void EmulatorController::_run() {
		std::cout << "[Emulator] ::: Starting the emulator update loop" << std::endl;

		while (emu_state.running) {
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
			
			window->clear(sf::Color::Black);
			window->display();
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
		
		comps.bus = std::make_shared<AddressBus>(AddressBus(*this));
		std::cout << "[Emulator] ::: AddressBus Created!" << std::endl;


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
	 * @brief Get the Cartridge object
	 * @return std::shared_ptr<Cartridge> Shared pointer to the inUse cartridge
	 */
	std::shared_ptr<Cartridge> EmulatorController::getCartridge() {
		if(!comps.cart){
			std::cout << "[Emulator] ::: Get Cartridge on a null shared!" << std::endl;
		}
		return comps.cart;
	}
}

