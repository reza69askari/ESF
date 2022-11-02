/*
 * esf_compiler.h
 *
 * @Created: 6/27/2022 23:20:03
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

/* Notes:
 * any word started by double underline '__', is used as internal thing and user
 * should not use them as real method or variable or anything else.
 * Ex: '__value2array' is a private micro.
 * */

/* 
 * #Note:
 * State				//! config for machine report
 * Status				//! current state of machine
 * 
 * #Note:
 * ref...: reference; mean the method needs an external memory to copy result into it
 * out...: output; mean the method used an intenal memory for result (#Mind other method in that class/library may using that memory too)
 * 
 * Useful Preprocessor Macros for Debugging
 * ANSI C defines some useful preprocessor macros and variables,[3][4] also called "magic constants", include:
 * __FILE__ => The name of the current file, as a string literal
 * __LINE__ => Current line of the source file, as a numeric literal
 * __DATE__ => Current system date, as a string ("Feb 12 1996")
 * __TIME__ => Current system time, as a string ("23:59:01")
 * __TIMESTAMP__ => Date and time (non-standard)
 * __cplusplus => undefined when your C code is being compiled by a C compiler; 199711L when your C code is being compiled by a C++ compiler compliant with 1998 C++ standard.
 * __FUNCTION__ | __func__ => Current function name of the source file, as a string (part of C99)
 * __PRETTY_FUNCTION__ => "decorated" Current function name of the source file, as a string (in GCC; non-standard)
 * */

//////////////////////////////////////////////////////////////////////////
//! #Requires:
// 
//! #Options:
// #define Builder_BreakOnFirstError
// #define Builder_AvoidUninitialized
// #define Builder_WarningConversion
// #define Builder_AvoidMismatch
//////////////////////////////////////////////////////////////////////////

#ifndef ESF_COMPILER_H_
#define ESF_COMPILER_H_

#define CONF_			"is using for board hardware definitions"			/* is using for board hardware definitions */
#define Conf_			"is using for project & application definitions"	/* is using for project definitions */
#define Config_			"is using for applications & libraries definitions"	/* is using for applications & libraries definitions */

//! >>>>>>> Compiler commands

//! ref: https://gcc.gnu.org/onlinedocs/gcc/Function-Specific-Option-Pragmas.html
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wformat"
//#pragma GCC diagnostic warning "-Wformat"
//#pragma GCC diagnostic error "-Wformat"
//#pragma GCC diagnostic pop

#pragma GCC diagnostic push

/* ignore pragma region as warning */
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

/* if reached to this error, check inner sets, where arrays needs braces
 * Ex: <code>
twi_package_t package = {
	.addr = channel,  //! '.addr' is array of 3 byte (recommend to use value2array24(channel)).
	...
};
 * </code>
 * */
#pragma GCC diagnostic error "-Wmissing-braces"

//! >>>>>>> Optional commands, define options on 'board_config.h'

#ifdef Builder_BreakOnFirstError			//! Break builder when achieved to first error
#	pragma GCC diagnostic error "-Wfatal-errors"//#pragma GCC diagnostic error "-fmax-errors=4"
#endif // Builder_BreakOnFirstError

#ifndef Builder_AvoidUninitialized			//! Error if variable is not set before use
#	pragma GCC diagnostic error "-Wmaybe-uninitialized"
#endif // Builder_AvoidUninitialized

#ifdef Builder_WarningConversion			//! made warning on auto value converter
#	pragma GCC diagnostic warning "-Wconversion"
#endif // Builder_WarningConversion

#ifndef Builder_AvoidMismatch				//! Error if variable is on wrong conversion (Ex: passing pointer instead of requested int)
#	pragma GCC diagnostic error "-Wint-conversion"
#endif // Builder_AvoidMismatch

//! >>>>>>> exist files
#include <conf_board.h>
#include <utils/esf_errors.h>
#include <compiler.h>
#include <preprocessor.h>
#include <status_codes.h>

