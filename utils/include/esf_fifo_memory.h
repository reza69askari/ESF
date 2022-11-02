/*
 * esf_fifo_memory.h
 *
 * @Created: 11/2/2022 13:25:15
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_LOG_H_
#  error "Include <utils/esf_log.h> instead of this file."
#endif


#ifndef ESF_FIFO_MEMORY_H_
#define ESF_FIFO_MEMORY_H_

#include "../esf_string_progmem.h"

/* 
 * copy new message into begin of log-memory
 * */
static inline void fifo_mem(PGM_P msg, string logMem, size_t logLen)
{
	//! message length; avoid string terminate
	size_t len = strlen_P(msg) -1;
	//! move old logs (as long as message + new-line)
	memmove(&logMem[len +2], &logMem[0], len);
	//! put message to begin
	memcpy_P(&logMem[0], msg, len);
	//! put new line after message
	logMem[len +1] = '\r'; logMem[len +2] = '\n';
	//! if reached to end of memory
	if (strlen(logMem) +len +2 >= logLen)
	{	//! truncate logs
		strcpy_P(&logMem[0] +logLen -4, PSTR("...\0"));
	}
}



#endif // ESF_FIFO_MEMORY_H_