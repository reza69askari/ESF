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

#define UTC(h,m)		(h * ONE_HOUR + m * ONE_MINUTE)

typedef enum TimeZones_enum
{
	Tehran				= UTC(3,30)
} TimeZone_t;

static inline void time_set_zone(TimeZone_t zone)
{
	#include <util/usa_dst.h>
	set_zone(Tehran);
	switch (zone)
	{
		default: set_dst(usa_dst); break;
	}
}


#endif // ESF_TIME_ZONES_H_