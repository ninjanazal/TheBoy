#include "emulView.h"
#include "emulatorController.h"
#include <math.h>

namespace TheBoy {

	/**
	 * @brief Construct a new Emul View:: Emul View object
	 * @param ctrl Target emulator controller
	 */
	EmulView::EmulView(EmulatorController* ctrl) : emulCtrl(ctrl) {
		window = std::make_unique<sf::RenderWindow>(
			sf::VideoMode(900, 500), "TheBoy Emulator",
			sf::Style::Titlebar | sf::Style::Titlebar | sf::Style::Close
			);

		viewInput = new InputStates();

		window->setFramerateLimit(30);
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
		delete viewInput;
	}


	/**
	 * @brief Manages the window events
	 */
	void EmulView::ManageEvents() {
		sf::Event* evt = new sf::Event();

		while (window->pollEvent(*evt)) {
			switch (evt->type) {
			case sf::Event::Closed:
				emulCtrl->forceEmuStop("Close event found!");
				window->close();
				return;

			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased: {
				switch (evt->key.code)
				{
				case sf::Keyboard::A: {
					viewInput->a = evt->type == sf::Event::KeyPressed; break; }
				case sf::Keyboard::Z: {
					viewInput->b = evt->type == sf::Event::KeyPressed; break; }
				case sf::Keyboard::Enter: {
					viewInput->start = evt->type == sf::Event::KeyPressed; break; }
				case sf::Keyboard::Backspace: {
					viewInput->select = evt->type == sf::Event::KeyPressed; break; }
				case sf::Keyboard::Up: {
					viewInput->up = evt->type == sf::Event::KeyPressed; break; }
				case sf::Keyboard::Down: {
					viewInput->down = evt->type == sf::Event::KeyPressed; break; }
				case sf::Keyboard::Left: {
					viewInput->left = evt->type == sf::Event::KeyPressed; break; }
				case sf::Keyboard::Right: {
					viewInput->right = evt->type == sf::Event::KeyPressed; break; }
				default:break;
				}
				return;
			}
			default:
				break;
			}
		}
		delete evt;
	}


