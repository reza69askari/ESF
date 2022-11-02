/*
 * esf_DateTime.h
 * > use on 'strftime(...)'
 *
 * @Created: 6/12/2022 20:10:35
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_DATETIME_H_
#define ESF_DATETIME_H_

//////////////////////////////////////////////////////////////////////////

#define	yday			"%j"	/* "184"		%j day of the year (001–366). [tm_yday] */
#define	wday			"%w"	/* "0"			%w weekday (0–6), where Sunday is 0. [tm_wday] */
#define	Wday			"%u"	/* "7"			%u weekday (1–7), where Monday is 1 (ISO 8601). [tm_wday] */
#define	yyyy			"%G"	/* "2022"		%G week-based year (e.g., 1997) (ISO 8601). [tm_year, tm_wday, tm_yday] */
//#define yyyy			"%Y"	/* "2022"		%Y year (e.g., 1997). [tm_year] */
#define	YY				"%C"	/* "20"			%C year divided by 100 and truncated to an integer, (00–99). [tm_year] */
#define	yy				"%g"	/* "22"			%g last 2 digits of the week-based year (00–99) (ISO 8601). [tm_year, tm_wday, tm_yday] */
//#define yy			"%y"	/* "22"			%y last 2 digits of the year (00–99). [tm_year] */
#define	MMMM			"%B"	/* "July"		%B locale’s full month name. one of "January", "February", . . . , "December". [tm_mon] */
#define	MMM				"%b"	/* "Jul"		%b locale’s month name. the first three characters of %B. [tm_mon] */
//#define MMM			"%h"	/* "Jul"		%h is equivalent to "%b". [tm_mon] */
#define	MM				"%m"	/* "07"			%m month (01–12). [tm_mon] */
#define	dddd			"%A"	/* "Sunday"		%A locale’s full weekday name. one of "Sunday", "Monday", . . . , "Saturday". [tm_wday] */
#define	ddd				"%a"	/* "Sun"		%a locale’s weekday name. the first three characters of %A. [tm_wday] */
#define	dd				"%d"	/* "03"			%d day of the month (01–31). [tm_mday] */
#define	sd				"%e"	/* " 3"			%e day of the month (1–31); a single digit is preceded by a space. [tm_mday] */
#define	HH				"%H"	/* "02"			%H hour (24-hour clock) (00–23). [tm_hour] */
#define	hh				"%I"	/* "02"			%I hour (12-hour clock) (01–12). [tm_hour] */
#define	mm				"%M"	/* "07"			%M minute (00–59). [tm_min] */
#define	ss				"%S"	/* "04"			%S second (00–59). [tm_sec] */
#define	tt				"%p"	/* "AM"			%p AM/PM. [tm_hour] */
#define	k				"%z"	/* "+0330"		%z offset from UTC in the ISO 8601 format "-0430" (meaning 4 hours 30 minutes behind UTC, west of Greenwich), or by no characters if no time zone is determinable. [tm_isdst] */
#define	wyday			"%V"	/* "26"			%V ISO 8601 week number (01–53). [tm_year, tm_wday, tm_yday] */
//#define wyday			"%U"	/* "27"			%U week number of the year (the first Sunday as the first day of week 1) (00–53). [tm_year, tm_wday, tm_yday] */
//#define wyday			"%W"	/* "26"			%W week number of the year (the first Monday as the first day of week 1) (00–53). [tm_year, tm_wday, tm_yday] */
#define	TimeP			"%r"	/* "5:07:04 AP"	%r locale’s 12-hour clock time. [tm_hour, tm_min, tm_sec] */
#define	ShortTime24		"%R"	/* "02:07"		%R "%H:%M". [tm_hour, tm_min ] */
#define	LongTime24		"%T"	/* "02:07:04"	%T "%H:%M:%S" (the ISO 8601 time format). [tm_hour, tm_min, tm_sec] */
#define	Time24			"%X"	/* "02:07:04"	%X locale’s time. */
#define	ShortDateP		"%D"	/* "07/03/22"	%D "%m/%d/%y". [tm_mon, tm_mday, tm_year] */
#define	DateP			"%x"	/* "07/03/22"	%x locale’s date. */
#define	ISO_Date		"%F"	/* "2022-07-03"	%F "%Y-%m-%d" (the ISO 8601 date format). [tm_year, tm_mon, tm_mday] */
#define	FullDateTimeP	"%c"	/* "Sun Jul 03 02:07:04 2022"	%c locale’s date and time. %c equivalent to "%a %b %e %T %Y". */
//#define	NL			"%n"	/* "\n"			%n is replaced by a new-line character. */
#define	Tab				"%t"	/* "\t"			%t is replaced by a horizontal-tab character. */
//#define				"%Z"	/* "?"			%Z is replaced by the locale’s time zone name or abbreviation, or by no characters if no time zone is determinable. [tm_isdst] */
//#define				"%%"	/* "%"			%% is replaced by %. */

//////////////////////////////////////////////////////////////////////////

#define ShortTime					hh":"mm" "tt						/* 1:45 PM (en-US) */
#define LongTime					hh":"mm":"ss" "tt					/* 1:45:30 PM (en-US) */
#define ShortDate					M"/"dd"/"yyyy						/* 6/15/2009 (en-US) */
#define GeneralDateShortTime		ShortDate" "ShortTime				/* 6/15/2009 1:45 PM (en-US) */
#define GeneralDateTime				ShortDate" "LongTime				/* 6/15/2009 1:45:30 PM (en-US) */
#define MonthDay					MMMM" "dd							/* June 15 (en-US) */
#define YearMonth					MMMM" "yyyy							/* June 2009 (en-US) */
#define RFC1123						ddd", "dd" "MMM" "yyyy				/* Mon, 15 Jun 2009 20:45:30 GMT */
#define LongDate					dddd", "MMMM" "dd", "yyyy			/* Monday, June 15, 2009 (en-US) */
#define FullDateShortTime			LongDate" "ShortTime				/* Monday, June 15, 2009 1:45 PM (en-US) */
#define FullDateTime				LongDate" "LongTime					/* Monday, June 15, 2009 1:45:30 PM (en-US) */

//////////////////////////////////////////////////////////////////////////

#define PrintTime_Required			42 //! strlen("WEDNESDAY, SEPTEMBER 31, 2022 12:45:30 PM")


#endif // ESF_DATETIME_H_