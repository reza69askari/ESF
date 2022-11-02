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
#define __size2print(num, f)					((num)==1)? U8 ## f : ((num)==2)? U16 ## f : U32 ## f
#define __PRTP(b)								do { printf(__size2print(sizeof(*b), PX0 " "), *(b)); } while (0);
#define __NavBuf(b, len)						do { __PRTP(b++); } while (--l);
#define __LogBuffer(buf, l)						do { typeof(*buf)*b=buf; __NavBuf(b, l); } while (0);
#define __LogBufferF(buf,len) \
	do { \
		if (!Log_IsSuspend() && __LogLvl <= Trace) { \
			size_t l=((len)!=0)?len:sizeof_array(buf); \
			_Log(Trace, PSTR("buffer print '" #buf "' addr=%u size=%d"), (U16)buf, (len)); \
			__LogBuffer((buf),l); PRINTF_P(NewLine); \
		} \
	} while (0);

//! >>>>>>> to print variable
#define __LogParam(lvl, format, ...) _Log(lvl, format, __VA_ARGS__)
string_P(printU8 , "%S= " U8P   "(" U8P0X0   ")");
string_P(printS8 , "%S= " S8P   "(" S8P0X0   ")");
string_P(printU16, "%S= " U16P  "(" U16P0X0  ")");
string_P(printS16, "%S= " S16P  "(" S16P0X0  ")");
string_P(printU32, "%S= " U32P  "(" U32P0X0  ")");
string_P(printS32, "%S= " S32P  "(" S32P0X0  ")");
string_PArray(printTypes) = { printU8, printS8, printU16, printS16, printU32, printS32, };
#define __LogPUP(num)							(((num) == 1) ? 0 : ((num) == 2) ? 2 : 4)
#define __LogPSUP(num)							(__LogPUP(sizeof(num)) + (IsSigned(num) ? 1 : 0))
#define __LogP(num)								(&printTypes[(__LogPSUP(num))])

#pragma endregion InternalUsage

//! >>>>>>> >>>>>>> Avalble methods:

//! >>>>>>> Base methods

#define LogTrace(S, ...)						Log(Trace		, (S), ##__VA_ARGS__)
#define LogDebug(S, ...)						Log(Debug		, (S), ##__VA_ARGS__)
#define LogInformation(S, ...)					Log(Information	, (S), ##__VA_ARGS__)
#define LogWarning(S, ...)						Log(Warning		, (S), ##__VA_ARGS__)
#define LogError(S, ...)						Log(Error		, (S), ##__VA_ARGS__)
#define LogCritical(S, ...)						Log(Critical	, __FILE__ "::%d " S, __LINE__, ##__VA_ARGS__)

//! >>>>>>> Helper methods

#define LogParameter_lvl(lvl, var)				__LogParam(lvl, string_PRF((__LogP((var)))), PSTR(#var), var, var)
#define LogParameter(var)						LogParameter_lvl(Trace, var)
#define LogTP(var)								LogParameter(var)

#define LogString_lvl(lvl, str)					Log(lvl, #str ": %s", str)
#define LogString(str)							LogString_lvl(Trace, str)

#define LogBuffer(buf)							__LogBufferF((buf),0)
#define LogMemory(pointer)						__LogBufferF(((U8*)(pointer)),sizeof(*(pointer)))
#define LogStruct(buf,len)						__LogBufferF(((U8*)(buf)),len)

#define LogIf_lvl(lvl, condition, S, ...)		if ((condition)) { Log(lvl, S, ##__VA_ARGS__); }

#define LogException_lvl(lvl, exc, S, ...)		Log(lvl, (STR(exc) S), ##__VA_ARGS__)
#define LogExceptionParam_lvl(lvl, exc, arg)	Log(lvl, (STR(exc) "(%S: %d)"), PSTR(STR(arg)), arg)

//! >>>>>>> IsDevelopment

#if ((defined DEBUG) || (defined RELEASE_KEEP_LOGS)) //TODO: #ToDo better def name!
#	ifdef Conf_Log_UseLessMemory
#		define LOG_FUNCTION()					Log(Debug, (__FILE__ "::L#%03d..."), __LINE__)
#	else
#		define LOG_FUNCTION()					Log(Debug, (__FILE__ "::%s..."), __FUNCTION__)
#	endif
#	define LOG_METHOD(foo_name)					Log(Debug, (__FILE__ "::%S..."), PSTR(STR(foo_name)))
#	define LOG_ASSERT(hangOn, S, ...)			LogIf_lvl(Critical, hangOn, (STR(S) " " #hangOn), ##__VA_ARGS__)
#	define LOG_EXCEPTION(exc, S, ...)			Log(Critical, (" " STR(exc) " " S), ##__VA_ARGS__)
#	define LOG_FAULT(exc, S, ...)				Log(Error, (" " STR(exc) S), ##__VA_ARGS__)
#	define LOG_PARAM(var)						LogParameter(var)
#else
#	define LOG_FUNCTION()						void0
#	define LOG_METHOD(foo_name)					void0
#	define LOG_ASSERT(hangOn, S, ...)			void0
#	define LOG_EXCEPTION(exc, S, ...)			void0
#	define LOG_FAULT(exc, S, ...)				void0
#	define LOG_PARAM(var)						void0
#endif // DEBUG || RELEASE_KEEP_LOGS


#endif // ESF_LOG_EXTENSION_H_