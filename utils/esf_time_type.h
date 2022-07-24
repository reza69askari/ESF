/*
 * esf_time_type.h
 *
 * @Created: 7/4/2022 18:49:57
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_TIME_TYPE_H_
#define ESF_TIME_TYPE_H_

#include <time.h>
#include <stdint.h>

typedef struct tm tm_t, datetime_t, DateTime;
#define YEAR_OFFSET		1900
#define TimeYearOffset	YEAR_OFFSET
typedef time_t timestamp_t, TimeStamp;
#define TimeStampP		U32P
/* Return converted time, effected by zone
 */
#define TimeStamp2DateTime(ts)		(datetime_t *)localtime((const timestamp_t *)ts)
/* Return converted time without zone effect
 */
#define TimeStamp2DateTime_UTC(ts)	(datetime_t *)gmtime((const timestamp_t *)ts)
/* Convert Timestamp(time_t) to DateTime(datetime_t), 
 *   tm_wday & tm_yday are ignored and will be update if suggest.
 */
#define DateTime2TimeStamp(tm)		(timestamp_t)mktime((datetime_t *)tm)
/* Convert Timestamp(time_t) to DateTime(datetime_t), 
 *  tm_wday & tm_yday are ignored.
 */
#define DateTime2TimeStamp_UTC(tm)	(timestamp_t)mk_gmtime((datetime_t *)tm)

/* Return the length of month, given the year and month, where 'month' is in the range 0 to 11.
 *  ('year' since 1900: YEAR_OFFSET is applying)
 */
static inline uint8_t time_month_days (int16_t year, uint8_t month)
{
	return month_length((year + YEAR_OFFSET), (month + 1));
}


#endif // ESF_TIME_TYPE_H_