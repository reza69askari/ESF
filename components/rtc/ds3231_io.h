/*
 * ds3231_io.h
 *
 * @Created: 7/2/2022 7:25:08
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

//////////////////////////////////////////////////////////////////////////
//                              TWI-DS3231                              //
//          Extremely Accurate I2C-Integrated RTC/TCXO/Crystal          //
//////////////////////////////////////////////////////////////////////////

#ifndef DS3231_IO_H_
#define DS3231_IO_H_

#include <esf_compiler.h>

#define DS3231_I2C_StandardSpeed	100000
#define DS3231_I2C_FastSpeed		400000
#define DS3231_ChipAddress			0x68

typedef struct DS3231_DateTime_struct
{
	U8 Seconds			; /* DS3231-Seconds-Register 0x00 (0-59) */
	U8 Minutes			; /* DS3231-Minutes-Register 0x01 (0-59) */
	U8 Hours			; /* DS3231-Hours-Register 0x02 (AM/PM 1–12 | 00–23) */
	U8 Day				; /* DS3231-Day-Register 0x03 (1-7) */
	U8 Date				; /* DS3231-Date-Register 0x04 (1-31) */
	U8 Month_Century	; /* DS3231-Century/Month-Register 0x05 (Century, 01–12) */
	U8 Year				; /* DS3231-Year-Register 0x06 (00–99) */
} DS3231_DateTime_t;

typedef struct DS3231_Alarm1_struct
{
	U8 Alarm1_Seconds	; /* DS3231-Alarm1-Seconds-Register 0x07 (A1M1, 0-59) */
	U8 Alarm1_Minutes	; /* DS3231-Alarm1-Minutes-Register 0x08 (A1M2, 0-59) */
	U8 Alarm1_Hours		; /* DS3231-Alarm1-Hours-Register 0x09 (A1M3, AM/PM 1–12 | 00–23) */
	U8 Alarm1_DayDate	; /* DS3231-Alarm1-Day/Date-Register 0x0A (A1M4, 1-7 | 1-31) */
} DS3231_Alarm1_t, DS3231_Alarm_t;

typedef struct DS3231_Alarm2_struct
{
	U8 Alarm2_Minutes	; /* DS3231-Alarm2-Minutes-Register 0x0B (A2M2, 0-59) */
	U8 Alarm2_Hours		; /* DS3231-Alarm2-Hours-Register 0x0C (A2M3, AM/PM 1–12 | 00–23) */
	U8 Alarm2_DayDate	; /* DS3231-Alarm2-Day/Date-Register 0x0D (A2M4, 1-7 | 1-31) */
} DS3231_Alarm2_t;

typedef struct DS3231_struct
{
	DS3231_DateTime_t DT;
	DS3231_Alarm1_t Alarm1;
	DS3231_Alarm2_t Alarm2;
	U8 Control			; /* DS3231-Control-Register 0x0E */
	U8 Control_Status	; /* DS3231-Control/Status-Register 0x0F */
	S8 AgingOffset		;
	S8 Temp_MSB			;
	U8 Temp_LSB			;
} DS3231_t;

typedef enum DS3231_Registers_enum
{
	DS3231_REG_Seconds				, /* DS3231-Seconds-Register 0x00 (0-59) */
	DS3231_REG_Minutes				, /* DS3231-Minutes-Register 0x01 (0-59) */
	DS3231_REG_Hours				, /* DS3231-Hours-Register 0x02 (AM/PM 1–12 | 00–23) */
	DS3231_REG_Day					, /* DS3231-Day-Register 0x03 (1-7) */
	DS3231_REG_Date					, /* DS3231-Date-Register 0x04 (1-31) */
	DS3231_REG_Month_Century		, /* DS3231-Century/Month-Register 0x05 (Century, 01–12) */
	DS3231_REG_Year					, /* DS3231-Year-Register 0x06 (00–99) */
	DS3231_REG_Alarm1_Seconds		, /* DS3231-Alarm1-Seconds-Register 0x07 (A1M1, 0-59) */
	DS3231_REG_Alarm1_Minutes		, /* DS3231-Alarm1-Minutes-Register 0x08 (A1M2, 0-59) */
	DS3231_REG_Alarm1_Hours			, /* DS3231-Alarm1-Hours-Register 0x09 (A1M3, AM/PM 1–12 | 00–23) */
	DS3231_REG_Alarm1_DayDate		, /* DS3231-Alarm1-Day/Date-Register 0x0A (A1M4, 1-7 | 1-31) */
	DS3231_REG_Alarm2_Minutes		, /* DS3231-Alarm2-Minutes-Register 0x0B (A2M2, 0-59) */
	DS3231_REG_Alarm2_Hours			, /* DS3231-Alarm2-Hours-Register 0x0C (A2M3, AM/PM 1–12 | 00–23) */
	DS3231_REG_Alarm2_DayDate		, /* DS3231-Alarm2-Day/Date-Register 0x0D (A2M4, 1-7 | 1-31) */
	DS3231_REG_Control				, /* DS3231-Control-Register 0x0E */
	DS3231_REG_Control_Status		, /* DS3231-Control/Status-Register 0x0F */
	DS3231_REG_AgingOffset			,
	DS3231_REG_Temp_MSB				,
	DS3231_REG_Temp_LSB				,
} DS3231_Registers_t;

