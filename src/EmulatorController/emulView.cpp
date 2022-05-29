#include "emulView.h"

namespace TheBoy {

	/**
	 * @brief Construct a new Emul View:: Emul View object
	 * @param ctrl Target emulator controller
	 */
	EmulView::EmulView(EmulatorController* ctrl) : emulCtrl(ctrl) {
		window = std::make_shared<sf::RenderWindow>(
			sf::VideoMode(900, 500), "TheBoy Emulator"
		);

		std::cout << "[VIEW] :: Window created! " << std::endl;
	}

	/**
	 * @brief Manages the window events
	 */
	void EmulView::ManageEvents() {
		sf::Event evt;

		while (window->pollEvent(evt)) {
			switch (evt.type) {
			case sf::Event::Closed:
				emulCtrl->forceEmuStop("Close event found!");
				window->close();
				return;
			
			default:
				break;
			}
		}
	}


	/**
	 * @brief Updates the current window
	 */
	void EmulView::Draw(){
		window->clear(sf::Color::Black);
		window->display();
	}	
} // namespace TheBoy
