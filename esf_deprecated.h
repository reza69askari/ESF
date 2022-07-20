/*
 * esf_deprecated.h
 *
 * @Created: 7/20/2022 20:13:43
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 */

#ifndef ESF_DEPRECATED_H_
#define ESF_DEPRECATED_H_

#include <esf_compiler.h>

//#pragma GCC diagnostic error "-Wdeprecated-declarations"
/* deprecate function with an error message.
 * HTU: DEPRECATED(uint8_t FlagCount_u16(const uint16_t), "use 'FlagCount' instead.");
 * */
#define DEPRECATED(func, desc) __attribute__ ((deprecated(desc))) func
/* 
 * HTU: DEPRECATED_TYPE(handler_t  , "use 'event_t' instead.");
 * */
#define DEPRECATED_TYPE(type, desc)	typedef int type __attribute__ ((deprecated(desc)));

#define DEPRECATE(name) static inline void __attribute__((deprecated)) deprecate_ ## name (void) { ; }

/* Following lines are deprecated and Not Recommended for New Design (NRND) */

//! >>>>>>>>>>>>>>>> NRND >>>>>>>>>>>>>>>>
#pragma region NRND
#define bit0_bm			0x01	/* bit 0 mask */
#define bit1_bm			0x02	/* bit 1 mask */
#define bit2_bm			0x04	/* bit 2 mask */
#define bit3_bm			0x08	/* bit 3 mask */
#define bit4_bm			0x10	/* bit 4 mask */
#define bit5_bm			0x20	/* bit 5 mask */
#define bit6_bm			0x40	/* bit 6 mask */
#define bit7_bm			0x80	/* bit 7 mask */

#define bit0_bp			0	/* bit 0 position */
#define bit1_bp			1	/* bit 1 position */
#define bit2_bp			2	/* bit 2 position */
#define bit3_bp			3	/* bit 3 position */
#define bit4_bp			4	/* bit 4 position */
#define bit5_bp			5	/* bit 5 position */
#define bit6_bp			6	/* bit 6 position */
#define bit7_bp			7	/* bit 7 position */

#define Group0_gm		0x01	//((uint8_t)0b00000001)
#define Group1_gm		0x03	//((uint8_t)0b00000011)
#define Group2_gm		0x07	//((uint8_t)0b00000111)
#define Group3_gm		0x0F	//((uint8_t)0b00001111)
#define Group4_gm		0x1F	//((uint8_t)0b00011111)
#define Group5_gm		0x3F	//((uint8_t)0b00111111)
#define Group6_gm		0x7F	//((uint8_t)0b01111111)
#define Group7_gm		0xFF	//((uint8_t)0b11111111)
#pragma endregion NRND
//! <<<<<<<<<<<<<<<< .NRND <<<<<<<<<<<<<<<<


#endif // ESF_DEPRECATED_H_