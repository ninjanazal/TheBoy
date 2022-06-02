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

		mainLoad();
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
		for (int i = 0; i < (sizeof(wText)/sizeof(wText[0])); i++) {
			window->draw(*wText[i].get());
		}
		window->display();
	}


	/**
	 * @brief Loads all the needed objects to memory
	 */
	void EmulView::mainLoad() {
		winSize = window->getSize();

		wIcon = std::make_shared<sf::Image>();
		wFont = std::make_shared<sf::Font>();
		
		if(!wIcon->loadFromFile("assets\\icons\\MaBoy_EmuIcon.png")){
			std::cout << "[VIEW] :: Failed to load window icon! " << std::endl;
		}

		if(!wFont->loadFromFile("assets\fonts\\JetBrainsMono-Regular.ttf")){
			std::cout << "[VIEW] :: Failed to load window icon! " << std::endl;
		}

		window->setIcon(254, 254, wIcon->getPixelsPtr());

		// init all the window Texts
		for (int i = 0; i < (sizeof(wText)/sizeof(wText[0])); i++) {
			wText[i] = std::make_shared<sf::Text>();
		}
		
	}


} // namespace TheBoy
