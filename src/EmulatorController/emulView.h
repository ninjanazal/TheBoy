#ifndef EMULVIEW_H
#define EMULVIEW_H

#include <SFML/System.hpp>
#include "emulatorController.h"
#include "cpu.h"

namespace TheBoy {
	struct InputStates;

	class EmulView {
	public:
		/**
		 * @brief Construct a new Emul View object
		 * @param ctrl Reference to the target emulator controller
		 */
		EmulView(EmulatorController* ctrl);


		/**
		 * @brief Destroy the Emul View object
		 */
		~EmulView();


		/**
		 * @brief Manages the window events
		 */
		void ManageEvents();


		/**
		 * @brief Updates the current window
		 */
		void Draw();

		/**
		 * @brief Set the Cart Informations on screen
		 * @param inf information String
		 */
		void setCartInfo(const char* inf);


		/**
		 * @brief Set the Cart Checksum value on screen
		 * @param inf Checksum result string
		 */
		void setCartChecksum(const char* inf);


		/// <summary>
		/// Defines the Ppu frame count on
		/// </summary>
		/// <param name="inf"></param>
		void setPpuFrameCount(const char* inf);

		/// <summary>
		/// Get the registed input
		/// </summary>
		/// <returns>Registed input values</returns>
		InputStates* getInputState();

	private:
#pragma region Properties
		/**
		 * @brief Pointer to the target emulator controller
		 */
		EmulatorController* emulCtrl;


		/**
		 * @brief Holds the current window size
		 */
		sf::Vector2u winSize;

		/**
		 * @brief Defined vRam Tile arrangement
		 */
		const sf::Vector2u tileSizeView = sf::Vector2u(24, 16);


		/**
		 * @brief Pointer to the inUse window
		 */
		std::unique_ptr<sf::RenderWindow> window;


		/**
		 * @brief Window icon texture
		 */
		std::unique_ptr<sf::Image> wIcon;


		/**
		 * @brief Debug font used
		 */
		std::shared_ptr<sf::Font> wFont;


		/**
		 * @brief Array with the debug text elements
		 */
		std::shared_ptr<sf::Text> wText[7];



		/**
		 * @brief Buffer holding the current
		 */
		sf::Uint8* viewPixels;


		/**
		 * @brief Current vRam image representation
		 */
		std::shared_ptr<sf::Image> iGRam;


		/**
		 * @brief Current visual output image
		 */
		std::shared_ptr<sf::Image> iView;



		/**
		 * @brief Curent vRam graphical representation
		 */
		std::shared_ptr<sf::Texture> tGRam;


		/**
		 * @brief Current visual output
		 */
		std::shared_ptr<sf::Texture> tView;


		/**
		 * @brief Current graphic vRam sprite representation
		 */
		std::shared_ptr<sf::Sprite> sGRam;


		/**
		 * @brief Current visual output sprite
		 */
		std::shared_ptr<sf::Sprite> sView;


		// GameBoy color pallet representation
		bit32 gbPallet[4] = {
			0xFFFFFFFF,
			0xACACACFF,
			0x555555FF,
			0x000000FF
		};

		/// <summary>
		/// Internal input view values
		/// </summary>
		InputStates* viewInput;

#pragma endregion

		/**
		 * @brief Loads all the needed objects to memory
		 */
		void mainLoad();


		/// <summary>
		/// Set the Registors Vals on screen
		/// </summary>
		void setRegistorsVals();


		/// <summary>
		/// Updates the text elements positions
		/// </summary>
		void positionTextElms();


		/**
		 * @brief Updates the debug view texture
		 */
		void buildDebugView();


		/**
		 * @brief Adds the target mem tile to the debug pixel array
		 * @param addr Target reading address
		 * @param tileId Current tile iD
		 */
		void addTileToDebug(bit16 addr, int tileId);


		/// <summary>
		/// Creates the output view
		/// </summary>
		void buildOutView();
	};
} // namespace TheBoy
#endif