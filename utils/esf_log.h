/*
 * esf_log.h
 *
 * @Created: 5/29/2022 6:59:56
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */
//////////////////////////////////////////////////////////////////////////
//! #Options:
// Conf_Log_UseLessMemory	//! Replace 'file,function-name' with 'file,line-number'
//////////////////////////////////////////////////////////////////////////

#ifndef ESF_LOG_H_
#define ESF_LOG_H_

//! > Requires
#include <conf_board.h>
#include <esf_compiler.h>
#include <progmem.h>
#include "esf_stdio.h"
#include "esf_print_type.h"

OptionalMethod(string print_time(void));

typedef enum Log_Levels_enum
{
	Trace = 0,
	Debug,
	Information,
	Warning,
	Error,
	Critical,
	None,
} LogLvl_t;
LogLvl_t __LogLvl;

void Log_Init(void);

void Log_SetLevel(LogLvl_t newState);

void _Log_v(LogLvl_t lvl, PGM_P s, va_list args);

void _Log(LogLvl_t lvl, PGM_P format, ...);

void Log_Suspend(state_t state);
bool Log_IsSuspend(void);

//! > Avalble methods:

#define Log(lvl, S, ...)			_Log(lvl, PSTR(S), ##__VA_ARGS__)

//! > Optional
#include "include/esf_log_extension.h"
#include "include/esf_log_info.h"
#include "include/esf_fifo_memory.h"
#include "esf_errors.h"
#include "esf_string.h"
#include "esf_string_progmem.h"


#endif // ESF_LOG_H_