/*
 * ds3231.h
 *
 * @Created: 6/5/2022 0:17:41
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */
//////////////////////////////////////////////////////////////////////////
//! #Requires:
// #define DS3231_TWI				&TWIC
//! #Options:
// #define Config_DS3231_TwiSpeed DS3231_I2C_FastSpeed //! DS3231_I2C_FastSpeed | DS3231_I2C_StandardSpeed
//////////////////////////////////////////////////////////////////////////

#ifndef DS3231_H_
#define DS3231_H_

#include "ds3231_io.h"
#include <esf_compiler.h>

typedef enum DS3231_Commands_enum
{
	DS3231_CMD_ChangeOCS = 1,
	DS3231_CMD_RequestNewTemperature,
	DS3231_CMD_ChangeSQW,
	DS3231_CMD_ChangeInterrupt,
	DS3231_CMD_ChangeO32KHz,
} DS3231_Commands_t;

void ds3231_init(void);

status_code_t ds3231_transfer(bool read, DS3231_Registers_t addr, void* buf, size_t len);
inline status_code_t ds3231_write(DS3231_Registers_t addr, void* buf, size_t len) { return ds3231_transfer(false, addr, buf, len); }
inline status_code_t ds3231_read(DS3231_Registers_t addr, void* buf, size_t len) { return ds3231_transfer(true, addr, buf, len); }

void ds3231_get_datetime(DS3231_DateTime_t* dt);
void ds3231_set_datetime(DS3231_DateTime_t* DT);
status_code_t ds3231_set_alarm(DS3231_Alerts_t alert, DS3231_Alarm_t* alarm, bool useInterrupt);

/* in case of system startup, to check if alert is passed during system was off. */
bool ds3231_is_alarm_passed(DS3231_Alerts_t alert);

DS3231_Status_t ds3231_read_status(void);

S16 ds3231_get_temperature_raw (bool askNewRequest);
static inline S8 ds3231_get_temperature_fast (bool askNewRequest)
{
	return (S8)(ds3231_get_temperature_raw(askNewRequest) >> DS3231_TEMP_MSB_gp);
}
static inline S16 ds3231_get_temperature (bool askNewRequest)
{
	S16 _temp;
	_temp = Swap16(ds3231_get_temperature_raw(askNewRequest));
	_temp = ((_temp >> DS3231_TEMP_HI_gp) *100) + 
		(((_temp & DS3231_TEMP_LO_gm) >> DS3231_TEMP_LSB_gp) *25);
	return _temp;
}

status_code_t ds3231_change_SQW(DS3231_SQW_t frq, bool keepPulseOnPowerLose);

status_code_t ds3231_change_O32KHz(bool turnOn);

status_code_t ds3231_freez_chip(void);


#endif // DS3231_H_