/* DS3231-Seconds-Register 0x00 (0-59) */
/* Clock::Seconds-Value. */
#define DS3231_Seconds_bm			BITs60_gm
#define DS3231_Seconds_bp			BITs60_gp

/* DS3231-Minutes-Register 0x01 (0-59) */
/* Clock::Minutes-Value. */
#define DS3231_Minutes_bm			BITs60_gm
#define DS3231_Minutes_bp			BITs60_gp

/* DS3231-Hours-Register 0x02 (AM/PM 1–12 | 00–23) */
/* Clock::Hour-Config:
 * config & read this register as:
 * 0: 24H (00-23)
 * 1: 12H (1-12)
 * */
#define DS3231_1224_bm				BIT6_bm
#define DS3231_1224_bp				BIT6_bp
/* Clock::12H-Config:
 * config & read this register as:
 * 0: AM
 * 1: PM
 * */
#define DS3231_PMAM_bm				BIT5_bm
#define DS3231_PMAM_bp				BIT5_bp
/* Clock::12H-Value. */
#define DS3231_12Hour_gm			BITs40_gm
#define DS3231_12Hour_gp			BITs40_gp
/* Clock::24H-Value. */
#define DS3231_24Hour_gm			BITs50_gm
#define DS3231_24Hour_gp			BITs50_gp

/* DS3231-Day-Register 0x03 (1-7) */
/* Clock::Day-Value (DayOfWeek). */
#define DS3231_Day_gm				BITs20_gm
#define DS3231_Day_gp				BITs20_gp

/* DS3231-Date-Register 0x04 (1-31) */
/* Clock::Date-Value (DayOfMonth). */
#define DS3231_Date_gm				BITs50_gm
#define DS3231_Date_gp				BITs50_gp

/* DS3231-Century/Month-Register 0x05 (Century, 01–12) */
/* Clock::Century-Value. */
#define DS3231_Century_bm			BIT7_bm
#define DS3231_Century_bp			BIT7_bp
/* Clock::Month-Value. */
#define DS3231_Month_bm				BITs40_gm
#define DS3231_Month_bp				BITs40_gp

/* DS3231-Year-Register 0x06 (00–99) */
/* Clock::Year-Value. */
#define DS3231_Year_bm				BITs70_gm
#define DS3231_Year_bp				BITs70_gp

/* DS3231-Alarm1-Seconds-Register 0x07 (A1M1, 0-59) */
/* Alarm-1:Mode-b1. */
#define DS3231_A1_M1_bm				BIT7_bm
#define DS3231_A1_M1_bp				BIT7_bp
/* Alarm-2::Seconds-Value. */
#define DS3231_A1_Seconds_bm		BITs60_gm
#define DS3231_A1_Seconds_bp		BITs60_gp

/* DS3231-Alarm1-Minutes-Register 0x08 (A1M2, 0-59) */
/* Alarm-1:Mode-b2. */
#define DS3231_A1_M2_bm				BIT7_bm
#define DS3231_A1_M2_bp				BIT7_bp
/* Alarm-2::Minutes-Value. */
#define DS3231_A1_Minutes_bm		BITs60_gm
#define DS3231_A1_Minutes_bp		BITs60_gp

/* DS3231-Alarm1-Hours-Register 0x09 (A1M3, AM/PM 1–12 | 00–23) */
/* Alarm-1:Mode-b3. */
#define DS3231_A1_M3_bm				BIT7_bm
#define DS3231_A1_M3_bp				BIT7_bp
/* Alarm-1::Hour-Config:
 * config & read this register as:
 * 0: 24H (00-23)
 * 1: 12H (1-12)
 * */
#define DS3231_A1_1224_bm			BIT6_bm
#define DS3231_A1_1224_bp			BIT6_bp
/* Alarm-1::12H-Config:
 * config & read this register as:
 * 0: AM
 * 1: PM
 * */
#define DS3231_A1_PMAM_bm			BIT5_bm
#define DS3231_A1_PMAM_bp			BIT5_bp
/* Alarm-2::12H-Value. */
#define DS3231_A1_12Hour_gm			BITs40_gm
#define DS3231_A1_12Hour_gp			BITs40_gp
/* Alarm-2::24H-Value. */
#define DS3231_A1_24Hour_gm			BITs50_gm
#define DS3231_A1_24Hour_gp			BITs50_gp

