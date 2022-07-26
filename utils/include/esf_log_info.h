/*
 * esf_log_info.h
 *
 * @Created: 7/11/2022 8:24:12
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */
//////////////////////////////////////////////////////////////////////////
//! #Requires:
// #define Device_Name					"ESF made by MR.A"
// #define Device_BuildVersion			".91111422001"	//! .91 MM DD YY BUILD[3]
// #define Device_Version				"V01"
// #define Device_HardwareRelease		"July 11, 2022"
//! #Options:
// 
//////////////////////////////////////////////////////////////////////////

#ifndef ESF_LOG_H_
#  error "Include <utils/esf_log.h> instead of this file."
#endif

#ifndef ESF_LOG_INFO_H_
#define ESF_LOG_INFO_H_

#include <conf_board.h>

#ifdef DEBUG
#	define BuildType	"Debug"
#else
#	define BuildType	"Release"
#endif // DEBUG

static inline void Log_Info(void)
{
	LogInformation(NL NL NL NL ">>>>>>>>>> >>>>>>>>>> " Device_Name " " Device_Version Device_BuildVersion " <<<<<<<<<< <<<<<<<<<< ");
	#ifdef Device_Manufacture
	LogInformation(NL ">>>>>>>>>> Producted by " Device_Manufacture " <<<<<<<<<< ");
	#endif // Device_Manufacture
	LogInformation(NL ">>>>>>>>>> Hardware Released on " Device_HardwareRelease " <<<<<<<<<< ");
	LogInformation(NL ">>>>>>>>>> " BuildType " mode, build on %s %s <<<<<<<<<< ", __DATE__, __TIME__);
	//...
	LogInformation(NL);
}


#endif // ESF_LOG_INFO_H_