//! >>>>>>> States

#ifndef UTILS_COMPILER_H
#define PASS			0
#define FAIL			1
#define LOW				0
#define HIGH			1
#define DISABLE			0
#define ENABLE			1
#endif // !UTILS_COMPILER_H
#define OK				0
#define OFF				0
#define ON				1
#define RESET			0
#define SET				1
#define STOP			0
#define RUN				1
#define IsHandled		0 //! STATUS_OK
#define NotHandled		1
typedef enum State_enum
{
	Off					= OFF,
	On					= ON,
	Disable				= DISABLE,
	Enable				= ENABLE,
	Reset				= RESET,
	Set					= SET,
} state_t, State_t;

#define RiseFlag(flag)	flag = true

//! >>>>>>> Size & Type

#define void0			((void)0)
#define NOP_FOO()		void0
#define nop_foo()		NOP_FOO()

#define CountZeroesFromLeft(x)		clz(x)	//! 0b00101000 -> 2
#define CountZeroesFromRight(x)		ctz(x)	//! 0b00101000 -> 3

//! Same as C#: a ?? b
#define NotNull(a, b)	(a? a: b)

//! inline absolute condition (shorter code instant using normal 'abs()')
#define abs_condition(x, c)			(x == c || x == -c)
#define absC(x, c)					abs_condition(x, c)

/* to get size of an array, not size of reserved bytes on memory
 * Ex: U32 test[10] -> sizeof(test)=>40, sizeof_array(test)=>10
 * */
#define sizeof_array(array)			(sizeof((array))/sizeof((array[0])))

#define IS_SIGNED_TYPE(v)			((typeof((v)))-1 <= 0)
#define IsSigned(v)					( IS_SIGNED_TYPE(v))
#define IsUnsigned(v)				(!IS_SIGNED_TYPE(v))

//! >>>>>>> Conjunction functions

/* Ex:
 * #define LEDS						48
 * STR(LEDS)						=> "LEDS"
 * STR_NUM(LEDS)					=> "48"		'Num as string'
 */
#define STR(x)						#x
#define STR_NUM(x)					STR(x)
#define Defined_Name(x)				#x
#define Defined_Value(x)			Defined_Name(x)
#define Defined_NameValue(var)		"T " ##var "=" #var
#define Concat(p1, p2)				p1 ## p2 //! Concat parts

//! >>>>>>> events

typedef status_code_t (*event_t ) (void );
typedef status_code_t (*event_pt) (void*);
typedef bool (*test_t ) (void );
typedef bool (*test_pt) (void*);

//! >>>>>>> To override methods

/* alias of 'compiler_demux_size(...)'
void foo8(uint8_t a, void *b);
void foo16(uint16_t a, void *b);
void foo32(uint32_t a, void *b);
#define foo(x, y)    compiler_demux_size(sizeof(x), foo, x, y)
 * */
