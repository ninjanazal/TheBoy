#include "ppu.h"
#include <cstring>


namespace TheBoy {
	/**
	 * @brief Construct a new Ppu object
	 * @param ctrl Current emulator controller reference
	 */
	Ppu::Ppu(EmulatorController* ctrl) : emulCtrl(ctrl) {
		std::cout << "[PPU] ::: PPU has been created" << std::endl;

		oam_ram = new OamElement[40] { 0 };
		vRam = new bit8[0x2000]{ };

		buffer = new bit32[yRes * xRes * sizeof(32)]{ 0 };

		cFrame = 0;
		cLineTicks = 0;
		
		//memset(oam_ram, 0, sizeof(oam_ram));

		fifo = new FIFO_DATA();
		fifo->init();
	}


	/**
	 * @brief Destroy the Ppu object
	 */
	Ppu::~Ppu() {
		delete[] vRam;
		delete[] oam_ram;
		delete[] buffer;
		delete fifo;
	}

	/// <summary>
	/// Gets a pointer to the PPU FIFO data
	/// </summary>
	/// <returns>FIFO pointer</returns>
	FIFO_DATA* Ppu::getFifo() {
		return fifo;
	}

	/**
	 * @brief Ppu interation
	 */
	void Ppu::step() {
		cLineTicks++;
		Lcd::LCDMODE curr = emulCtrl->getLcd()->getLCDSMode();

		switch (curr) {
		case Lcd::LCDMODE::OAM:
			PpuStates::mode_OAM(emulCtrl);
			break;

		case Lcd::LCDMODE::XFER:
			PpuStates::mode_XFER(emulCtrl);
			break;

		case Lcd::LCDMODE::VBLANK:
			PpuStates::mode_VBLANK(emulCtrl);
			break;

		case Lcd::LCDMODE::HBLANK:
			PpuStates::mode_HBLANK(emulCtrl);
			break;

		default:
			std::cout << "[PPU] ::: Failed step call, unknown state" << std::endl;
			break;
		}
	}


	/**
	 * @brief Writes a value to the OAM ram position
	 * @param addr Target address value
	 * @param val Defined value
	 */
	void Ppu::oamWrite(bit16 addr, bit8 val) {
		// Manage offset buffer position
		if (addr >= 0xFE00) {
			addr -= 0xFE00;
		}
		// For memory location address, castring the arry to a bit8 pointer
		// This will produce a access to the target Oam entry value
		((bit8*)oam_ram)[addr] = val;
	}


	/**
	 * @brief Reads from the OAM Ram on the defined addres
	 * @param addr Target addres
	 * @return bit8 Value on the addres
	 */
	bit8 Ppu::oamRead(bit16 addr) {
		// Manage offset buffer position
		if (addr >= 0xFE00) {
			addr -= 0xFE00;
		}
		// For memory location address, castring the arry to a bit8 pointer
		// This will produce a access to the target Oam entry value
		return ((bit8*)oam_ram)[addr];
	}


	/**
	 * @brief Writes to the vRam on the defined address
	 * @param addr Target address
	 * @param val Defined value
	 */
	void Ppu::write(bit16 addr, bit8 val) {
		// Tile data is stored in VRAM in the memory area at $8000-$97FF;
		vRam[addr - 0x8000] = val;

	}


	/**
	 * @brief Reads from the vRam on the defined address
	 * @param addr Target addres
	 * @return bit8 Value on the address
	 */
	bit8 Ppu::read(bit16 addr) {
		// Tile data is stored in VRAM in the memory area at $8000-$97FF;
		return vRam[addr - 0x8000];
	}

	/// <summary>
	/// Get the current PPu Line tick count
	/// </summary>
	/// <returns>Line Tick count</returns>
	bit32 Ppu::getCurrentLineTicks() {
		return cLineTicks;
	}

	/// <summary>
	/// Resets the current line tick value
	/// </summary>
	void Ppu::resetLineTicks() {
		cLineTicks = 0;
	}

	/// <summary>
	/// Gets the current Ppu Frame coutne
	/// </summary>
	/// <returns>Current Frame count</returns>
	bit32 Ppu::getCurrentFrame() {
		return cFrame;
	}

	/// <summary>
	/// Iterates the current frame value
	/// </summary>
	void Ppu::incrementCurrentFrame() {
		cFrame++;
	}


	/// <summary>
	/// Gets the defined target frame Time
	/// </summary>
	/// <returns>Defined Frame time value</returns>
	bit32 Ppu::getTargetFrameTime() {
		return targetFrameTime;
	}


	/// <summary>
	/// Gets the last frame time value
	/// </summary>
	/// <returns>Last frame time value</returns>
	long Ppu::getPreviousFrameTime() {
		return prevFrameTime;
	}

	/// <summary>
	/// Initial frame group time stamp
	/// </summary>
	/// <returns>Initial frame group time</returns>
	long Ppu::getInitialTimer() {
		return  initTimer;
	}

	/// <summary>
	/// Defines the initial frame group time stamp
	/// </summary>
	/// <param name="val">Target frame group stamp</param>
	void Ppu::setInitialTimer(bit32 val) {
		initTimer = val;
	}

	/// <summary>
	/// Updates the Frame time to the current tick value
	/// </summary>
	void Ppu::updateFrameTime() {
		prevFrameTime = static_cast<long>(emulCtrl->getCpu()->getTicks());
	}

	/// <summary>
	/// Gets the current frameCount value
	/// </summary>
	/// <returns>Frame count value</returns>
	long Ppu::getFrameCount() {
		return frameCounter;
	}

	/// <summary>
	/// Resets the internal frame counter
	/// </summary>
	void Ppu::resetFrameCount() {
		frameCounter = 0;
	}

	/// <summary>
	/// Add a defined amount of frames to the counter
	/// </summary>
	/// <param name="val">Increment amount</param>
	void Ppu::addFrameCount(bit8 val) {
		frameCounter += val;
	}

	/// <summary>
	/// Defines the video buffer value on a defined position
	/// </summary>
	/// <param name="position">Target position</param>
	/// <param name="val">Defined value</param>
	void Ppu::setBufferValue(bit32 position, bit32 val) {
		buffer[position] = val;
	}

} // namespace TheBoy
