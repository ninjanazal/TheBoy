#include "Controllers/emulatorController.h"

namespace TheBoy {
	/**
	 * @brief Construct a new Emulator Controller:: Emulator Controller object
	 * @param type Target emulation type
	 */
	EmulatorController::EmulatorController(EmuType type):
		emulationType(type) { 
			std::cout << "Emulator controller was created" << std::endl;
		}


	/**
	 * @brief Initialize the Emulator with a defined size
	 * @param size Window size
	 */
	void EmulatorController::Start(sf::Vector2u *size) {
		winSize = size;

		window = new sf::RenderWindow(sf::VideoMode(winSize->x, winSize->y), "TheBoy Emulator");
		//window->create(sf::VideoMode(winSize->x, winSize->y), "TheBoy Emulator");
		this->_run();
	}


	/**
	 * @brief Emulator internal loop
	 */
	void EmulatorController::_run() {
		while (window->isOpen()) {
			window->clear(sf::Color::Black);
			window->display();
		}
		
	}
}

