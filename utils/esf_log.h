/*
 * esf_log.h
 *
 * @Created: 5/29/2022 6:59:56
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_LOG_H_
#define ESF_LOG_H_

//! > Requires
#include <progmem.h>
#include <esf_compiler.h>
#include "esf_stdio.h"
#include "esf_print_type.h"

OptionalMethod(void print_time(void));

typedef enum Log_Levels_enum
{
	Trace		= 0,
	Debug		,
	Information	,
	Warning		,
	Error		,
	Critical	,
	None		,
} LogLvl_t;
static LogLvl_t __LogLvl;

static inline void Log_Init(void)
{
	#ifdef DEBUG
	__LogLvl = Trace;
	#else
	__LogLvl = Information;
	#endif // DEBUG
}

static inline void Log_SetLevel(LogLvl_t newState)
{
	__LogLvl = newState > Critical ? Critical : newState;
}

__attribute__ ((format(printf,2,0)))
static inline void _Log_v(LogLvl_t lvl, PGM_P s, va_list args)
{
	if (__LogLvl > lvl) { return; }
	switch (lvl)
	{
		case Trace		: printf_P(">" STR(Trace		) ": "); break;
		case Debug		: printf_P(">" STR(Debug		) ": "); break;
		case Information: printf_P(">" STR(Information	) ": "); break;
		case Warning	: printf_P(">" STR(Warning		) ": "); break;
		case Error		: printf_P(">" STR(Error		) ": "); break;
		case Critical	: printf_P(">" STR(Critical		) ": "); break;
		default: return;
	}
	if (print_time) { print_time(); }
	vprintf_P(s, args);
	printf_P(NewLine);
	Exception(lvl == Critical);
}
__attribute__ ((format(printf,2,3)))
static inline void _Log(LogLvl_t lvl, PGM_P format, ...)
{
	va_list args; va_start(args, format);
	_Log_v(lvl, format, args);
	va_end(args);
}

//! >>>>>>> >>>>>>> Avalble methods:

#define Log(lvl, s, ...)				_Log(lvl, PSTR(s), ##__VA_ARGS__)

//! > Optional
#include "include/esf_log_extension.h"
#include "include/esf_log_info.h"
#include "esf_errors.h"
#include "esf_string.h"
#include "esf_string_progmem.h"


#endif // ESF_LOG_H_