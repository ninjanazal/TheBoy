#include "emulatorController.h"


/**
 * @brief Main function for the TheBoy project
 * @return int
 */
using namespace TheBoy;

int main(int argc, char *argv[]) {

	std::shared_ptr<EmulatorController> emulator;
	emulator = std::make_shared<EmulatorController>();

	//! Remove this hammered path
	emulator->Start("D:\\Projects\\TheBoy\\ROMS\\tests\\11-op a,(hl).gb");
	return 0;
}