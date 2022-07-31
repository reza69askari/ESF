/*
 * AD5282.h
 *
 * @Created: 5/9/2022 7:09:29
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

//////////////////////////////////////////////////////////////////////////
//! #Requires:
// #define AD5282_TWI				&TWIE
//! #Options:
// #define Config_Replace_AD5280	/* Use AD5280 instead of AD5282 */
// #define Config_AD5282_PGA		0b10	/* AD5282 - Programmable Address (2-bits) */
// #define Config_AD5282_TwiSpeed	100000	/* AD5282 - TWI-Speed (MAX 400KHz) */
//////////////////////////////////////////////////////////////////////////

#ifndef AD5282_H_
#define AD5282_H_

#include <conf_board.h>
#include "AD5282_io.h"
#include <esf_compiler.h>

typedef enum AD5282_Channels_enum
{
	/* RDAC subaddress select; 0 = RDAC1 and 1 = RDAC2 */
	AD5282_Channel_A	= DPOT_RDAC1,
	#ifndef Config_Replace_AD5280
	AD5282_Channel_B	= DPOT_RDAC2,
	#endif
} AD5282_CH_t;

typedef enum AD5282_Options_enum
{
	/* Midscale reset, active high (only affects selected channel) */
	AD5282_SetMiddle_gc	= AD5282_RS_bm,
	/* Shutdown; same as SHDN pin operation except inverse logic (only affects selected channel) */
	AD5282_Shutdown_gc	= AD5282_SD_bm,
	#ifdef Config_Replace_AD5280
	/* AD5280 - Output logic pin latched values; default Logic 0 */
	AD5282_Output2_gc	= AD5282_O2_bm,
	#endif
	/* AD5280|AD5282 - Output logic pin latched values; default Logic 0 */
	AD5282_Output1_gc	= AD5282_O1_bm,
} AD5282_Opt_t;

#define AD5282_Options_gm			(AD5282_RS_bm|AD5282_SD_bm|AD5282_O2_bm|AD5282_O1_bm)

status_code_t AD5282_Init(void);

status_code_t AD5282_Write(AD5282_CH_t channel, AD5282_Opt_t options, U8 value);

static inline status_code_t AD5282_WriteChannel(AD5282_CH_t channel, U8 value)
{
	return AD5282_Write(channel, 0, value);
}

#endif // AD5282_H_