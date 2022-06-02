#ifndef EMULVIEW_H
#define EMULVIEW_H

#include "emulatorController.h"
#include <SFML/System.hpp>

namespace TheBoy {
	class EmulView {
	
	public:
		/**
		 * @brief 
		 */
		EmulView(EmulatorController* ctrl);

		/**
		 * @brief Destroy the Emul View object
		 */
		~EmulView() = default;

		/**
		 * @brief Manages the window events
		 */
		void ManageEvents();
	

		/**
		 * @brief Updates the current window
		 */
		void Draw();

	private:
		/**
		 * @brief Pointer to the target emulator controller
		 */
		EmulatorController* emulCtrl;


		/**
		 * @brief Holds the current window size
		 */
		sf::Vector2u winSize;


		/**
		 * @brief Pointer to the inUse window
		 */
		std::shared_ptr<sf::RenderWindow> window;


		/**
		 * @brief Window icon texture
		 */
		std::shared_ptr<sf::Image> wIcon;


		/**
		 * @brief Debug font used
		 */
		std::shared_ptr<sf::Font> wFont;


		/**
		 * @brief Array with the debug text elements
		 */
		std::shared_ptr<sf::Text> wText[8];

		/**
		 * @brief Loads all the needed objects to memory
		 */
		void mainLoad();

};	
} // namespace TheBoy


#endif