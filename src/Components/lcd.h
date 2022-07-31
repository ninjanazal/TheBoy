#pragma once
#ifndef LCD_H
#define LCD_H

#include "emulatorController.h"

namespace TheBoy {

	/*
	* FF40 - LCDC (LCD Control) (R/W)
		LCDC is the main LCD Control register. Its bits toggle what elements are displayed on the screen, and how.

		7	LCD and PPU enable	0=Off, 1=On
		6	Window tile map area	0=9800-9BFF, 1=9C00-9FFF
		5	Window enable	0=Off, 1=On
		4	BG and Window tile data area	0=8800-97FF, 1=8000-8FFF
		3	BG tile map area	0=9800-9BFF, 1=9C00-9FFF
		2	OBJ size	0=8x8, 1=8x16
		1	OBJ enable	0=Off, 1=On
		0	BG and Window enable/priority	0=Off, 1=On

	* FF41 - STAT (LCD Status) (R/W)

		Bit 6 - LYC=LY STAT Interrupt source         (1=Enable) (Read/Write)
		Bit 5 - Mode 2 OAM STAT Interrupt source     (1=Enable) (Read/Write)
		Bit 4 - Mode 1 VBlank STAT Interrupt source  (1=Enable) (Read/Write)
		Bit 3 - Mode 0 HBlank STAT Interrupt source  (1=Enable) (Read/Write)
		Bit 2 - LYC=LY Flag                          (0=Different, 1=Equal) (Read Only)
		Bit 1-0 - Mode Flag                          (Mode 0-3, see below) (Read Only)
				  0: HBlank
				  1: VBlank
				  2: Searching OAM
				  3: Transferring Data to LCD Controller

	FF42 - SCY (Scroll Y) (R/W), FF43 - SCX (Scroll X) (R/W)
		Those specify the top-left coordinates of the visible 160×144 pixel area within the 256×256 pixels BG map. Values in the range 0–255 may be used.



	*/

	typedef struct LcdRegs {
		/// <summary>
		/// LcdC lcd Register
		/// </summary>
		bit8 lcdc;


		/// <summary>
		/// Lcd status register
		/// </summary>
		bit8 lcds;

		/// <summary>
		// Scrolling registor value, vertical
		/// </summary>
		bit8 scrollY;


		/// <summary>
		// Scrolling registor value, horizontal
		/// </summary>
		bit8 scrollX;

		/// <summary>
		/// FF44 - LY (LCD Y Coordinate) (R)
		/// LY indicates the current horizontal line, which might be about to be drawn, 
		/// being drawn, or just been drawn.LY can
		/// hold any value from 0 to 153, with values from 144 to 153 indicating the VBlank period.
		/// </summary>
		bit8 ly;

		/// <summary>
		/// FF45 - LYC (LY Compare) (R/W)
		/// The Game Boy permanently compares the value of the LYCand LY registers.
		/// When both values are identical, the “LYC = LY” flag in the STAT register is set,
		/// and (if enabled) a STAT interrupt is requested.
		/// </summary>
		bit8 lyComp;

		/// <summary>
		/// As FF46 defined on the io switch
		/// </summary>
		bit8 dma;

		/// <summary>
		/// FF47 - BGP (BG Palette Data) (R/W) - Non CGB Mode Only
		/// This register assigns gray shades to the color indexes of the BGand Window tiles.
		/// </summary>
		bit8 backPallet;

		/// <summary>
		/// FF48 - OBP0 (OBJ Palette 0 Data) (R/W), FF49 - OBP1 (OBJ Palette 1 Data) (R/W) -
		///	Both Non CGB Mode Only
		/// These registers assigns gray shades to the color indexes of the OBJs that use the 
		/// corresponding palette.They work exactly like BGP, except that the lower two bits are
		///  ignored because color index 0 is transparent for OBJs.
		/// </summary>
		bit8* objPallets = new bit8[2];

		/// <summary>
		/// FF4A - WY (Window Y Position) (R/W), FF4B - WX (Window X Position + 7) (R/W)
		/// Specify the top - left coordinates of the Window.
		/// (The Window is an alternate background area which can be displayed above of
		/// the normal background.OBJs(sprites) may be still displayed above or behind the Window,
		/// just as for normal BG.)
		/// </summary>


		// Window Y coordinate
		bit8 WY;

		// Window X coordinate
		bit8 WX;
	} LcdRegs;


	class Lcd {
	public:
		/// <summary>
		/// Defines the multiple lcd states
		/// </summary>
		typedef enum LCDMODE {
			HBLANK,
			VBLANK,
			OAM,
			XFER
		} LCDMODE;