	/**
	 * @brief Updates the current window
	 */
	void EmulView::Draw() {
		window->clear(sf::Color::Black);

		setRegistorsVals();
		positionTextElms();
		buildDebugView();
		buildOutView();

		for (int i = 0; i < (sizeof(wText) / sizeof(wText[0])); i++) {
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


	/// <summary>
	/// Defines the Ppu frame count on change
	/// </summary>
	/// <param name="inf"></param>
	void EmulView::setPpuFrameCount(const char* inf) {
		wText[6]->setString(inf);
	}


	/**
	 * @brief Set the Registors Vals on screen
	 * @param regs Target CPU registors
	*/
	void EmulView::setRegistorsVals() {
		char* regBuffer(new char[256] {});
		char* opBuffer(new char[64] {});

		emulCtrl->getCpu()->getCpuSummary(regBuffer, opBuffer);

		wText[4]->setString(regBuffer);
		wText[5]->setString(opBuffer);
		delete[] regBuffer;
		delete[] opBuffer;
	}


	/**
	 * @brief Loads all the needed objects to memory
	 */
	void EmulView::mainLoad() {
		winSize = window->getSize();


		// 384 = 24 * 16 disposal
		iGRam = std::make_shared<sf::Image>();
		iGRam->create(tileSizeView.x * 8, tileSizeView.y * 8, sf::Color::Black);


		// Ram representation
		tGRam = std::make_shared<sf::Texture>();
		tGRam->loadFromImage(*iGRam.get());

		sGRam = std::make_shared<sf::Sprite>();
		sGRam->setTexture(*tGRam.get());
		sGRam->setScale(1.5f, 1.5f);


		// OutPut view
		viewPixels = new sf::Uint8[kBaseScreen[0] * kBaseScreen[1] * 4]{};

		iView = std::make_shared <sf::Image>();
		iView->create(Ppu::xRes, Ppu::yRes, sf::Color::White);

		tView = std::make_shared<sf::Texture>();
		tView->loadFromImage(*iView.get());


		sView = std::make_shared<sf::Sprite>();
		sView->setTexture(*tView.get());
		sView->setScale(3.25f, 3.25f);
		sView->setPosition(sf::Vector2(15.f, 15.f));

		wIcon = std::make_unique <sf::Image>();
		wFont = std::make_shared<sf::Font>();

		if (!wIcon->loadFromFile("assets\\icons\\MaBoy_EmuIcon_32.png")) {
			std::cout << "[VIEW] :: Failed to load window icon! " << std::endl;
		}
		else { window->setIcon(32, 32, wIcon->getPixelsPtr()); }

		if (!wFont->loadFromFile("assets\\fonts\\JetBrainsMono-Regular.ttf")) {
			std::cout << "[VIEW] :: Failed to load window icon! " << std::endl;
		}

		for (int i = 0; i < (sizeof(wText) / sizeof(wText[0])); i++) {
			wText[i] = std::make_shared<sf::Text>();
			wText[i]->setFont(*wFont.get());
			wText[i]->setCharacterSize(12);
			wText[i]->setString(" - ");
		}
	}


	/**
	 * @brief Updates the text elements positions
	 */
	void EmulView::positionTextElms() {
		for (int i = 0; i < (sizeof(wText) / sizeof(wText[0])); i++) {
			wText[i]->setPosition(
				winSize.x * 0.65f,
				(i == 0) ? 0.0f : wText[i - 1]->getGlobalBounds().top + wText[i - 1]->getGlobalBounds().height
			);
		}
		sGRam->setPosition(
			winSize.x * 0.65f,
			// calculate the bottom position for the last text element
			wText[(sizeof(wText) / sizeof(wText[0])) - 1]->getGlobalBounds().top +
			wText[(sizeof(wText) / sizeof(wText[0])) - 1]->getGlobalBounds().height + 10
		);
	}

	/**
	 * @brief Updates the debug view texture
	 */
	void EmulView::buildDebugView() {
		/*
			Tile data is stored in VRAM in the memory area at $8000-$97FF;
			with each tile taking 16 bytes, this area defines data for 384 tiles
			Each tile has 8x8 pixels and has a color depth of 4 colors/gray shades
			384 = 16 * 24 disposal
		*/
		bit16 addr = 0x8000;
		// Making the display table
		//iGRam->create(tileSizeView.x * 8, tileSizeView.y * 8, sf::Color::White);
		for (unsigned int y = 0; y < tileSizeView.y; y++) {
			for (unsigned int x = 0; x < tileSizeView.x; x++) {
				addTileToDebug(addr, ((y * 24) + x));
			}
		}
		tGRam->update(*iGRam.get());
	}


	/**
	 * @brief Adds the target mem tile to the debug pixel array
	 * @param addr Target reading addressGETBIT(byte2, b)
	 * @param tileId Current tile iD
	 */
	void EmulView::addTileToDebug(bit16 addr, int tileId) {
		/*
			with each tile taking 16 bytes
			Each tile occupies 16 bytes, where each line is represented by 2 bytes
			For each line, the first byte specifies the least significant bit of the color ID of each pixel,
			and the second byte specifies the most significant bit. In both bytes, bit 7 represents the leftmost pixel,
			and bit 0 the rightmost.

		*/
		int yTileOff = static_cast<int>(floor(tileId / tileSizeView.x));
		int xTileOff = tileId - (yTileOff * tileSizeView.x);

		for (int t = 0; t < 16; t += 2) {
			// For each tile line, needs to be readed 2 bytes
			// High and Low tile line value
			bit8 byte1 = emulCtrl->getBus()->abRead(addr + (tileId * 16) + t);
			bit8 byte2 = emulCtrl->getBus()->abRead(addr + (tileId * 16) + t + 1);

			// for each bit on the gathered line, the low nib value is the most significat one and vice versa
			// Place the target bit value from the 1st byte on the left of the target bit from the 2st byte
			for (int b = 0; b < 8; b++) {
				bit8 pixelID = (GETBIT(byte1, b) << 1) | GETBIT(byte2, b);
				// Pixel ID makes the target pixel color value, for the gb is 1 of 4 colors
				// Since each pixel needs a 4 value entrance
				// 64 pixels per tile

				iGRam->setPixel(
					(8 * xTileOff) + (7 - b),
					(8 * yTileOff) + static_cast<int>(floor(t / 2)),
					sf::Color(gbPallet[pixelID])
				);
			}
		}
	}

	/// <summary>
	/// Creates the output view
	/// </summary>
	void EmulView::buildOutView() {
		for (int row = 0; row < Ppu::yRes; row++) {
			for (int line = 0; line < Ppu::xRes; line++) {
				iView->setPixel(
					line, row,
					sf::Color(emulCtrl->getPpu()->getPpuBuffer()[line + (Ppu::xRes * row)])
				);
			}
		}
		tView->update(*iView.get());
	}

	/// <summary>
	/// Get the registed input
	/// </summary>
	/// <returns>Registed input values</returns>
	InputStates* EmulView::getInputState() {
		return viewInput;
	}
} // namespace TheBoy
