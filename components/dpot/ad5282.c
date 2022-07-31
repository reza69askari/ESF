/*
 * AD5282.c
 *
 * @Created: 5/9/2022 7:27:39
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */


//! >>>>>>> AD7190-Component-Library
#include <conf_board.h>
#include "AD5282.h"
#include <asf.h>
#include <esf_compiler.h>

//! >>>>>>>>>>>>>>>> Checker >>>>>>>>>>>>>>>>
#pragma region Checker

//! >>>>>>> Requires-Check
#ifndef AD5282_TWI
#	error ArgumentException 'AD5282_TWI' is required
#endif

//! >>>>>>> Options-Check
#if defined(Config_AD5282_PGA) && (Config_AD5282_PGA > AD5282_PrgAddress_gm)
#error "invalid address set, just define pins value"
#elif !defined(Config_AD5282_PGA)
#define Config_AD5282_PGA		0b00
#endif

#if defined(Config_AD5282_TwiSpeed) && (Config_AD5282_TwiSpeed > 400000UL)
#error "invalit set, maximum allowed speed is 400KHz"
#elif !defined(Config_AD5282_TwiSpeed)
#define Config_AD5282_TwiSpeed	100000
#endif

#pragma endregion Checker
//! <<<<<<<<<<<<<<<< .Checker <<<<<<<<<<<<<<<<

#define _AD5282_Address			(AD5282_BaseAddress | (Config_AD5282_PGA & AD5282_PrgAddress_gm))

status_code_t AD5282_Init(void)
{
	LogFunction();
	twi_master_options_t opt = {
		.speed = Config_AD5282_TwiSpeed,
		.chip  = _AD5282_Address
	};
	return twi_master_setup(AD5282_TWI, &opt);
}

status_code_t AD5282_Write(AD5282_CH_t channel, AD5282_Opt_t options, U8 value)
{
	twi_package_t dPotPackage = {
		.chip = _AD5282_Address, //! Chip address Byte
		.addr = value2array24((channel & AD5282_AB_bp) | (options & AD5282_Options_gm)), //! Instruction Byte
		.addr_length = 1,
		.buffer = &value, //! Data Byte
		.length = 1,
		.no_wait = false, //! do wait
	};
	status_code_t status = twi_master_write(AD5282_TWI, &dPotPackage);
	if (status != STATUS_OK) { return status; }
	
	//! Verify wrote data
	U8 reader = 0;
	dPotPackage.buffer = &reader;
	status = twi_master_read(AD5282_TWI, &dPotPackage);
	if (status != STATUS_OK) { return status; }
	
	return value == reader ? STATUS_OK : ERR_IO_ERROR;
}
