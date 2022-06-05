#include "emulView.h"
#include "emulatorController.h"

namespace TheBoy {

	/**
	 * @brief Construct a new Emul View:: Emul View object
	 * @param ctrl Target emulator controller
	 */
	EmulView::EmulView(EmulatorController* ctrl) : emulCtrl(ctrl) {
		window = std::make_shared<sf::RenderWindow>(
			sf::VideoMode(900, 500), "TheBoy Emulator",
			sf::Style::Titlebar | sf::Style::Titlebar | sf::Style::Close
		);

		mainLoad();
		wText[0]->setString("|TheBoy - Game boy Emulator\n - - - - - - - - - - -\n");
		wText[1]->setString("|:: Cartrige Information");

		std::cout << "[VIEW] :: Window created! " << std::endl;
	}


	/**
	 * 
	 * @brief Destroy the Emul View object
	 */
	EmulView::~EmulView() {
		delete[] viewPixels;
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
		positionTextElms();

		for (int i = 0; i < (sizeof(wText)/sizeof(wText[0])); i++) {
			window->draw(*wText[i].get());
		}
		window->draw(*sGRam.get());
		window->draw(*sView.get());

		window->display();
	}


	/**
	 * @brief Set the Cart Informations on screen
	 * @param inf information String
	 */
	void EmulView::setCartInfo(const char* inf) {
		wText[2]->setString(inf);
	}


	/**
	 * @brief Set the Cart Checksum value on screen
	 * @param inf Checksum result string
	 */
	void EmulView::setCartChecksum(const char* inf) {
		wText[3]->setString(inf);
	}


	/**
	 * @brief Set the Registors Vals on screen
	 * @param regs Target CPU registors
	*/
	void EmulView::setRegistorsVals(const Registers* regs){
		char* msgBuffer(new char[256] {});
		sprintf(msgBuffer, 
				"|:: Registors state\n" 
				"A: %2.2X F: %2.2X\n" 
				"BC: %2.2X %2.2X DE: %2.2X %2.2X HL: %2.2X %2.2X\n" 
				"SP: %4.4X PC %4.4X",
				regs->A, regs->F,
				regs->B, regs->C, regs->D, regs->E, regs->H, regs->L,
				regs->SP, regs->PC
		);
		wText[4]->setString(msgBuffer);
		delete[] msgBuffer;
	}


	/**
	 * @brief Set the Curr Operation information on screen
	 * @param inf Current operation info
	 */
	void EmulView::setCurrOperation(const char* inf) {
		wText[5]->setString(inf);
	}



	/**
	 * @brief Loads all the needed objects to memory
	 */
	void EmulView::mainLoad() {
		winSize = window->getSize();

		tGRam = std::make_shared<sf::Texture>();
		sGRam = std::make_shared<sf::Sprite>(*tGRam.get());


		viewPixels = new sf::Uint8[BASE_SCREEN[0] * BASE_SCREEN[1] * 4] {};

		tView = std::make_shared<sf::Texture>();
		tView->create(BASE_SCREEN[0], BASE_SCREEN[1]);
		tView->update(viewPixels);
		
		sView = std::make_shared<sf::Sprite>(*tView.get());		


		wIcon = std::make_shared<sf::Image>();
		wFont = std::make_shared<sf::Font>();
		
		if(!wIcon->loadFromFile("assets\\icons\\MaBoy_EmuIcon_32.png")){
			std::cout << "[VIEW] :: Failed to load window icon! " << std::endl;
		}
		window->setIcon(32, 32, wIcon->getPixelsPtr());

		if(!wFont->loadFromFile("assets\\fonts\\JetBrainsMono-Regular.ttf")){
			std::cout << "[VIEW] :: Failed to load window icon! " << std::endl;
		}

		for (int i = 0; i < (sizeof(wText)/sizeof(wText[0])); i++) {
			wText[i] = std::make_shared<sf::Text>();
			wText[i]->setFont(*wFont.get());
			wText[i]->setCharacterSize(12);
			wText[i]->setString(" - ");
		}
	}


	/**
	 * @brief Updates the text elements positions
	 */
	void EmulView::positionTextElms(){
		for(int i = 0; i < (sizeof(wText)/sizeof(wText[0])); i++){
			wText[i]->setPosition(
				winSize.x * 0.65f,
				(i == 0) ? 0.0f : wText[i - 1]->getGlobalBounds().top + wText[i - 1]->getGlobalBounds().height
			);
		}
	}
} // namespace TheBoy
