#include "emulatorController.h"
#include <SFML/Window.hpp>

namespace TheBoy {
	/**
	 * @brief Construct a new Emulator Controller:: Emulator Controller object
	 * @param type Target emulation type
	 */
	EmulatorController::EmulatorController(EmuType type): emulationType(type) { 
			std::cout << "Emulator controller was created" << std::endl;
		}


	/**
	 * @brief Initialize the Emulator with a defined size
	 * @param size Window size
	 */
	void EmulatorController::Start(const char* rom_path) {
		window = new sf::RenderWindow(
			sf::VideoMode(900, 500), "TheBoy Emulator"
		);
		cart = std::make_shared<Cartridge>(Cartridge(rom_path));
		cart->loadCartridgeFromFile();

		emu_state.reset();
		this->_run();
	}


	/**
	 * @brief Emulator internal loop
	 */
	void EmulatorController::_run() {
		std::cout << "Starting the emulator update loop" << std::endl;

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
}

