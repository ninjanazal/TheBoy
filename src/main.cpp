#include "emulatorController.h"


/**
 * @brief Main function for the TheBoy project
 * @return int
 */
using namespace TheBoy;

int main(int argc, char *argv[]) {
	std::unique_ptr<EmulatorController> emulator(new EmulatorController(EmuType::GBColor));
	//! Remove this hammered path
	emulator->Start("D:\\Projects\\TheBoy\\ROMS\\Tetris.gb");

	return 0;
}