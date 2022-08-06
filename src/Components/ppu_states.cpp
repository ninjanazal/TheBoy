#include "ppu_states.h"
#include "emulatorController.h"

namespace TheBoy {
	namespace PpuStates {
		/// <summary>
		/// OAM Scan (Mode 2), this will scan for 80 pixels
		/// </summary>
		/// <param name="ctrl">Reference to the Target emulatorController</param>
		void mode_OAM(EmulatorController* ctrl) {
			if (ctrl->getPpu()->getCurrentLineTicks() >= 80) {
				ctrl->getLcd()->setLCDSMode(Lcd::LCDMODE::XFER);
			}
		}

		/// <summary>
		/// Drawing Pixels (Mode 3), this will iterate for at least 172 after the Mode 2
		/// </summary>
		/// <param name="ctrl">Reference to the Target emulatorController</param>
		void mode_XFER(EmulatorController* ctrl) {
			if (ctrl->getPpu()->getCurrentLineTicks() >= (80 + 172)) {
				ctrl->getLcd()->setLCDSMode(Lcd::LCDMODE::HBLANK);
			}
		}

		/// <summary>
		/// Vertical Blank (Mode 1), this will iterate for all the 154 display lines
		/// </summary>
		/// <param name="ctrl">Reference to the Target emulatorController</param>
		void mode_VBLANK(EmulatorController* ctrl) {
			if (ctrl->getPpu()->getCurrentLineTicks() >= Ppu::TicksPerLine) {
				 vertLineIncrement(ctrl);

				if (ctrl->getLcd()->getLyValue() >= Ppu::LinePerFrame) {
					ctrl->getLcd()->setLCDSMode(Lcd::LCDMODE::OAM);
					ctrl->getLcd()->resetLyValue();
				}

				ctrl->getPpu()->resetLineTicks();
			}
		}

		/// <summary>
		/// Horizontal Blank (Mode 0), this will iterate for all the horizontal lines
		/// </summary>
		/// <param name="ctrl"Reference to the Target emulatorController></param>
		void mode_HBLANK(EmulatorController* ctrl) {
			if (ctrl->getPpu()->getCurrentLineTicks() >= Ppu::TicksPerLine) {
				vertLineIncrement(ctrl);

				if (ctrl->getLcd()->getLyValue() == Ppu::yRes) {
					ctrl->getLcd()->setLCDSMode(Lcd::LCDMODE::VBLANK);

					ctrl->getCpu()->requestInterrupt(InterruptFuncs::InterruptType::INTR_VBLANK);

					if (ctrl->getLcd()->getLCDSStat(Lcd::LCDSSTATS::VBLANK_STAT)) {
						ctrl->getCpu()->requestInterrupt(InterruptFuncs::InterruptType::INTR_STAT);
					}
					ctrl->getPpu()->incrementCurrentFrame();
				} 
				else {
					ctrl->getLcd()->setLCDSMode(Lcd::LCDMODE::OAM);
				}

				ctrl->getPpu()->resetLineTicks();
			}
		}

		/// <summary>
		/// Holds the vertical line increment action
		/// </summary>
		/// <param name="ctrl">Reference to the current emulator controller</param>
		void vertLineIncrement(EmulatorController* ctrl) {
			ctrl->getLcd()->incrementLy();

			if (ctrl->getLcd()->getLyValue() == ctrl->getLcd()->getLyCompValue()) {
				ctrl->getLcd()->setLCDSLycFlag(1);

				if (ctrl->getLcd()->getLCDSStat(Lcd::LCDSSTATS::LY_STAT)) {
					ctrl->getCpu()->requestInterrupt(InterruptFuncs::InterruptType::INTR_STAT);
				}
			}
			else {
				ctrl->getLcd()->setLCDSLycFlag(0);
			}
		}
	}
}