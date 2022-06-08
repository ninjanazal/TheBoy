#include "emulatorController.h"
#include <SFML/Window.hpp>

namespace TheBoy {
	/**
	 * @brief Construct a new Emulator Controller:: Emulator Controller object
	 * @param type Target emulation type
	 */
	EmulatorController::EmulatorController() { 
		std::cout << "[Emulator] ::: controller was created" << std::endl;

		debugBuffer = new char[1024] {};
		debugMsgPointer = 0;
	}


	/**
	 * @brief Destroy the Emulator Controller object
	 */
	EmulatorController::~EmulatorController() {
		delete[] debugBuffer;
	}


	/**
	 * @brief Emulator internal loop
	 */
	void EmulatorController::_run() {
		std::cout << "[Emulator] ::: Starting the emulator update loop" << std::endl;

		while (emu_state.running) {
			comps.view->ManageEvents();

			comps.cpu->step();
			debugUpdate();

			comps.view->Draw();
		}
	}


	/**
	 * @brief Initialize the Emulator with a defined size
	 * @param size Window size
	 */
	void EmulatorController::Start(const char* rom_path) {
		comps.view = std::make_shared<EmulView>(this);

		comps.bus = std::make_shared<AddressBus>(this);
		comps.dma = std::make_shared<Dma>(this);
		comps.ram = std::make_shared<Ram>(this);
		comps.ppu = std::make_shared<Ppu>(this);

		comps.io = std::make_shared<IO>(this);
		comps.timer = std::make_shared<Timer>(this);

		emu_state.reset();
		comps.cpu = std::make_shared<Cpu>(this);

		comps.cart = std::make_shared<Cartridge>(this, rom_path);
		if(!comps.cart->loadCartridgeFromFile()){
			std::cout << "[Emulator] ::: Fail to load cartridge!" << std::endl;
			return;
		}

		std::cout << "[Emulator] ::: Cartridge was loaded!" << std::endl;

		this->_run();
	}


	/**
	 * @brief Stops the emulation execution with a defined message
	 * @param msg Stop message
	 */
	void EmulatorController::forceEmuStop(const char* msg){
		emu_state.running = false;
		printf(msg);
		fflush(stdout);
	}


	/**
	 * @brief Emulates a defined number of cpu cycles
	 * @param cycles 
	 */
	void EmulatorController::emulCycles(const int& cycles) {
		for (int i = 0; i<cycles; i++ ) {
			for(int n = 0; n < 4; n++) {
				emu_state.ticks++;
				comps.timer->tick();
			}
		}
		comps.dma->step();
	}


	/**
	 * @brief Updates the debug, information if available
	 */
	void EmulatorController::debugUpdate() {
		// Some tests marks this address for pending information
		if(getBus()->abRead(0xFF02) == 0x81){
			debugBuffer[debugMsgPointer++] = getBus()->abRead(0xFF01);
			getBus()->abWrite(0xFF02, 0);
		}

		if(debugBuffer[0]){
			printf("[DEBUGMSG] ::: %s\n", debugBuffer);
		}
	}



	/**
	 * @brief Get the Cartridge object
	 * @return std::shared_ptr<Cartridge> Shared pointer to the inUse cartridge
	 */
	std::shared_ptr<Cartridge> EmulatorController::getCartridge() {
		if(!comps.cart){
			std::cout << "[Emulator] ::: Get Cartridge on a null shared!" << std::endl;
		}
		return comps.cart;
	}

	/**
	 * @brief Get the Bus object
	 * @return std::shared_ptr<AddressBus> Shared pointer to the inUse AddressBus
	 */
	std::shared_ptr<AddressBus> EmulatorController::getBus() {
		if(!comps.bus) {
			std::cout << "[Emulator] ::: Get Bus on a null shared!" << std::endl;
		}
		return comps.bus;
	}

	/**
	 * @brief Get the Dma object
	 * @return std::shared_ptr<Dma> Shared pointer to the inUse Dma
	 */
	std::shared_ptr<Dma> EmulatorController::getDma() {
		if(!comps.dma){
			std::cout << "[Emulator] ::: Get Dma on a null shared!" << std::endl;
		}
		return comps.dma;
	}


	/**
	 * @brief Get the Ram object
	 * @return std::shared_ptr<Ram> Shared pointer to the inUse Ram
	 */
	std::shared_ptr<Ram> EmulatorController::getRam() {
		if(!comps.ram) {
			std::cout << "[Emulator] ::: Get Ram on a null shared!" << std::endl;
		}
		return comps.ram;
	}


	/**
	 * @brief Get the Ppu object
	 * @return std::shared_ptr<Ppu> Shared pointer to the inUse Ppu
	 */
	std::shared_ptr<Ppu> EmulatorController::getPpu() {
		if(!comps.ppu) {
			std::cout << "[Emulator] ::: Get Ppu on a null shared!" << std::endl;
		}
		return comps.ppu;
	}


	/**
	 * @brief Get the Cpu object
	 * @return std::shared_ptr<Ram> Shared pointer to the inUse Cpu
	 */
	std::shared_ptr<Cpu> EmulatorController::getCpu() {
		if(!comps.cpu) {
			std::cout << "[Emulator] ::: Get Cpu on a null shared!" << std::endl;
		}
		return comps.cpu;
	}

	
	/**
	 * @brief Gets the IO object
	 * @return std::shared_ptr<IO> Shared pointer to the inUse IO
	 */
	std::shared_ptr<IO> EmulatorController::getIO() {
		if(!comps.io) {
			std::cout << "[Emulator] ::: Get IO on a null shared!" << std::endl;
		}
		return comps.io;
	}

	/**
 	* @brief Get the Timer object
	* @return std::shared_ptr<Timer> Shared pointer to the inUse Timer
	*/
	std::shared_ptr<Timer> EmulatorController::getTimer() {
		if(!comps.timer) {
			std::cout << "[Emulator] ::: Get Timer on a null shared!" << std::endl;
		}
		return comps.timer;
	}


	/**
 	* @brief Get the Timer object
	* @return std::shared_ptr<Timer> Shared pointer to the inUse Timer
	*/
	std::shared_ptr<EmulView> EmulatorController::getView() {
		if(!comps.timer) {
			std::cout << "[Emulator] ::: Get View on a null shared!" << std::endl;
		}
		return comps.view;
	}
}