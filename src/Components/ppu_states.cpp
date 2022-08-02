#include "ppu_states.h"
#include "emulatorController.h"

namespace TheBoy {
	namespace PpuStates {

		void mode_OAM(EmulatorController* ctrl) {
			if (ctrl->getPpu()->getCurrentLineTicks() >= 80) {
				ctrl->getLcd()->setLCDSMode(Lcd::LCDMODE::XFER);
			}
		}

		void mode_XFER(EmulatorController* ctrl) {
			if (ctrl->getPpu()->getCurrentLineTicks() >= (80 + 172)) {
				ctrl->getLcd()->setLCDSMode(Lcd::LCDMODE::HBLANK);
			}
		}

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

		void mode_HBLANK(EmulatorController* ctrl) {
		}



		/// <summary>
		/// Holds the vertical line increment action
		/// </summary>
		/// <param name="ctrl">Reference to the current emulator controller</param>
		void vertLineIncrement(EmulatorController* ctrl) {
			ctrl->getLcd()->incrementLy();
		}
	}
}