#define override_by_size			compiler_demux_size
#define override_by_type(var, func, ...)	\
	(IsUnsigned(var) ? \
	override_by_size(sizeof(var), func##U, ##__VA_ARGS__) : \
	override_by_size(sizeof(var), func##S, ##__VA_ARGS__))
#define override_by_type_def(func, var, ...)	\
	(IsUnsigned(var) ? \
	func##U(var, ##__VA_ARGS__) : \
	func##S(var, ##__VA_ARGS__))

//! >>>>>>>>>>>>>>>> Converters >>>>>>>>>>>>>>>>
#pragma region Converters

/* Decimal-value to BCD-value */
#define DEC2BCD(dec)				((((dec) / 10) << 4) + ((dec) % 10))
/* BCD-value to Decimal-value
 * 
 * to prevent extra commands, value is not checking (for invalid value). user must handle it!
 * */
#define BCD2DEC(bcd)				((((bcd) >> 4) * 10) + ((bcd) & 0x0F))

#define __value2array(n,addr)		((((addr)>>(n*8))&0xFF)),
/* 24-bit value to arrays of 3-Byte values
 * Ex: 0x123456 to { 0x56, 0x34, 0x12 }
 * */
#define value2array24(addr)			{ MREPEAT(3,__value2array,(U32)addr) }
/* any value size to arrays of values
 * Ex: 0x12345678 to { 0x78, 0x56, 0x34, 0x12 }
 * */
#define value2array(addr)			{ MREPEAT(sizeof(addr),__value2array,addr) }

#pragma endregion Converters
//! <<<<<<<<<<<<<<<< .Converters <<<<<<<<<<<<<<<<

//! >>>>>>>>>>>>>>>> Counter >>>>>>>>>>>>>>>>
#pragma region Counter

/* Counter
 * Ex:
 * <code>
CountOn(InitSteps);
int foo(void)
{
	CountIt();
	...
	CountIt();
	...
	CountIt();
	...
}
Counted(InitSteps);
 * </code>
 * */
#define CountOn(name)				enum { name ## _BASE = __COUNTER__ }; const U8 name
#define CountIt(name)				(U8)(__COUNTER__ - (name ## _BASE))
#define Counted(name)				const U8 name = (U8)(__COUNTER__ - (name ## _BASE) -1);
/* alias of Counter
 * Ex:
 * <code>
TakeSteps(InitSteps);
int foo(void)
{
	AddStep();
	...
	AddStep();
	...
	AddStep();
	...
}
TotalSteps(InitSteps);
 * </code>
 * */
#define TakeSteps(name)				CountOn(name)
/* this is invalid to read, want to get value ? use 'CountIt(name)' OR 'NextStep(name)' */
#define AddStep()					do { __COUNTER__; } while (0);
#define TotalSteps(name)			Counted(name)
/* better use of Counter (on format print 'step, total')
 * 'step' starts at 1
 * Ex:
 * <code>
TakeSteps(InitSteps);
int foo(void)
{
	printf(StepsPRT " Initializing system clock...", NextStep(InitSteps));
	...
	printf(StepsPRT " Loading configurations...", NextStep(InitSteps));
	...
	printf(StepsPRT " Applying...", NextStep(InitSteps));
	...
}
TotalSteps(InitSteps);
 * </code>
 * */
#define NextStep(name)				CountIt(name), name
#define StepsPRT					"%d of %d"
#define NextStepP					StepsPRT

#pragma endregion Counter
//! <<<<<<<<<<<<<<<< .Counter <<<<<<<<<<<<<<<<

//! >>>>>>>>>>>>>>>> Attributes >>>>>>>>>>>>>>>>
#pragma region Attributes

#define OptionalMethod(func)		extern func		 __attribute__((weak)) 

#define aliasof(baseName)			__attribute__((alias(baseName)))
#define AliasOf(baseName)			aliasof(STR(baseName))

#define __unused					__attribute__((unused))

#define __noinline					__attribute__ ((noinline))
#define NoInline					__noinline

#pragma endregion Attributes
//! <<<<<<<<<<<<<<<< .Attributes <<<<<<<<<<<<<<<<

//! >>>>>>> esf files
#include <asf.h>
#include <esf_deprecated.h>
#include <utils/esf_mask_position.h>
#include <utils/esf_stdlib.h>
#include <MPUs/esf_io.h>
#include <utils/esf_log.h>

#if defined(_ASSERT_ENABLE_)
#	if defined(TEST_SUITE_DEFINE_ASSERT_MACRO)
// Assert() is defined in unit_test/suite.h
#		include "unit_test/suite.h"
#	else
#		ifdef Assert
#			undef Assert
#		endif
#		define Assert(expr)		if (!(expr)) { LogCritical("#AppFreezed"); while (true); }
#	endif
#else
#	ifdef Assert
#		undef Assert
#	endif
#	define Assert(expr) ((void) 0)
#endif


#endif // ESF_COMPILER_H_