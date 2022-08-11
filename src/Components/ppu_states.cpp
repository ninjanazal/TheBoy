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

				ctrl->getPpu()->getFifo()->currState = FIFOSTATE::FF_TILE;
				ctrl->getPpu()->getFifo()->lineX = 0;
				ctrl->getPpu()->getFifo()->fetchedX = 0;
				ctrl->getPpu()->getFifo()->pushedX = 0;
				ctrl->getPpu()->getFifo()->fifoX = 0;
			}

			if (ctrl->getPpu()->getCurrentLineTicks() == 1) {
				ctrl->getPpu()->clearLineSpritePointer();
				ctrl->getPpu()->setLineSpriteCounter(0);

				loadLineSpt(ctrl);
			}
		}

		/// <summary>
		/// Drawing Pixels (Mode 3), this will iterate for at least 172 after the Mode 2
		/// </summary>
		/// <param name="ctrl">Reference to the Target emulatorController</param>
		void mode_XFER(EmulatorController* ctrl) {
			PixelPipe::PipelineStep(ctrl);
			if (ctrl->getPpu()->getFifo()->pushedX >= Ppu::xRes) {
				PixelPipe::PipelineFiFoReset(ctrl);
				ctrl->getLcd()->setLCDSMode(Lcd::LCDMODE::HBLANK);

				if (ctrl->getLcd()->getLCDSStat(Lcd::LCDSSTATS::HBLANK_STAT)) {
					ctrl->getCpu()->requestInterrupt(InterruptFuncs::InterruptType::INTR_STAT);
				}
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

					// Fps Counting
					bit32 currTick = ctrl->getCpu()->getTicks();
					bit32 frameTime = currTick - ctrl->getPpu()->getPreviousFrameTime();

					if (frameTime < ctrl->getPpu()->getTargetFrameTime()) {
						ctrl->getCpu()->sleepCpu(ctrl->getPpu()->getTargetFrameTime() - frameTime);
					}

					if (currTick - ctrl->getPpu()->getInitialTimer() >= 1000) {
						char* msgBuffer(new char[64]{});
						sprintf_s(msgBuffer, 64, 
							"-> Ppu Frames: %d", ctrl->getPpu()->getFrameCount());

						ctrl->getView()->setPpuFrameCount(msgBuffer);
						delete[] msgBuffer;

						ctrl->getPpu()->setInitialTimer(currTick);
						ctrl->getPpu()->resetFrameCount();
					}

					ctrl->getPpu()->addFrameCount(1);
					ctrl->getPpu()->updateFrameTime();
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

		/// <summary>
		/// Loads the Sprites for the target line
		/// </summary>
		/// <param name="ctrl">Reference to the current emulator controller</param>
		void loadLineSpt(EmulatorController* ctrl) {
			int cY = ctrl->getLcd()->getLyValue();
			bit8 sptHeight = ctrl->getLcd()->getLCDCObjHeight();

			// reset array
			for (int i = 0; i < 40; i++)
			{
				OamElement* entry = ctrl->getPpu()->getOamRamElementId(i);
				// If x == 0, this is not visible
				if (!entry->x) {
					continue;
				}

				// Maximum sprites per line reached
				if (ctrl->getPpu()->getLineSpriteCounter() >= 10) {
					break;
				}

				// On the current line sprite
				if ((entry->y <= cY + 16) && (entry->y + sptHeight > (cY + 16))) {
					OamLineElement* e = ctrl->getPpu()->getLineSpriteById(
						ctrl->getPpu()->incrementAndGetLineCounter());

					if (entry != NULL) {
						e->elm = *entry;
						e->next = nullptr;
					}

					if (ctrl->getPpu()->getLineSpritePointer() == nullptr ||
						ctrl->getPpu()->getLineSpritePointer()->elm.x > entry->x) {
						e->next = ctrl->getPpu()->getLineSpritePointer();
						ctrl->getPpu()->setLineSpritePointer(e);
					}

					// Sort the sprites
					OamLineElement* left = ctrl->getPpu()->getLineSpritePointer();
					OamLineElement* prev = left;

					while (left)
					{
						if (left->elm.x > entry->x) {
							//Should for fist 
							prev->next = e;
							if (e) {
								e->next = left;
							}
							break;
						}

						if (!left->next) {
							left->next = e;
						}

						prev = left;
						left = left->next;
					}
				}
			}

		}
	}
}