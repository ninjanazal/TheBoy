#pragma once
#ifndef PIXELPIPELINE_H
#define PIXELPIPELINE_H
#include "common.h"

namespace TheBoy {
	class EmulatorController;
	namespace PixelPipe {

		/// <summary>
		/// Fifo Push function
		/// </summary>
		/// <param name="ctrl">Target Emulator controller</param>
		/// <param name="value">Push value</param>
		void FifoPush(EmulatorController* ctrl, bit32 value);


		/// <summary>
		/// FIFO pop instruction
		/// </summary>
		/// <param name="ctrl">Target Emulator controller</param>
		/// <returns>Defined value</returns>
		bit32 FifoPop(EmulatorController* ctrl);


		/// <summary>
		/// Pipeline internal switcher, fetch the pipeline state
		/// </summary>
		/// <param name="ctrl">Target Emulator controller</param>
		void PipelineFetch(EmulatorController* ctrl);


		/// <summary>
		/// Pipeline iteration function
		/// </summary>
		/// <param name="ctrl">Target Emulator controller</param>
		void PipelineStep(EmulatorController* ctrl);


		/// <summary>
		/// Push the pixel value to the ppu buffer
		/// </summary>
		/// <param name="ctrl">Target Emulator controller</param>
		void PipelinePushPixel(EmulatorController* ctrl);


		/// <summary>
		/// Adds a color to the fifo
		/// </summary>
		/// <param name="ctrl">Target Emulator controller</param>
		/// <returns>Was able to push to fifo</returns>
		bool PipelineFiFoAdd(EmulatorController* ctrl);


		/// <summary>
		/// Resets the fifo state
		/// </summary>
		/// <param name="ctrl">Target Emulator controller</param>
		void PipelineFiFoReset(EmulatorController* ctrl);
	}

}

#endif // !PIXELPIPELINE_H
