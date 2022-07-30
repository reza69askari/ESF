/*
 * esf_rtc_ds3231.h
 * > DS3231 translator for ESF-RTC
 * @Created: 7/23/2022 21:10:33
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_RTC_DS3231_H_
#define ESF_RTC_DS3231_H_

#include "../esf_time_type.h"
#include <components/rtc/ds3231.h>

#define _KeeperYearOffset	2000

static void _Keeper2Time(DS3231_DateTime_t* x, datetime_t* t)
{
	t->tm_sec	= BCD2DEC(x->Seconds);
	t->tm_min	= BCD2DEC(x->Minutes);
	t->tm_hour	= BCD2DEC(x->Hours);
	t->tm_mday	= BCD2DEC(x->Date);
	t->tm_wday	= BCD2DEC(x->Day) -1;
	t->tm_mon	= BCD2DEC((x->Month_Century & ~DS3231_Century_bm)) -1;
	t->tm_year	= BCD2DEC(x->Year) +_KeeperYearOffset -DateTime_YearOffset;
	t->tm_year	+= (x->Month_Century & DS3231_Century_bm ? 100 : 0);
}
static void _Time2Keeper(datetime_t* t, DS3231_DateTime_t* x)
{
	bool _century = (t->tm_year +DateTime_YearOffset -_KeeperYearOffset) > 99;
	x->Seconds	= DEC2BCD(t->tm_sec);
	x->Minutes	= DEC2BCD(t->tm_min);
	x->Hours	= DEC2BCD(t->tm_hour);
	x->Date 	= DEC2BCD(t->tm_mday);
	x->Day		= DEC2BCD((t->tm_wday+1));
	x->Month_Century= DEC2BCD((t->tm_mon+1)) | (_century ? DS3231_Century_bm : 0);
	x->Year		= DEC2BCD((t->tm_year +DateTime_YearOffset -_KeeperYearOffset -(_century ? 100 : 0)));
}
static void ds3231_set_time(timestamp_t time)
{
	DS3231_DateTime_t kDT;
	datetime_t *dt = TimeStamp2DateTime_UTC(&time);
	_Time2Keeper(dt, &kDT);
	ds3231_set_datetime(&kDT);
}
static timestamp_t ds3231_get_time(void)
{
	DS3231_DateTime_t kDT;
	datetime_t dt;
	ds3231_get_datetime(&kDT);
	_Keeper2Time(&kDT, &dt);
	return DateTime2TimeStamp_UTC(&dt);
}


#endif // ESF_RTC_DS3231_H_