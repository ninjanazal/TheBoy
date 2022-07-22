#include "lcd.h"

namespace TheBoy {

	/// <summary>
	/// 
	/// </summary>
	Lcd::Lcd() {

	}

	/// <summary>
	/// Lcd Clas Destructor
	/// </summary>
	Lcd::~Lcd() {
		delete regs;
		delete[] bgColorPallets;
		delete[] spriteColors1;
		delete[] spriteColors2;
	}

	/// <summary>
	/// Gets the bit value for the BG and Window enable/priority
	/// </summary>
	/// <returns>BG and Window enable/priority value</returns>
	bit8 Lcd::getLCDC_BGW_ENABLE() {
		return static_cast<bit8>(GETBIT(regs->lcdc, 0));
	}

	/// <summary>
	/// Gets the bit value for the OBJ enable
	/// </summary>
	/// <returns>OBJ enable value</returns>
	bit8 Lcd::getLCDC_OBJ_ENABLE() {
		return static_cast<bit8>(GETBIT(regs->lcdc, 1));
	}

	/// <summary>
	/// Gets the obj size value 
	/// </summary>
	/// <returns>OBJ size value</returns>
	bit8 Lcd::getLCDC_OBJ_HEIGHT() {
		return GETBIT(regs->lcdc, 2) ? 0x10 : 0x8;
	}

	/// <summary>
	/// Gets the Background map area
	/// </summary>
	/// <returns>BG tile map area value</returns>
	bit16 Lcd::getLCDC_BG_MAP_AREA() {
		return GETBIT(regs->lcdc, 3) ? 0x9C00 : 0x9800;
	}

	/// <summary>
	/// Gets the background and window tile data area
	/// </summary>
	/// <returns>BG and Window tile data area value</returns>
	bit16 Lcd::getLCDC_BDW_DATA_AREA() {
		return GETBIT(regs->lcdc, 4) ? 0x8000 : 0x8800;
	}

	/// <summary>
/// Gets the window enable bit value
/// </summary>
/// <returns>Window enable</returns>
	bit8 Lcd::getLCDC_WIN_ENABLE() {
		return static_cast<bit8>(GETBIT(regs->lcdc, 5));
	}
}