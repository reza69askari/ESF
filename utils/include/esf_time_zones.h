/*
 * esf_time_zones.h
 *
 * @Created: 7/24/2022 21:03:04
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_TIME_H_
#  error "Include <utils/esf_time.h> instead of this file."
#endif

#ifndef ESF_TIME_ZONES_H_
#define ESF_TIME_ZONES_H_

#include <time.h>

#ifndef ONE_MINUTE
#define ONE_MINUTE		60	/** One minute, expressed in seconds */
#endif // ONE_MINUTE
#ifndef ONE_HOUR
#define ONE_HOUR		3600		//! 1 Hour	3600 Seconds
#endif // ONE_HOUR
#ifndef ONE_DAY
#define ONE_DAY			86400		//! 1 Day	86400 Seconds
#endif // ONE_DAY
#define ONE_WEEK		604800		//! 1 Week	604800 Seconds
#define ONE_MONTH		2629743		//! 1 Month (30.44 days)	2629743 Seconds
#define ONE_YEAR 		31556926	//! 1 Year (365.24 days)	31556926 Seconds

#define UTC(h,m)		(h * ONE_HOUR + m * ONE_MINUTE)

typedef enum TimeZones_enum
{
	Tehran				= UTC(3,30)
} TimeZone_t;

static inline void time_set_zone(TimeZone_t zone)
{
	LOG_FUNCTION();
	
	#include <util/usa_dst.h>
	set_zone(Tehran);
	switch (zone)
	{
		default: set_dst(usa_dst); break;
	}
}


#endif // ESF_TIME_ZONES_H_