/* DS3231-Alarm1-Day/Date-Register 0x0A (A1M4, 1-7 | 1-31) */
/* Alarm-1:Mode-b4. */
#define DS3231_A1_M4_bm				BIT7_bm
#define DS3231_A1_M4_bp				BIT7_bp
/* Alarm-1::Day/Date Config:
 * config & read this register as:
 * 0: Date (DayOfMonth: 1-31)
 * 1: Day (DayOfWeek: 1-7)
 * */
#define DS3231_A1_DYDT_bm			BIT6_bm
#define DS3231_A1_DYDT_bp			BIT6_bp
/* Alarm-1::Date-Value. */
#define DS3231_A1_Date_gm			BITs50_gm
#define DS3231_A1_Date_gp			BITs50_gp
/* Alarm-1::Day-Value. */
#define DS3231_A1_Day_gm			BITs30_gm
#define DS3231_A1_Day_gp			BITs30_gp

/* DS3231-Alarm2-Minutes-Register 0x0B (A2M2, 0-59) */
/* Alarm-2:Mode-b2. */
#define DS3231_A2_M2_bm				BIT7_bm
#define DS3231_A2_M2_bp				BIT7_bp
/* Alarm-2::Minutes-Value. */
#define DS3231_A2_Minutes_bm		BITs60_gm
#define DS3231_A2_Minutes_bp		BITs60_gp

/* DS3231-Alarm2-Hours-Register 0x0C (A2M3, AM/PM 1–12 | 00–23) */
/* Alarm-2:Mode-b3. */
#define DS3231_A2_M3_bm				BIT7_bm
#define DS3231_A2_M3_bp				BIT7_bp
/* Alarm-2::Hour-Config:
 * config & read this register as:
 * 0: 24H (00-23)
 * 1: 12H (1-12)
 * */
#define DS3231_A2_1224_bm			BIT6_bm
#define DS3231_A2_1224_bp			BIT6_bp
/* Alarm-2::12H-Config:
 * config & read this register as:
 * 0: AM
 * 1: PM
 * */
#define DS3231_A2_PMAM_bm			BIT5_bm
#define DS3231_A2_PMAM_bp			BIT5_bp
/* Alarm-2::12H-Value. */
#define DS3231_A2_12Hour_gm			BITs40_gm
#define DS3231_A2_12Hour_gp			BITs40_gp
/* Alarm-2::24H-Value. */
#define DS3231_A2_24Hour_gm			BITs50_gm
#define DS3231_A2_24Hour_gp			BITs50_gp

/* DS3231-Alarm2-Day/Date-Register 0x0D (A2M4, 1-7 | 1-31) */
/* Alarm-2:Mode-b4. */
#define DS3231_A2_M4_bm				BIT7_bm
#define DS3231_A2_M4_bp				BIT7_bp
/* Alarm-2::Day/Date Config:
 * config & read this register as:
 * 0: Date (DayOfMonth: 1-31)
 * 1: Day (DayOfWeek: 1-7)
 * */
#define DS3231_A2_DYDT_bm			BIT6_bm
#define DS3231_A2_DYDT_bp			BIT6_bp
/* Alarm-2::Date-Value. */
#define DS3231_A2_Date_gm			BITs50_gm
#define DS3231_A2_Date_gp			BITs50_gp
/* Alarm-2::Day-Value. */
#define DS3231_A2_Day_gm			BITs30_gm
#define DS3231_A2_Day_gp			BITs30_gp

/* DS3231-Control-Register 0x0E (0b00011100) */
/* ActiveLow, Enable oscillator (0):
 * > When you are going to lunch products, and they waiting for 
 *   end customer, this bit can be set on your company and oscillator 
 *   will waiting for next system-run. (Keep battery off!)
 * > Not effeting when VCC is exist.
 * */
#define DS3231_CTL_EOCS_bm			BIT7_bm
#define DS3231_CTL_EOCS_bp			BIT7_bp
/* Battery-Backed Square-Wave Enable (0): generate SQW on power Fail. */
#define DS3231_CTL_BBSQW_bm			BIT6_bm
#define DS3231_CTL_BBSQW_bp			BIT6_bp
/* Convert Temperature (BSY & CONV check is required before set new command). */
#define DS3231_CTL_CONV_bm			BIT5_bm
#define DS3231_CTL_CONV_bp			BIT5_bp
/* SQUARE-WAVE Output Frequency (0b11: 8.192KHz). */
#define DS3231_CTL_RSs_gm			BITs43_gm
#define DS3231_CTL_RSs_gp			BITs43_gp
/* ActiveLow, Interrupt Control, 0: Interrupt generator, 1: Square wave generator (1). */
#define DS3231_CTL_INTCN_bm			BIT2_bm
#define DS3231_CTL_INTCN_bp			BIT2_bp
/* Alarm 2 Interrupt Enable (0). */
#define DS3231_CTL_A2IE_bm			BIT1_bm
#define DS3231_CTL_A2IE_bp			BIT1_bp
/* Alarm 1 Interrupt Enable (0). */
#define DS3231_CTL_A1IE_bm			BIT0_bm
#define DS3231_CTL_A1IE_bp			BIT0_bp

