#ifndef EMULVIEW_H
#define EMULVIEW_H

#include "emulatorController.h"


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
		 * @brief Pointer to the inUse window
		 */
		std::shared_ptr<sf::RenderWindow> window;


};	
} // namespace TheBoy


#endif