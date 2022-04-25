#include "Controllers/emulatorController.h"


/**
 * @brief Main function for the TheBoy project
 * @return int
 */
int main(void) {
	using namespace TheBoy;

	EmulatorController *emulator = new EmulatorController(EmuType::GBColor);
	emulator->Start(new sf::Vector2u(800, 800));

	return 0;
}