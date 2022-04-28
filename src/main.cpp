#include <Controllers/emulatorController.h>


/**
 * @brief Main function for the TheBoy project
 * @return int
 */
int main(void) {
	using namespace TheBoy;
	std::unique_ptr<EmulatorController> emulator(new EmulatorController(EmuType::GBColor));
	emulator->Start();

	return 0;
}