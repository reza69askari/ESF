/*
 * DS3231.c
 *
 * @Created: 7/2/2022 7:46:02
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

//////////////////////////////////////////////////////////////////////////
//! #Requires:
// #define DS3231_TWI				&TWIC
//! #Options:
// #define Config_DS3231_TwiSpeed	DS3231_I2C_FastSpeed //! DS3231_I2C_FastSpeed | DS3231_I2C_StandardSpeed
//////////////////////////////////////////////////////////////////////////


//! >>>>>>> DS3231-Library
#include <conf_board.h>
#include "ds3231.h"
#include <esf_compiler.h>
#include <utils/esf_wait.h>

//! >>>>>>>>>>>>>>>> Checker >>>>>>>>>>>>>>>>
#pragma region Checker

//! >>>>>>> Requires-Check
#ifndef DS3231_TWI
#	error ArgumentException 'DS3231_TWI' is required
#endif

//! >>>>>>> Options-Check
#ifndef Config_DS3231_TwiSpeed
#	define Config_DS3231_TwiSpeed	DS3231_I2C_FastSpeed
#elif (Config_DS3231_TwiSpeed != DS3231_I2C_FastSpeed && Config_DS3231_TwiSpeed != DS3231_I2C_StandardSpeed)
#	error ArgumentException: Config_DS3231_TwiSpeed
#endif

#pragma endregion Checker
//! <<<<<<<<<<<<<<<< .Checker <<<<<<<<<<<<<<<<

void ds3231_init(void)
{
	LOG_FUNCTION();
	twi_master_options_t opt = {
		.speed = Config_DS3231_TwiSpeed,
		.chip  = DS3231_ChipAddress
	};
	twi_master_setup(DS3231_TWI, &opt);
}

status_code_t ds3231_transfer(bool read, DS3231_Registers_t addr, void* buf, size_t len)
{
	LogDebug("DS3231 update request... A:%X - R/W:%d", addr, read);
	if (!buf) { return ERR_NO_MEMORY; }
	twi_package_t dPackage = {
		//! TWI chip address to communicate with.
		.chip = DS3231_ChipAddress, //! Chip address
		//! TWI address/commands to issue to the other chip (node).
		.addr = value2array24(addr), //! Instruction Byte
		//! Length of the TWI data address segment (1-3 bytes).
		.addr_length = 1,
		//! Where to find the data to be written.
		.buffer = buf,
		//! How many bytes do we want to write.
		.length = len,
		//! Whether to wait if bus is busy (false) or return immediately (true)
		.no_wait = false, //! false: do wait
	};
	LOG_ASSERT(pmic_get_enabled_levels() & CONF_PMIC_INTLVL, STR(IOException));
	status_code_t _s = twi_master_transfer(DS3231_TWI, &dPackage, read);
	LogStruct(buf, len);
	return _s;
}

Union16 __DS3231_CNT = { .u8[0] = 0b00011100, .u8[1] = 0b10001111 };
static status_code_t _ds3231_apply_command(DS3231_Commands_t cmd_c, U8 param)
{
	switch (cmd_c)
	{
		case DS3231_CMD_ChangeOCS:
			WriteFlags(__DS3231_CNT.u8[0]
				, DS3231_CTL_EOCS_bm
				, param ? 0 : DS3231_CTL_EOCS_bm);
			break;
		
		case DS3231_CMD_RequestNewTemperature:
			WriteFlags(__DS3231_CNT.u8[0]
				, DS3231_CTL_CONV_bm
				, DS3231_CTL_CONV_bm);
			break;
		
		case DS3231_CMD_ChangeO32KHz:
			WriteFlags(__DS3231_CNT.u8[1]
				, DS3231_CTL2_EN32KHz_bm
				, param ? DS3231_CTL2_EN32KHz_bm : 0);
			break;
		
		case DS3231_CMD_ChangeInterrupt:
			WriteFlags(__DS3231_CNT.u8[0]
				, DS3231_CTL_A1IE_bm | DS3231_CTL_A2IE_bm | DS3231_CTL_INTCN_bm
				, param & ~DS3231_CTL_INTCN_bm);
			break;
			
		case DS3231_CMD_ChangeSQW:
			WriteFlags(__DS3231_CNT.u8[0]
				, DS3231_CTL_BBSQW_bm | DS3231_CTL_RSs_gm | DS3231_CTL_INTCN_bm
				, param | DS3231_CTL_INTCN_bm);
			break;
		
		default: return ERR_INVALID_ARG;
	}
	
	status_code_t _s = ds3231_write(DS3231_REG_Control, &__DS3231_CNT, sizeof(__DS3231_CNT));
	__DS3231_CNT.u8[0] &= ~(DS3231_CTL_CONV_bm);
	__DS3231_CNT.u8[1] &= ~(0x00);
	__DS3231_CNT.u8[0] |= 0x00;
	__DS3231_CNT.u8[1] |= 0b10000111; //! keep flags unchanged on next unwanted write
	return _s;
}

DS3231_Status_t ds3231_read_status(void)
{
	U8 buf;
	ds3231_read(DS3231_REG_Control_Status, &buf, sizeof(buf));
	return buf;
}

bool ds3231_is_alarm_passed(DS3231_Alerts_t alert)
{
	DS3231_Status_t _s = ds3231_read_status();
	switch (alert)
	{
		case DS3231_Alarm1: return HasFlag(_s, DS3231_Status_A1F_gc);
		case DS3231_Alarm2: return HasFlag(_s, DS3231_Status_A2F_gc);
		default: return false;
	}
}

static status_code_t ds3231_is_ready(void)
{
	return HasFlag(ds3231_read_status(), DS3231_Status_BSY_gc) ? ERR_BUSY : STATUS_OK;
}

S16 ds3231_get_temperature_raw(bool askNewRequest)
{
	if (askNewRequest)
	{
		U16 _timeout;
		WaitUntil_Action(ds3231_is_ready(), _timeout, 2000);
		_ds3231_apply_command(DS3231_CMD_RequestNewTemperature, true);
	}
	S16 _temp;
	ds3231_read(DS3231_REG_Temp_MSB, &_temp, sizeof(_temp));
	return _temp;
}

void ds3231_get_datetime(DS3231_DateTime_t* dt)
{
	ds3231_read(DS3231_REG_Seconds, dt, sizeof(DS3231_DateTime_t));
}

void ds3231_set_datetime(DS3231_DateTime_t* DT)
{
	ds3231_write(DS3231_REG_Seconds, DT, sizeof(DS3231_DateTime_t));
}

status_code_t ds3231_set_alarm(DS3231_Alerts_t alert, DS3231_Alarm_t* alarm, bool useInterrupt)
{
	switch(alert)
	{
		case DS3231_Alarm1:
			if (useInterrupt) { _ds3231_apply_command(DS3231_CMD_ChangeInterrupt, DS3231_CTL_A1IE_bm); }
			return ds3231_write(DS3231_REG_Alarm1_Seconds, alarm, sizeof(DS3231_Alarm1_t));
		case DS3231_Alarm2:
			if (useInterrupt) { _ds3231_apply_command(DS3231_CMD_ChangeInterrupt, DS3231_CTL_A2IE_bm); }
			return ds3231_write(DS3231_REG_Alarm2_Minutes, &alarm->Alarm1_Minutes, sizeof(DS3231_Alarm2_t));
		default: break;
	}
	LogExceptionParam_lvl(Error, ArgumentException, alert);
	return ERR_INVALID_ARG;
}

status_code_t ds3231_change_SQW(DS3231_SQW_t frq, bool keepPulseOnPowerLose)
{
	return _ds3231_apply_command(DS3231_CMD_ChangeSQW, frq | (keepPulseOnPowerLose ? DS3231_CTL_BBSQW_bm : 0));
}

status_code_t ds3231_change_O32KHz(bool turnOn)
{
	return _ds3231_apply_command(DS3231_CMD_ChangeO32KHz, turnOn);
}

status_code_t ds3231_freez_chip(void)
{
	return _ds3231_apply_command(DS3231_CMD_ChangeOCS, true);
}
