/*
 * esf_GPS.h
 *
 * @Created: 6/13/2022 7:53:37
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 */
//////////////////////////////////////////////////////////////////////////
//! #Requires:
// 
//! #Options:
// 
//////////////////////////////////////////////////////////////////////////

#ifndef ESF_GPS_H_
#define ESF_GPS_H_

//#include <esf_common.h>

typedef enum Units_enum
{
	Meter,
	Inch,
} Units_t;
//TODO: #ToDo in esf_gps
typedef union GPS_union
{
	struct {
		char Latitude[12];	//! Format: ddmm.mmmmmm, Ex:
		bool NSIndicator;	//! N/S Indicator
		char Longitude[13];	//! Format: dddmm.mmmmmm, Ex:
		bool EWIndicator;	//! E/W Indicator
		U8 SatellitesUsed;	//! Range 0 to 12
	} T1;
	U8 data[28];
	char str[30];
} GPS_t;

#endif // ESF_GPS_H_