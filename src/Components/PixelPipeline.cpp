#include "PixelPipeline.h"
#include "emulatorController.h"

namespace TheBoy {

	/// <summary>
	/// Fifo Push function declaration
	/// </summary>
	/// <param name="ctrl">Target Emulator controller</param>
	/// <param name="value">Push value</param>
	void FifoPush(EmulatorController* ctrl, bit32 value) {
		FIFO_Value* next = new FIFO_Value();
		next->next = nullptr;
		next->color = value;

		// Initial entry, set the head and tail to the created next
		if (!ctrl->getPpu()->getFifo()->pixelFifo.head) {
			ctrl->getPpu()->getFifo()->pixelFifo.head = ctrl->getPpu()->getFifo()->pixelFifo.tail = next;
		}
		else {
			// If not defined the current tail next and the created next and define it as the new tail
			ctrl->getPpu()->getFifo()->pixelFifo.tail->next = next;
			ctrl->getPpu()->getFifo()->pixelFifo.tail = next;
		}

		ctrl->getPpu()->getFifo()->pixelFifo.size++;
	}

	/// <summary>
	/// FIFO pop instruction
	/// </summary>
	/// <param name="ctrl">Target Emulator controller</param>
	/// <returns>Defined value</returns>
	bit32 FifoPop(EmulatorController* ctrl) {
		if (ctrl->getPpu()->getFifo()->pixelFifo.size <= 0) {
			printf("[PIXEL PIPE] ::: ->  ERROR POPING FIFO VALUE! Wrong size!\n");
			fflush(stdout);
		}

		FIFO_Value* popHead = ctrl->getPpu()->getFifo()->pixelFifo.head;
		ctrl->getPpu()->getFifo()->pixelFifo.head = popHead->next;
		ctrl->getPpu()->getFifo()->pixelFifo.size--;

		bit32 val = popHead->color;
		delete popHead;

		return val;
	}

	/// <summary>
	/// Pipeline internal switcher, fetch the pipeline state
	/// </summary>
	/// <param name="ctrl">Target Emulator controller</param>
	void PipelineFetch(EmulatorController* ctrl) {
		switch (ctrl->getPpu()->getFifo()->currState) {
		case FIFOSTATE::FF_TILE: {
			if (ctrl->getLcd()->getLCDCBgwEnable())
			{
				ctrl->getPpu()->getFifo()->bg_fetched[0] = ctrl->getCpu()->requestBusRead(
					ctrl->getLcd()->getLCDCBgMapArea() +
					(ctrl->getPpu()->getFifo()->mapX / 8) +
					((ctrl->getPpu()->getFifo()->mapY / 8) * 32)
				);
			}
			break;
		}
		case FIFOSTATE::FF_DATA_LOW:
			break;
		case FIFOSTATE::FF_DATA_HIGH:
			break;
		case FIFOSTATE::FF_SLEEP:
			break;
		case FIFOSTATE::FF_PUSH:
			break;
		default:
			break;
		}
	}

	/// <summary>
	/// Push the pixel value to the ppu buffer
	/// </summary>
	/// <param name="ctrl">Target Emulator controller</param>
	void PipelinePushPixel(EmulatorController* ctrl) {
		if (ctrl->getPpu()->getFifo()->pixelFifo.size > 8) {
			bit32 pData = FifoPop(ctrl);

			if (ctrl->getPpu()->getFifo()->lineX >= (ctrl->getLcd()->getLcdRegistors()->scrollX % 8)) {
				ctrl->getPpu()->setBufferValue(
					ctrl->getPpu()->getFifo()->pushedX + (ctrl->getLcd()->getLyValue() * Ppu::xRes),
					pData
				);

				ctrl->getPpu()->getFifo()->pushedX++;
			}
			ctrl->getPpu()->getFifo()->lineX++;
		}
	}

	/// <summary>
	/// Pipeline iteration function
	/// </summary>
	/// <param name="ctrl">Target Emulator controller</param>
	void PipelineStep(EmulatorController* ctrl) {
		ctrl->getPpu()->getFifo()->mapY =
			(ctrl->getLcd()->getLyValue() + ctrl->getLcd()->getLcdRegistors()->scrollY);
		ctrl->getPpu()->getFifo()->mapX =
			(ctrl->getPpu()->getFifo()->fetchedX + ctrl->getLcd()->getLcdRegistors()->scrollX);

		ctrl->getPpu()->getFifo()->tileY =
			((ctrl->getLcd()->getLyValue() + ctrl->getLcd()->getLcdRegistors()->scrollY) % 8) * 2;

		// During a even line step
		if (!(ctrl->getPpu()->getCurrentLineTicks() & 1)) {
			PipelineFetch(ctrl);
		}
		PipelinePushPixel(ctrl);
	}
}