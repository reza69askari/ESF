/*
 * esf_tick.h
 *
 * @Created: 6/30/2022 13:03:54
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */
//////////////////////////////////////////////////////////////////////////
//! #Requires:
// #define Conf_SystemTick_Source			TC_CLKSEL_EVCH7_gc
// #define Conf_SystemTick_Source_Init()	do { sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS); EVSYS_CH7MUX = EVSYS_CHMUX_PRESCALER_32768_gc; } while (0);
// #define Conf_SystemTick_PreScale			32768
// **OR**
// #define Conf_SystemTick_Base				50 //! in millisecond, if not define this, you should call 'SystemTick_IRQ' or 'Tick' by your own.
//! #Options:
// 
//////////////////////////////////////////////////////////////////////////

#ifndef ESF_TICK_H_
#define ESF_TICK_H_

#include <conf_board.h>
#include <esf_compiler.h>

typedef U16 Tick_t;

typedef enum Tick_Divition_enum
{
	Tick_MUX1_gc		= DIV1_gc		,
	Tick_MUX2_gc		= DIV2_gc		,
	Tick_MUX4_gc		= DIV4_gc		,
	Tick_MUX8_gc		= DIV8_gc		,
	Tick_MUX16_gc		= DIV16_gc		,
	Tick_MUX32_gc		= DIV32_gc		,
	Tick_MUX64_gc		= DIV64_gc		,
	Tick_MUX128_gc		= DIV128_gc		,
	Tick_MUX256_gc		= DIV256_gc		,
	Tick_MUX512_gc		= DIV512_gc		,
	Tick_MUX1024_gc		= DIV1024_gc	,
	Tick_MUX2048_gc		= DIV2048_gc	,
	Tick_MUX4096_gc		= DIV4096_gc	,
	Tick_MUX8192_gc		= DIV8192_gc	,
	Tick_MUX16384_gc	= DIV16384_gc	,
	Tick_MUX32768_gc	= DIV32768_gc	,
} TickDiv_t;

//! >>>>>>>>>>>>>>>> Checker >>>>>>>>>>>>>>>>
#pragma region Checker

//! >>>>>>> Requires-Check

#ifdef Conf_SystemTick_Source
#	define _TickmSbase		(1000UL / ((F_CPU) / (Conf_SystemTick_PreScale)))
#elif Conf_SystemTick_Base
#	error NotImplementedException
#endif // Conf_SystemTick_Source
#if (_TickmSbase == 0)
#	error ArgumentException
#endif

//! >>>>>>> Options-Check

#pragma endregion Checker
//! <<<<<<<<<<<<<<<< .Checker <<<<<<<<<<<<<<<<

static inline void SystemTick_Init(void)
{
	LOG_FUNCTION();
	tc_enable(SystemTick_TC);
	#ifdef Conf_SystemTick_Source
	Conf_SystemTick_Source_Init();
	tc_write_clock_source(SystemTick_TC, Conf_SystemTick_Source);
	#elif (defined Conf_SystemTick_Base)
	//! base on 125mS
	tc_set_periode_and_source(SystemTick_TC, (1000UL/Conf_SystemTick_Base));
	#else
	#	error NotImplementedException
	#endif // Conf_SystemTick_Source
	//! Operate on: Normal, 16-bit
	tc_set_wgm(SystemTick_TC, TC_WG_NORMAL);
	
	//! Log
	LogParameter((F_CPU / Conf_SystemTick_PreScale));
	LogParameter(_TickmSbase);
	LogParameter((1000.0/(F_CPU/Conf_SystemTick_PreScale))*(2<< 0));
	LogParameter((1000.0/(F_CPU/Conf_SystemTick_PreScale))*(2<<15));
	
}

static inline Tick_t SystemTick_Test (void)
{
	static Tick_t _Match = 0, _Flags, _Count;
	_Flags = 0;
	if (tc_is_overflow(SystemTick_TC))
	{	//! overflow occurred
		tc_clear_overflow(SystemTick_TC);
		while (_Match < UINT16_MAX)
		{
			_Flags |= (UINT16_MAX ^ _Match) & UINT16_MAX;
			_Match++;
			if (_Match == UINT16_MAX) { _Match = 0; break; }
		}
		_Match = 0;
	}
	_Count = tc_read_count(SystemTick_TC);
	while (_Match < _Count)
	{
		_Flags |= (_Count ^ _Match) & _Count;
		_Match++;
		if (_Match == UINT16_MAX) { _Match = 0; break; }
	}
	return _Flags;
}

//bool SystemTick_Test_uS (Tick_t tick, U16 uS)

static inline bool SystemTick_Test_mS (Tick_t tick, U16 mS)
{
	if (tick == 0) { return false; }
	Tick_t _look = 1;
	mS >>= 1;
	while((mS / _TickmSbase) >= 1) {
		_look <<= 1;
		mS >>= 1; //! to prevent massive memory usage
		if (tick < _look) { return false; } //! prevent calculate if there is no flags left
	}
	return (tick & _look) != 0;
}


#endif // ESF_TICK_H_