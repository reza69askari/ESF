/*
 * esf_mask_position.h
 * > *_gc:	group configuration
 * > *_gm:	group mask
 * > *_gp:	group position
 * > *_bm:	bit mask
 * > *_bp:	bit position
 *
 * @Created: 7/20/2022 20:04:29
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_MASK_POSITION_H_
#define ESF_MASK_POSITION_H_

//! >>>>>>> exist files
#include <compiler.h>

//! >>>>>>> inline set and check

#define HasFlag(_var, flag_gc)		Tst_bits(_var, flag_gc)	/* check if value has any of specific flag(s). */
#define HasAny(_var)				((_var) != 0)			/* check if variable has any flag|value. */
#define SetFlag(_var, flag_gc)		Set_bits(_var, flag_gc)
#define ClearFlag(_var, flag_gc)	Clr_bits(_var, flag_gc)
#define WriteFlags(_var, mask, flag_gc)	Wr_bits(_var, mask, flag_gc)

#define RemoveFlag(_var, flag_gc)	ClearFlag(_var, flag_gc)
#define ResetFlag(_var, flag_gc)	ClearFlag(_var, flag_gc)

//! >>>>>>> group configurations

typedef enum Divition_enum
{
	DIV1_gc				= (1 << 0),
	DIV2_gc				= (1 << 1),
	DIV4_gc				= (1 << 2),
	DIV8_gc				= (1 << 3),
	DIV16_gc			= (1 << 4),
	DIV32_gc			= (1 << 5),
	DIV64_gc			= (1 << 6),
	DIV128_gc			= (1 << 7),
	DIV256_gc			= (1 << 8),
	DIV512_gc			= (1 << 9),
	DIV1024_gc			= (1 << 10),
	DIV2048_gc			= (1 << 11),
	DIV4096_gc			= (1 << 12),
	DIV8192_gc			= (1 << 13),
	DIV16384_gc			= (1 << 14),
	DIV32768_gc			= (1 << 15),
} Div_t;

//! >>>>>>> Masks and Positions

#ifndef PIN0_bm			// Generic Port Pins
#define PIN0_bp			0
#define PIN1_bp			1
#define PIN2_bp			2
#define PIN3_bp			3
#define PIN4_bp			4
#define PIN5_bp			5
#define PIN6_bp			6
#define PIN7_bp			7

#define PIN0_bm			0x01
#define PIN1_bm			0x02
#define PIN2_bm			0x04
#define PIN3_bm			0x08
#define PIN4_bm			0x10
#define PIN5_bm			0x20
#define PIN6_bm			0x40
#define PIN7_bm			0x80
#endif // Generic Port Pins

#define BIT0_bm			0x01
#define BIT1_bm			0x02
#define BIT2_bm			0x04
#define BIT3_bm			0x08
#define BIT4_bm			0x10
#define BIT5_bm			0x20
#define BIT6_bm			0x40
#define BIT7_bm			0x80

#define BIT0_bp			0
#define BIT1_bp			1
#define BIT2_bp			2
#define BIT3_bp			3
#define BIT4_bp			4
#define BIT5_bp			5
#define BIT6_bp			6
#define BIT7_bp			7

#define BITs10_gm		0x00000011
#define BITs20_gm		0x00000111
#define BITs30_gm		0x00001111
#define BITs40_gm		0x00011111
#define BITs50_gm		0x00111111
#define BITs60_gm		0x01111111
#define BITs70_gm		0x11111111
#define BITs21_gm		0x00000110
#define BITs31_gm		0x00001110
#define BITs41_gm		0x00011110
#define BITs51_gm		0x00111110
#define BITs61_gm		0x01111110
#define BITs71_gm		0x11111110
#define BITs32_gm		0x00001100
#define BITs42_gm		0x00011100
#define BITs52_gm		0x00111100
#define BITs62_gm		0x01111100
#define BITs72_gm		0x11111100
#define BITs43_gm		0x00011000
#define BITs53_gm		0x00111000
#define BITs63_gm		0x01111000
#define BITs73_gm		0x11111000
#define BITs54_gm		0x00110000
#define BITs64_gm		0x01110000
#define BITs74_gm		0x11110000
#define BITs65_gm		0x01100000
#define BITs75_gm		0x11100000
#define BITs76_gm		0x11000000

#define BITs10_gp		0
#define BITs20_gp		0
#define BITs30_gp		0
#define BITs40_gp		0
#define BITs50_gp		0
#define BITs60_gp		0
#define BITs70_gp		0
#define BITs21_gp		1
#define BITs31_gp		1
#define BITs41_gp		1
#define BITs51_gp		1
#define BITs61_gp		1
#define BITs71_gp		1
#define BITs32_gp		2
#define BITs42_gp		2
#define BITs52_gp		2
#define BITs62_gp		2
#define BITs72_gp		2
#define BITs43_gp		3
#define BITs53_gp		3
#define BITs63_gp		3
#define BITs73_gp		3
#define BITs54_gp		4
#define BITs64_gp		4
#define BITs74_gp		4
#define BITs65_gp		5
#define BITs75_gp		5
#define BITs76_gp		6


#endif // ESF_MASK_POSITION_H_