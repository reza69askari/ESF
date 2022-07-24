/*
 * esf_io.h
 *
 * @Created: 7/11/2022 6:57:06
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_IO_H_
#define ESF_IO_H_

#include <compiler.h>

#if XMEGA
#	include <MPUs/atxmega/esf_atxmega_io.h>
#else
#	error ArgumentException
#endif


#endif // ESF_IO_H_