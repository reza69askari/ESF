/*
 * AD7190_io.h
 *
 * @Created: 5/6/2022 17:59:04
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

//////////////////////////////////////////////////////////////////////////
//                              Ex-ADC                                  //
//                    4.8 kHz Ultra-low Noise 24-Bit                    //
//                      Sigma-Delta ADC with PGA                        //
//////////////////////////////////////////////////////////////////////////

#ifndef AD7190_IO_H_
#define AD7190_IO_H_

/* AD7190-Registers */
typedef enum AD7190_Registers_enum
{	//! 0 R/W^ REG CRead 00 
	AD7190_REG_COMM_gc				= 0x00, /* 8 bits 000: Communications register during a write operation 8 bits */
////	AD7190_REG_STATUS_gc		= 0x00,	/* 8 bits 000: Status register during a read operation 8 bits */
	//AD7190_REG_STATUS_DRDY_gc		= 0x04,	/* 8 bits 000: Status register during a read operation 8 bits */
	AD7190_REG_MODE_gc				= 0x08,	/* 24bits 001: Mode register 24 bits */
	AD7190_REG_CONFIG_gc			= 0x10,	/* 24bits 010: Configuration register 24 bits */
	AD7190_REG_DATA_gc				= 0x18,	/* 24/32b 011: Data register/data register plus status information 24 bits/32 bits */
	AD7190_REG_ID_gc				= 0x20,	/* 8 bits 100: ID register 8 bits */
	AD7190_REG_GPOCON_gc			= 0x28,	/* 8 bits 101: GPOCON register 8 bits */
	AD7190_REG_OFFSET_gc			= 0x30,	/* 24bits 110: Offset register 24 bits */
	AD7190_REG_FULLSCALE_gc			= 0x38,	/* 24bits 111: Full-scale register 24 bits */
	ContinuousRead_On_cmd			= 0x5C, /* 0 Read DataRegister(011) ContinuousRead-ON  00 */
	ContinuousRead_Off_cmd			= 0x58, /* 0 Read DataRegister(011) ContinuousRead-OFF 00 */
} AD7190_Registers_t;
#define AD7190_Registers_Read_bm	(1 << 6)

/* >>>>>>> AD7190-Mode-Register <<<<<<< */

#define AD7190_MODE_OP_gm			(7UL << 21)
#define AD7190_MODE_OP_gp			21
#define AD7190_MODE_DAT_STA_bm		(1UL << 20)
#define AD7190_MODE_DAT_STA_bp		20
#define AD7190_MODE_CLKx_gm			(3UL << 18)
#define AD7190_MODE_CLKx_gp			18
#define AD7190_MODE_SINC3_bm		(1UL << 15)
#define AD7190_MODE_SINC3_bp		15
#define AD7190_MODE_ENPAR_bm		(1UL << 13)
#define AD7190_MODE_ENPAR_bp		13
#define AD7190_MODE_SINGLE_bm		(1UL << 11)
#define AD7190_MODE_SINGLE_bp		11
#define AD7190_MODE_REJ60_bm		(1UL << 10)
#define AD7190_MODE_REJ60_bp		10
#define AD7190_MODE_PER_gm			(0x0003FF << 0)
#define AD7190_MODE_PER_gp			0
#define AD7190_MODE_PER_min			1
#define AD7190_MODE_PER_max			1023UL

typedef enum AD7190_Operation_Mode_enum
{
	AD7190_OPS_ContinuousConversion_gc = (0UL << AD7190_MODE_OP_gp),
	AD7190_OPS_SingleConversion_gc	= (1UL << AD7190_MODE_OP_gp),
	AD7190_OPS_Idle_gc				= (2UL << AD7190_MODE_OP_gp),
	AD7190_OPS_PowerDown_gc			= (3UL << AD7190_MODE_OP_gp),
} AD7190_Operation_t;
typedef enum AD7190_Operation_Mode_Part2_enum
{
	AD7190_CAL_InternalZero_gc		= (4UL << AD7190_MODE_OP_gp), /* RDY goes high when the calibration is initiated and returns low when the calibration is complete. */
	AD7190_CAL_InternalFull_gc		= (5UL << AD7190_MODE_OP_gp), /* RDY goes high when the calibration is initiated and returns low when the calibration is complete. */
	AD7190_CAL_SystemZero_gc		= (6UL << AD7190_MODE_OP_gp), /* RDY goes high when the calibration is initiated and returns low when the calibration is complete. */
	AD7190_CAL_SystemFull_gc		= (7UL << AD7190_MODE_OP_gp), /* RDY goes high when the calibration is initiated and returns low when the calibration is complete. */
} AD7190_Calibration_t;

//typedef enum AD7190_ModeExtera_enum
//{
//	IncludeStatusRegister_gc			= (1 << AD7190_MODE_DAT_STA_bp),
//	AvoidIncludeStatusRegister_gc		= (0 << AD7190_MODE_DAT_STA_bp),
//} ????_t;

#define AD7190_CLKSEL_ExtXTAL		(0UL << AD7190_MODE_CLKx_gp) /* External crystal. The external crystal is connected from MCLK1 to MCLK2. */
#define AD7190_CLKSEL_ExtClock		(1UL << AD7190_MODE_CLKx_gp) /* External clock. The external clock is applied to the MCLK2 pin.		    */
#define AD7190_CLKSEL_Int4920Hz		(2UL << AD7190_MODE_CLKx_gp) /* Internal 4.92 MHz clock. Pin MCLK2 is tristated.						    */
#define AD7190_CLKSEL_Int4920Hz_SQW	(3UL << AD7190_MODE_CLKx_gp) /* Internal 4.92 MHz clock. The internal clock is available on MCLK2.	    */

