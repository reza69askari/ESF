/*
 * esf.c
 *
 * @Created: 8/19/2022 18:06:05
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#include <conf_board.h>
#include <esf_compiler.h>
#include <esf.h>

#ifdef ESF_LOG_H_ //! ESF\utils\esf_log.h

static LogLvl_t __log_level;
static state_t __log_suspend = Disable;

void Log_Init(void)
{
	__log_suspend = Disable;
	#ifdef DEBUG
	__log_level = Trace;
	#else
	__log_level = Information;
	#endif // DEBUG
}

void Log_Suspend(state_t state) { __log_suspend = state; }
bool Log_IsSuspend(void) { return __log_suspend != 0; }

void Log_SetLevel(LogLvl_t newState)
{
	__log_level = newState > Critical ? Critical : newState;
}

__attribute__ ((format(printf,2,0)))
void _Log_v(LogLvl_t lvl, PGM_P s, va_list args)
{
	if (__log_suspend) { return; }
	if (__log_level > lvl) { return; }
	string _time = print_time ? print_time() : "";
	switch (lvl)
	{
		case Trace		: PRINTF_P("[%s  %-6S]  ", _time, PSTR(STR(Trace ))); break;
		case Debug		: PRINTF_P("[%s  %-6S]  ", _time, PSTR(STR(Debug ))); break;
		case Information: PRINTF_P("[%s  %-6S]  ", _time, PSTR(STR(Info  ))); break;
		case Warning	: PRINTF_P("[%s  %-6S]  ", _time, PSTR(STR(Warn  ))); break;
		case Error		: PRINTF_P("[%s  %-6S]  ", _time, PSTR(STR(Error ))); break;
		case Critical	: PRINTF_P("[%s  %-8S]  ", _time, PSTR(STR(Critical))); break;
		default: return;
	}
	//free(_time);
	vprintf_P(s, args);
	//Exception(lvl == Critical);
	if (lvl == Critical)
	{
		char c;
		scan_get_param("press any key to continue with this error...", "%c", &c);
	}
	PRINTF_P(NewLine);
}

__attribute__ ((format(printf,2,3)))
void _Log(LogLvl_t lvl, PGM_P format, ...)
{
	va_list args; va_start(args, format);
	_Log_v(lvl, format, args);
	va_end(args);
}

#endif // ESF_LOG_H_
