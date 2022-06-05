#ifndef PPU_H
#define PPU_H

#include "emulatorController.h"

namespace TheBoy {

	/**
	 * @brief Defined struct using the 
	 */
	typedef struct {
		bit8 y;
		bit8 x;
		bit8 tIndex;
		bit8 flags;
	} OamElement;

	class Ppu {	
		/*
		VRAM Sprite Attribute Table (OAM)
			Sprite attributes reside in the Sprite Attribute Table (OAM - Object Attribute Memory) at $FE00-FE9F.
			Each of the 40 entries consists of four bytes with the following meanings:
			
			Byte 0 - Y Position
				Y = Sprite’s vertical position on the screen + 16
				So for example, Y=0 hides a sprite, Y=2 hides an 8×8 sprite but displays the last two rows of an 8×16 sprite,
				Y=16 displays a sprite at the top of the screen,
				Y=144 displays an 8×16 sprite aligned with the bottom of the screen,
				Y=152 displays an 8×8 sprite aligned with the bottom of the screen,
				Y=154 displays the first six rows of a sprite at the bottom of the screen, Y=160 hides a sprite.

			Byte 1 - X Position
				X = Sprite’s horizontal position on the screen + 8.
				This works similarly to the examples above, except that the width of a sprite is always 8.
				An off-screen value (X=0 or X>=168) hides the sprite, but the sprite still affects the priority ordering,
				thus other sprites with lower priority may be left out due to the ten sprites limit per scan-line.
				A better way to hide a sprite is to set its Y-coordinate off-screen.

			Byte 2 - Tile Index
				In 8x8 mode (LCDC bit 2 = 0), this byte specifies the sprite’s only tile index ($00-$FF).
				This unsigned value selects a tile from the memory area at $8000-$8FFF.
			Byte 3 - Attributes/Flags
				---------------------------------------------------------------------------------
				Bit7   BG and Window over OBJ (0=No, 1=BG and Window colors 1-3 over the OBJ)
				Bit6   Y flip          (0=Normal, 1=Vertically mirrored)
				Bit5   X flip          (0=Normal, 1=Horizontally mirrored)
				Bit4   Palette number  **Non CGB Mode Only** (0=OBP0, 1=OBP1)
				Bit3   Tile VRAM-Bank  **CGB Mode Only**     (0=Bank 0, 1=Bank 1)
				Bit2-0 Palette number  **CGB Mode Only**     (OBP0-7)
		*/

	public:
		/**
		 * @brief Construct a new Ppu object
		 * @param ctrl Current emulator controller reference
		 */
		Ppu(EmulatorController* ctrl);

		/**
		 * @brief Destroy the Ppu object
		 */
		~Ppu() = default;

	private:
		/**
		 * @brief Pointer to the target emulator controller
		 */
		EmulatorController* emulCtrl;


		std::shared_ptr<OamElement> oam_ram;
	};
} // namespace TheBoy
#endif