		/// <summary>
		/// Enumeration for access the 3 to 6 bit flag on the LCDS reg value
		/// </summary>
		typedef enum LCDSSTATS {
			HBLANK_STAT = (1 << 3),
			VBLANK_STAT = (1 << 4),
			OAM_STAT = (1 << 5),
			LY_STAT = (1 << 6)
		} LCDSSTATS;


		/// <summary>
		/// Lcd Class Constructor
		/// </summary>
		Lcd(EmulatorController* ctrl);
	

		/// <summary>
		/// Lcd Clas Destructor
		/// </summary>
		~Lcd();

		/// <summary>
		/// Gets the defined value for a addres on the Lcd
		/// </summary>
		/// <param name="addres">Target address</param>
		/// <returns>Value</returns>
		bit8 read(bit16 addres);


		/// <summary>
		/// Writes to the defined addres value
		/// </summary>
		/// <param name="addres">Target address value</param>
		/// <param name="value">Value</param>
		void write(bit16 addres, bit8 value);

		/// <summary>
		/// Gets the bit value for the BG and Window enable/priority
		/// </summary>
		/// <returns>BG and Window enable/priority value</returns>
		bit8 getLCDCBgwEnable();


		/// <summary>
		/// Gets the bit value for the OBJ enable
		/// </summary>
		/// <returns>OBJ enable value</returns>
		bit8 getLCDCObjEnable();


		/// <summary>
		/// Gets the obj size value 
		/// </summary>
		/// <returns>OBJ size value</returns>
		bit8 getLCDCObjHeight();


		/// <summary>
		/// Gets the Background map area
		/// </summary>
		/// <returns>BG tile map area value</returns>
		bit16 getLCDCBgMapArea();


		/// <summary>
		/// Gets the background and window tile data area
		/// </summary>
		/// <returns>BG and Window tile data area value</returns>
		bit16 getLCDCBdwDataArea();

		/// <summary>
		/// Gets the window enable bit value
		/// </summary>
		/// <returns>Window enable</returns>
		bit8 getLCDCWindEnable();

		/// <summary>
		/// Gets the windows tile map area value
		/// </summary>
		/// <returns>Window tile map area value</returns>
		bit16 getLCDCWindMapArea();

				/// <summary>
		/// Gets if the LCD and PPU are enable
		/// </summary>
		/// <returns>LCD and PPU enable value</returns>
		bit8 getLCDCLcdEnable();

		/// <summary>
		/// Gets the current Lcd Mode
		/// </summary>
		/// <returns>LCDMODE value</returns>
		LCDMODE getLCDSMode();

		/// <summary>
		/// Defines the current lcd mode
		/// </summary>
		/// <param name="mode">LCD mode value</param>
		void setLCDSMode(LCDMODE mode);

		/// <summary>
		/// Gets the LYC Flag value
		/// </summary>
		/// <returns>LYC Value</returns>
		bit8 getLCDSLycFlag();

		/// <summary>
		/// Defines the LYC flag value
		/// </summary>
		/// <param name="value">LYC define value</param>
		void setLCDSLycFlag(bit8 value);

		/// <summary>
		/// Get the target stat vlaue
		/// </summary>
		/// <param name="stat">Target Stat value</param>
		/// <returns>Current stat flag value</returns>
		bit8 getLCDSStat(LCDSSTATS stat);

	private:
		/// <summary>
		/// Pointer to the target emulator controller
		/// </summary>
		EmulatorController* emulCtrl;


		/// <summary>
		/// Lcd registors structure
		/// </summary>
		LcdRegs* regs;


		/// <summary>
		/// Holds the defined background colors
		/// </summary>
		sf::Color bgColorPallets[4];

		/// <summary>
		/// Defines the sprite1 colors
		/// </summary>
		sf::Color spriteColors1[4];

		/// <summary>
		/// Defines the sprite2 colors
		/// </summary>
		sf::Color spriteColors2[4];

		// GameBoy color pallet representation
		sf::Color defaultColors[4] = {
			sf::Color::White,
			sf::Color(210, 210, 210),
			sf::Color(45, 45, 45),
			sf::Color::Black
		};

		/// <summary>
		/// Updates the target pallet colors based on defined values
		/// </summary>
		/// <param name="palletAddr">Pallet addres element</param>
		/// <param name="val">Target pallet group</param>
		void updatePallet(bit8 palletAddr, bit8 val);
	};
}
#endif // !LCD_H