/* DS3231-Control2/Status-Register 0x0F (0b1---1XXX) */
/* Oscillator Stop Flag: A logic 1 in this bit indicates that the oscillator 
 * either is stopped or was stopped for some period and may be used to judge 
 * the validity of the timekeeping data.
 * 1) The first time power is applied.
 * 2) The voltages present on both VCC and VBAT are insufficient to support 
 * oscillation.
 * 3) The EOSC bit is turned off in battery-backed mode.
 * 4) External influences on the crystal (i.e., noise, leakage, etc.).
 * 
 * > This bit remains at logic 1 until written to logic 0.
 * */
#define DS3231_STATUS_OSF_bm		BIT7_bm
#define DS3231_STATUS_OSF_bp		BIT7_bp
/* BITs64_gm: Reserved. */
/* Enable 32kHz Output. */
#define DS3231_CTL2_EN32KHz_bm		BIT3_bm
#define DS3231_CTL2_EN32KHz_bp		BIT3_bp
/* Busy. */
#define DS3231_STATUS_BSY_bm		BIT2_bm
#define DS3231_STATUS_BSY_bp		BIT2_bp
/* Alarm 2 Flag, Clear by 0, Set to 1 mean keep state. */
#define DS3231_STATUS_A2F_bm		BIT1_bm
#define DS3231_STATUS_A2F_bp		BIT1_bp
/* Alarm 1 Flag, Clear by 0, Set to 1 mean keep state. */
#define DS3231_STATUS_A1F_bm		BIT0_bm
#define DS3231_STATUS_A1F_bp		BIT0_bp

/* DS3231-Temperature */
#define DS3231_TEMP_HI_gp			8
#define DS3231_TEMP_LO_gm			0x00C0
#define DS3231_TEMP_LO_gp			6
/* DS3231-Temperature-MSB-Register 0x11 */
#define DS3231_TEMP_MSB_gm			BITs70_gm
#define DS3231_TEMP_MSB_gp			BITs70_gp
/* DS3231-Temperature-LSB-Register 0x12 */
#define DS3231_TEMP_LSB_gm			BITs76_gm
#define DS3231_TEMP_LSB_gp			BITs76_gp

typedef enum DS3231_Alerts_enum
{
	DS3231_Alarm1,
	DS3231_Alarm2,
	
	__DS3231_Allerts_Total
} DS3231_Alerts_t;

enum DS3231_Alarm1_MaskBits_enum
{
	AlarmOncePerSecond,							/* X1111 */
	AlarmWhenSecondsMatch,						/* X1110 */
	AlarmWhenMinutesSecondsMatch,				/* X1100 */
	AlarmWhenHoursMinutesSecondsMatch,			/* X1000 */
	AlarmWhenDateHoursMinutesSecondsMatch,		/* 00000 */
	AlarmWhenDayHoursMinutesSecondsMatch,		/* 10000 */
};
enum DS3231_Alarm2_MaskBits_enum
{
	AlarmOncePerMinute,							/* X111 (00 seconds of every minute) */
	AlarmWhenMinutesMatch,						/* X110 */
	AlarmWhenHoursMinutesMatch,					/* X100 */
	AlarmWhenDateHoursMinutesMatch,				/* 0000 */
	AlarmWhenDayHoursMinutesMatch,				/* 1000 */
};

typedef enum DS3231_SQUARE_WAVE_OUTPUT_FREQUENCY_enum
{
	DS3231_SQW_1Hz					= (0b00 << BITs43_gp),
	DS3231_SQW_1024Hz				= (0b01 << BITs43_gp),
	DS3231_SQW_4096Hz				= (0b10 << BITs43_gp),
	DS3231_SQW_8192Hz				= (0b11 << BITs43_gp),
} DS3231_SQW_t;
#define DS3231_SQW_ExistOnBattery	DS3231_CTL_BBSQW_bm

typedef enum DS3231_Status_enum
{
	DS3231_Status_OSF_gc			= DS3231_STATUS_OSF_bm,
	DS3231_Status_BSY_gc			= DS3231_STATUS_BSY_bm,
	DS3231_Status_A2F_gc			= DS3231_STATUS_A2F_bm,
	DS3231_Status_A1F_gc			= DS3231_STATUS_A1F_bm,
} DS3231_Status_t;


#endif // DS3231_IO_H_