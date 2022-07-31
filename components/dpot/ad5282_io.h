/*
 * AD5282_io.h
 *
 * @Created: 5/9/2022 7:08:28
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

//////////////////////////////////////////////////////////////////////////
//                               TWI-D-POT                              //
//        Single(AD5280)/Dual(AD5282), +15 V/±5 V, 256-Position,        //
//                  I2C-Compatible Digital Potentiometer                //
//////////////////////////////////////////////////////////////////////////

#ifndef AD5282_IO_H_
#define AD5282_IO_H_

#define AD5282_Channels				1	/* AD5282: 2Channel (0,1), AD5280: 1Channel (0) */

/* AD5282 - Address Byte (I2C frame 1) */
#define AD5282_BaseAddress			0b0101100
#define AD5282_PrgAddress_gm		0b0000011	/* Programmable Address group mask */

/* AD5282 - Instruction Byte (I2C frame 2) */
/* RDAC subaddress select; 0 = RDAC1 and 1 = RDAC2 */
#define AD5282_AB_bp	7
#define AD5282_AB_bm	(1 << 7)
/* Midscale reset, active high (only affects selected channel) */
#define AD5282_RS_bp	6
#define AD5282_RS_bm	(1 << 6)
/* Shutdown; same as SHDN pin operation except inverse logic (only affects selected channel) */
#define AD5282_SD_bp	5
#define AD5282_SD_bm	(1 << 5)
/* AD5280 - Output logic pin latched values; default Logic 0 */
#define AD5282_O2_bp	4
#define AD5282_O2_bm	(1 << 4)
/* AD5280|AD5282 - Output logic pin latched values; default Logic 0 */
#define AD5282_O1_bp	3
#define AD5282_O1_bm	(1 << 3)

enum AD5282_RDAC_enum
{
	DPOT_RDAC1			= (0 << AD5282_AB_bp),
	DPOT_RDAC2			= (1 << AD5282_AB_bp),
};


#endif // AD5282_IO_H_