typedef enum AD7190_Mode_Filters_enum
{
	/* SINC4 filter (Settling time = 4/Fadc); it's good at higher Output-Data-Rate (FS value less than 5)
	 * due to its deeper notches, SINC4 filter gives better 50 Hz/60 Hz rejection */
	AD7190_Filter_SINC4_gc			= (0 << AD7190_MODE_SINC3_bp),
	/* SINC3 filter is faster than SINC4-filter (Settling time = 3/Fadc) */
	AD7190_Filter_SINC3_gc			= (1 << AD7190_MODE_SINC3_bp),
} AD7190_Filters_t;

/* Mode to flags: */
typedef enum AD7190_ModeFlags_enum
{
	AD7190_UseSINC3_gc				= AD7190_MODE_SINC3_bm,
	AD7190_UseENPAR_gc				= AD7190_MODE_ENPAR_bm,
	//AD7190_UseSINGLE_gc				= AD7190_MODE_SINGLE_bm,
	AD7190_UseREJ60_gc				= AD7190_MODE_REJ60_bm,
} AD7190_Options_t;
#define AD7190_Options_gm			(AD7190_MODE_SINC3_bm | AD7190_MODE_ENPAR_bm | AD7190_MODE_REJ60_bm)//AD7190_MODE_SINGLE_bm

/* >>>>>>> AD7190-Config-Register <<<<<<< */

#define AD7190_CONFIG_CHOP_bm		(1UL << 23)
#define AD7190_CONFIG_CHOP_bp		23
#define AD7190_CONFIG_REFSEL_bm		(1UL << 20)
#define AD7190_CONFIG_REFSEL_bp		20
#define AD7190_CONFIG_CHx_gm		(0x0000FF << 8)
#define AD7190_CONFIG_CHx_gp		8
#define AD7190_CONFIG_BURN_bm		(1UL << 7)
#define AD7190_CONFIG_BURN_bp		7
#define AD7190_CONFIG_REFDET_bm		(1UL << 6)
#define AD7190_CONFIG_REFDET_bp		6
#define AD7190_CONFIG_BUF_bm		(1UL << 4)
#define AD7190_CONFIG_BUF_bp		4
#define AD7190_CONFIG_POL_bm		(1UL << 3)	/* Polarity */
#define AD7190_CONFIG_POL_bp		3			/* Polarity */
#define AD7190_CONFIG_GAIN_gm		(0x000007 << 0)
#define AD7190_CONFIG_GAIN_gp		0

typedef enum AD7190_Config_RefrenceSelection_enum
{
	AD7190_REF_IN1_gc				= (0 << AD7190_CONFIG_REFDET_bp), /* 0: External reference applied between REFIN1(+) and REFIN1(?). */
	AD7190_REF_IN2_gc				= (1 << AD7190_CONFIG_REFDET_bp), /* 1: External reference applied between the P1/REFIN2(+) and P0/REFIN2(-) pins. */
} AD7190_Refrence_t;

typedef enum AD7190_Config_Channels_enum
{
	AD7190_AIN12_gc					= (0b00000001 << AD7190_CONFIG_CHx_gp), /* +:AIN1 & -:AIN2 */
	AD7190_AIN34_gc					= (0b00000010 << AD7190_CONFIG_CHx_gp), /* +:AIN3 & -:AIN4 */
	AD7190_TempS_gc					= (0b00000100 << AD7190_CONFIG_CHx_gp), /* Internal Temperature Sensor */
	AD7190_AIN22_gc					= (0b00001000 << AD7190_CONFIG_CHx_gp), /* +:AIN2 & -:AIN2 */
	AD7190_AIN1_gc					= (0b00010000 << AD7190_CONFIG_CHx_gp), /* +:AIN1 & -:AINCOM */
	AD7190_AIN2_gc					= (0b00100000 << AD7190_CONFIG_CHx_gp), /* +:AIN2 & -:AINCOM */
	AD7190_AIN3_gc					= (0b01000000 << AD7190_CONFIG_CHx_gp), /* +:AIN3 & -:AINCOM */
	AD7190_AIN4_gc					= (0b10000000 << AD7190_CONFIG_CHx_gp), /* +:AIN4 & -:AINCOM */
} AD7190_Channels_t;

typedef enum AD7190_Config_Polarity_enum
{
	AD7190_Bipolar_gc				= (0 << AD7190_CONFIG_POL_bp),
	AD7190_Unipolar_gc				= (1 << AD7190_CONFIG_POL_bp),
} AD7190_Polarity_t;

typedef enum AD7190_Config_Gains_enum
{
	AD7190_Gain1_gc					= (0b000 << AD7190_CONFIG_GAIN_gp), /* Gain:   1, Input-range: ±5.0 V */
	AD7190_Gain8_gc					= (0b011 << AD7190_CONFIG_GAIN_gp), /* Gain:   8, Input-range: ±625 mV */
	AD7190_Gain16_gc				= (0b100 << AD7190_CONFIG_GAIN_gp), /* Gain:  16, Input-range: ±312.5 mV */
	AD7190_Gain32_gc				= (0b101 << AD7190_CONFIG_GAIN_gp), /* Gain:  32, Input-range: ±156.2 mV */
	AD7190_Gain64_gc				= (0b110 << AD7190_CONFIG_GAIN_gp), /* Gain:  64, Input-range: ±78.125 mV */
	AD7190_Gain128_gc				= (0b111 << AD7190_CONFIG_GAIN_gp), /* Gain: 128, Input-range: ±39.06 mV */
} AD7190_Gain_t;


#endif // AD7190_IO_H_