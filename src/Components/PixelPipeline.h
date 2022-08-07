#pragma once
#ifndef PIXELPIPELINE_H
#define PIXELPIPELINE_H
#include "common.h"

namespace TheBoy {
	class EmulatorController;

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
}

#endif // !PIXELPIPELINE_H
