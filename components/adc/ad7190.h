/*
 * ad7190.h
 *
 * @Created: 4/13/2022 2:44:42
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

//////////////////////////////////////////////////////////////////////////
//! #Requires:
//#define AD7190_PORT_id			IOPORT_PORTC
//#define AD7190_Sync_pin			IOPORT_CREATE_PIN(PORTB, PIN3_bp)
//#define CONFIG_SPI_MASTER_DUMMY	0x00
//! #Options:
//#define AD7190_CS_pin				IOPORT_CREATE_PIN(PORTB, PIN1_bp)
//#define Config_AD7190_UseIRQ		/* read data in interrupt. */
//#define Config_AD7190_ClockHz		4920000UL
//#define Config_AD7190_ClockSource	AD7190_CLKSEL_ExtXTAL
//////////////////////////////////////////////////////////////////////////

#ifndef AD7190_H_
#define AD7190_H_

#include "ad7190_io.h"
#include <esf_compiler.h>

#define AD7190_BipolarScale			(1 << (24-1))	//! 2 ^ (24bit -1) bipolar scale
#define AD7190_UnipolarScale		(1 << 24)		//! 2 ^ (24bit) unipolar scale
#define AD7190_bits					24
#define AD7190_ReadSize				(24 / 8)

//! > Common

/* 
 * /has-delay
 * */
void AD7190_Init(U16 samples
	, AD7190_Refrence_t refrence
	, AD7190_Channels_t inputs
	, AD7190_Polarity_t polarity
	, AD7190_Gain_t gain
	, AD7190_Operation_t operation
	, AD7190_Options_t options);

extern void AD7190_Active(void);
extern void AD7190_Deactive(void);

/* RDY-Signal is not generating, when the part is placed in power-down mode 
 * or idle mode or when SYNC is taken low (or not activated!). */
status_code_t AD7190_WaitDRDY(void);
status_code_t AD7190_WaitDRDY_Pulse(U32 value);

/* Get chip data register(24-bit data - 3B) */
U8* AD7190_Get24bitData(void);

U8* AD7190_Read(AD7190_Registers_t reg);
static inline U32* AD7190_Read32(AD7190_Registers_t reg) { return (U32*)AD7190_Read(reg); }

void AD7190_Write(AD7190_Registers_t reg, U32 value);

typedef enum AD7190_Commands_enum
{
	/* Reset will reload all registers to their default value, 
	 * recommend to use 'cmd-reload' instead of this.
	 * */
	AD7190_CMD_Reset,
	/* SYNC must be apply after any calibration or changing inputs.
	 * */
	AD7190_CMD_Sync,
	AD7190_CMD_Reload,
	AD7190_CMD_ContinuousRead = ContinuousRead_On_cmd,
	AD7190_CMD_ExitRead = ContinuousRead_Off_cmd,
} AD7190_Commands_t;
/* 
 * /has-delay
 * */
void AD7190_ExecuteCommand(AD7190_Commands_t cmd);

//! > Config REG

/* SYNC must be apply after any calibration or changing inputs.
 * */
void AD7190_WriteChannels(AD7190_Channels_t chs, bool exeSync);

//! > Mode REG

/* 
 * \param value: in Hz
 * */
void AD7190_WriteSampleRate(U16 clock);

void AD7190_WriteOperationMode(AD7190_Operation_t opm);
inline void AD7190_RunCalibration(AD7190_Calibration_t cal, bool exeSync)
{
	AD7190_WriteOperationMode((AD7190_Operation_t)cal);
	if (exeSync) { AD7190_ExecuteCommand(AD7190_CMD_Sync); }
}


#endif // AD7190_H_