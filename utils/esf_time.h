/*
 * esf_time.h
 *
 * @Created: 8/21/2018 10:23:05
 *  @Author: MohammadReza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_TIME_H_
#define ESF_TIME_H_

#include <time.h>
#include <utils/esf_rtc.h>
#include "include/esf_time_zones.h"
#include "esf_time_type.h"

#include <esf_compiler.h>
#include "esf_string.h"
#include "esf_stdio.h"
#include "esf_print_type.h"
#include "esf_print_time.h"

//! >>>>>>> Time-Setters

static inline void time_update_system_time(void)
{
	set_system_time(rtc_get_time());
}

/*
 * Set GMT time (zone is effect on this if set, and read by local).
 */
static inline void time_set (timestamp_t time)
{
	rtc_set_time(time);
	set_system_time(time);
}

/* 
 * Ex: TimeSet_TimeStamp(1656868020)
 * */
static inline void time_set_Unix (timestamp_t unix_timestamp)
{
	unix_timestamp -= UNIX_OFFSET;
	time_set(unix_timestamp);
}

/* 
 * Ex: TimeSet_TimeStamp(1656868020)
 * */
static inline void time_set_NTP (timestamp_t ntp_timestamp)
{
	ntp_timestamp -= NTP_OFFSET;
	time_set(ntp_timestamp);
}

/* 
 * Ex:
<code>
TimeSet(&(tm_t*){
		.tm_sec		= 4,
		.tm_min 	= 7,
		.tm_hour	= 2,
		.tm_mday	= 3,
		//.tm_wday	= 0,
		.tm_mon 	= 6,
		.tm_year	= 2022-1900,
		//.tm_yday	= ,
		//.tm_isdst	= ,
	});
</code>
 * */
static inline void time_set_datetime (datetime_t* time)
{
	time_set(DateTime2TimeStamp(time));
}

/* 
 * \year: 0-9999
 * \month: 1-12
 * \day: 1-31
 * \hour24: 0-23
 * \minute: 0-59
 * \second: 0-59
 * */
static inline void time_set_s (U16 year, U8 month, U8 day, U8 hour24, U8 minute, U8 second)
{
	time_set_datetime(&(tm_t){
		.tm_sec		= second,
		.tm_min 	= minute,
		.tm_hour	= hour24,
		.tm_mday	= day,
		//.tm_wday	= ,
		.tm_mon 	= month -1,
		.tm_year	= year-DateTime_YearOffset,
		//.tm_yday	= ,
		//.tm_isdst	= ,
	});
}

//! >>>>>>> Time-Getters

/* Get timestamp of Time.Now
 * \p refTime can be NULL
 *  \ret is time-value not a pointer of time-value
 */
static inline timestamp_t time_get(timestamp_t* refTime, bool updateRequest)
{
	if (updateRequest) { time_update_system_time(); }
	return time(refTime);
}

/* Get Local(Zone effected) time of Time.Now
 */
static inline datetime_t* time_get_datetime(void)
{
	timestamp_t F = time_get(NULL, false);
	return TimeStamp2DateTime(&F);
}
/* Get GMT time of Time.Now
 */
static inline datetime_t* time_get_datetime_gmt(void)
{
	timestamp_t F = time_get(NULL, false);
	return TimeStamp2DateTime_UTC(&F);
}

//! >>>>>>> >>>>>>> Print

/* Ex: Time = 05/06/2018 10:30:58 +03:30
 *  \ret "2018-06-06 14:01:01"
 */
static inline string time_get_ISO (void)
{
	return isotime(time_get_datetime());
}
/* Ex: Time = 05/06/2018 10:30:58 +03:30
 *  \ret "Wed Jun 06 10:30:58 2018"
 */
static inline string time_get_GMT (void)
{
	return asctime(time_get_datetime());
}

static inline string time_get_format_r(string format, string refStr, size_t bufferSize)
{
	if (!refStr) { return refStr; }
	datetime_t* _w = time_get_datetime();
	
	_w->tm_isdst = 0;
	strftime(refStr, bufferSize, format, _w);
	return refStr;
}

static inline string time_get_string(void)
{
	static string _time;
	if (!_time) { _time = (string)malloc(PrintTime_Required); }
	return time_get_format_r(FullDateTime, _time, PrintTime_Required);
}

inline void print_time (void)
{
	#if (defined SystemTime_TC) //TODO: #ToDo system-time, and use RTC as tick of 1Hz... & add bool updateRequest
	printf_P("%s.%05u\t", time_get_ISO(), SystemTime_TC.CNT);
	#elif (defined SystemTick_TC)
	printf_P("%s.%05u\t", time_get_ISO(), SystemTick_TC.CNT);
	#else
	printf_P("%s\t", time_get_ISO());
	#endif
}

//! >>>>>>> >>>>>>> call me in a timer with 1Hz overflow
__always_inline static void Time_callback (void)
{
	system_tick();
}

static inline void time_init (void)
{
	rtc_init();
	
	//! if has time keeper, load system time from it
	time_update_system_time();
}


#endif /* ESF_TIME_H_ */