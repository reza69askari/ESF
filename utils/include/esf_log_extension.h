/*
 * esf_log_extension.h
 *
 * @Created: 7/24/2022 18:06:29
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_LOG_H_
#  error "Include <utils/esf_log.h> instead of this file."
#endif

#ifndef ESF_LOG_EXTENSION_H_
#define ESF_LOG_EXTENSION_H_

#include "../esf_string_progmem.h"

#pragma region InternalUsage

//! >>>>>>> to print buffer
#define __size2print(num, f)	((num)==1)? U8 ## f : ((num)==2)? U16 ## f : U32 ## f
#define __PRTP(b)				do { printf(__size2print(sizeof(*b), PX0 " "), *(b)); } while (0);
#define __NavBuf(b, len)		do { __PRTP(b++); } while (--l);
#define __LogBuffer(buf, l)		do { typeof(*buf)*b=buf; __NavBuf(b, l); } while (0);
#define __LogBufferF(buf,len)		\
	do { \
		if (__LogLvl <= Trace) { \
			size_t l=((len)!=0)?len:sizeof_array(buf); \
			_Log(Trace, PSTR("buffer print '" #buf "' addr=%u size=%d"), (U16)buf, (len)); \
			__LogBuffer((buf),l); printf_P(NewLine); \
		} \
	} while (0);

//! >>>>>>> to print variable
#define __LogParam(lvl, format, ...)	_Log(lvl, format, __VA_ARGS__)
string_P(printU8 , "%S= " U8P   "(" U8P0X0   ")");
string_P(printS8 , "%S= " S8P   "(" S8P0X0   ")");
string_P(printU16, "%S= " U16P  "(" U16P0X0  ")");
string_P(printS16, "%S= " S16P  "(" S16P0X0  ")");
string_P(printU32, "%S= " U32P  "(" U32P0X0  ")");
string_P(printS32, "%S= " S32P  "(" S32P0X0  ")");
string_PArray(printTypes) = { printU8, printS8, printU16, printS16, printU32, printS32, };
#define __LogPUP(num)					(((num) == 1) ? 0 : ((num) == 2) ? 2 : 4)
#define __LogPSUP(num)					(__LogPUP(sizeof(num)) + (IsSigned(num) ? 1 : 0))
#define __LogP(num)						(&printTypes[(__LogPSUP(num))])

#pragma endregion InternalUsage

//! >>>>>>> >>>>>>> Avalble methods:

#define LogTrace(s, ...)				Log(Trace		, (s), ##__VA_ARGS__)
#define LogDebug(s, ...)				Log(Debug		, (s), ##__VA_ARGS__)
#define LogInformation(s, ...)			Log(Information	, (s), ##__VA_ARGS__)
#define LogWarning(s, ...)				Log(Warning		, (s), ##__VA_ARGS__)
#define LogError(s, ...)				Log(Error		, (s), ##__VA_ARGS__)
#define LogCritical(s, ...)				Log(Critical	, __FILE__ "::%d " s, __LINE__, ##__VA_ARGS__)

#define LogIf(condition, lvl, s, ...)	if ((condition)) { Log(lvl, s, ##__VA_ARGS__); }
#define LogParameter(lvl, var)			__LogParam(lvl, string_PRF((__LogP(var))), PSTR(#var), var, var)
//#define LogParameter(lvl, var)		__LogParam(lvl, ((__LogP(var))), PSTR(#var), var, var)
#define LogString(lvl, str)				Log(lvl, #str ": %s", str)
#define LogBuffer(buf)					__LogBufferF((buf),0)
#define LogStruct(buf,len)				__LogBufferF(((U8*)(buf)),len)
#define LogException(lvl, exc, s, ...)	Log(lvl, (STR(exc) s), ##__VA_ARGS__)
#define LogExceptionP(lvl, exc, arg)	Log(lvl, (STR(exc) "(%S: %d)"), PSTR(STR(arg)), arg)
#define LogTraceString(str)				LogString(Trace, str)
#define LogTraceParam(var)				LogParameter(Trace, var)
#ifdef DEBUG
#	define LogFunction()				Log(Debug, (__FILE__ "::%s..."), __FUNCTION__)
#	define LogIf_Debug(condition, lvl, s, ...)	LogIf(condition, lvl, s, ##__VA_ARGS__)
#else
#	define LogFunction()				void0
#	define LogIf_Debug(condition, lvl, s, ...)	void0
#endif // DEBUG

//! alias short-name
#define LogTP(var)						LogTraceParam(var)

#endif // ESF_LOG_EXTENSION_H_