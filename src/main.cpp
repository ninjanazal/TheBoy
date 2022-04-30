#include "emulatorController.h"


/**
 * @brief Main function for the TheBoy project
 * @return int
 */
using namespace TheBoy;

int main(void) {
	std::unique_ptr<EmulatorController> emulator(new EmulatorController(EmuType::GBColor));
	emulator->Start(std::string("D:\\Projects\\TheBoy\\ROMS\\Tetris.gb").c_str());

	return 0;
}