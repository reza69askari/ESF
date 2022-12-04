/*
 * esf_rtc.h #Driver
 * > RTCs chip controller
 *
 * @Created: 7/4/2022 10:08:24
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */
//////////////////////////////////////////////////////////////////////////
//! #Requires:
//
//! #Options:
// #define Conf_TimeKeeper		1//! DS3231_RTC
//////////////////////////////////////////////////////////////////////////

#ifndef ESF_RTC_H_
#define ESF_RTC_H_

#define Internal_RTC			0
#define DS3231_RTC				1
#define External_RTC			2	/* Manually ... //TODO: #ToDo$Important external rtc & internal temperature sensor ... */

#include <conf_board.h>
#include <esf_compiler.h>
#include "esf_time_type.h"
#include <time.h>

#ifndef Conf_TimeKeeper
#	define Conf_TimeKeeper		Internal_RTC
#endif // Conf_TimeKeeper

#ifdef __ExterTimeKeeper
#	undef __ExterTimeKeeper
#endif
#if (Conf_TimeKeeper == Internal_RTC)
#	include <rtc.h> //! add it on ASF
#elif (Conf_TimeKeeper == DS3231_RTC)
#	include <components/rtc/ds3231.h>
#	if (defined RTC_H || defined DRIVERS_RTC_RTC_H)
#		error "Conflict with asf, remove rtc on ASF-Wizard!"
#	endif
#	define __ExterTimeKeeper	ds3231
#	include "include/esf_rtc_ds3231.h"
#elif (Conf_TimeKeeper == External_RTC)
	extern void rtc_init(void);
	extern void rtc_set_time(timestamp_t time);
	extern timestamp_t rtc_get_time(void);
#else
#	error NotImplementedException
#endif

#ifdef __ExterTimeKeeper
#	define keeper_init		ATPASTE2(__ExterTimeKeeper, _init)
#	define keeper_set_time	ATPASTE2(__ExterTimeKeeper, _set_time)
#	define keeper_get_time	ATPASTE2(__ExterTimeKeeper, _get_time)
	
	static inline void rtc_init(void) { keeper_init(); }
	static inline void rtc_set_time(timestamp_t time) { keeper_set_time(time); }
	static inline timestamp_t rtc_get_time(void) { return keeper_get_time(); }
	//TODO: #ToDo below methods...
	//bool rtc_alarm_has_triggered(void);
	//void rtc_set_callback(rtc_callback_t callback);
	//static inline void rtc_set_alarm(uint32_t time){}
	//static inline void rtc_set_alarm_relative(TimeStamp offset) { rtc_set_alarm(rtc_get_time() + offset); }
	
	//ISR(RTC_OVF_vect, ISR_NAKED)
	//{
	//	system_tick();
	//	reti();
	//}
#endif

#endif // ESF_RTC_H_