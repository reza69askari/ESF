/*
 * esf_print_type.h
 * Copyright: this file is created base on inttypes.h
 *
 * @Created: 6/28/2022 15:47:18
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_PRINT_TYPE_H_
#define ESF_PRINT_TYPE_H_

//! > Optional implemention
#include <inttypes.h>

/* 
 * char					int8_t
 * signed char			int8_t
 * unsigned char		uint8_t
 * int					int16_t
 * signed int			int16_t
 * unsigned int			uint16_t
 * short				int16_t
 * signed short			int16_t
 * unsigned short		uint16_t
 * long					int32_t
 * signed long			int32_t
 * unsigned long		uint32_t
 * long long			int64_t
 * signed long long		int64_t
 * unsigned long long	uint64_t
 * 
 * double is 32bit (4Byte)
 * float  is 32bit (4Byte)
 *
 * print %X	uint16_t
 * 
 * sprintf:	http://en.cppreference.com/w/cpp/io/c/fprintf
 * see also: <inttypes.h>
 * Conversion 	Explanation			Argument type
 * specifier						(none)			nickname	h				l				L
 * c			single character	int				int16_t		-				wint_t			-
 * s			character string	char*			int8_t*		-				wchar_t*		-
 * d/i			signed integer		int				int16_t		short			long			-
 * o(octal)	unsigned integer	unsigned int	uint16_t	unsigned short	unsigned long	-
 * x/X			"					"				uint16_t	"				"				"
 * u			"					"				uint16_t	"				"				"
 * f/F			floating-point number	double		-			-				-				long double
 * e/E			"					"				-			"				"				"
 * g/G			"					"				-			"				"				"
 * n	number of characters written	int*		int16_t*	short*			long*			-
 * p			pointer				void*			-			-				-				-
 * Ex (print uint32_t): %lu -> Long-Unsign
 * Ex (print uint16_t): %u
 * Ex (print with zero): %02X
 * for double print https://startingelectronics.org/articles/atmel-AVR-8-bit/print-float-atmel-studio-7/
 * A) AVR/GNU Linker > General > Use vprintf library(-Wl,-u,vfprintf)
 * B) AVR/GNU Linker > Miscellaneous > -lprintf_flt
 * */

#define  S8P			 "%d"
#define S16P			 "%d"
#define S32P			"%ld"
#define  U8P			 "%u"
#define U16P			 "%u"
#define U32P			"%lu"

#define  U8Po			 "%o"
#define U16Po			 "%o"
#define U32Po			"%lo"

#define  U8Px			 "%x"
#define U16Px			 "%x"
#define U32Px			"%lx"
#define  U8PX			 "%X"
#define U16PX			 "%X"
#define U32PX			"%lX"

#define  U8Px0			 "%02x"
#define U16Px0			 "%04x"
#define U32Px0			"%08lx"
#define  U8PX0			 "%02X"
#define U16PX0			 "%04X"
#define U32PX0			"%08lX"

#define  U8P0x0			 "0x%02x"
#define U16P0x0			 "0x%04x"
#define U32P0x0			"0x%08lx"
#define  U8P0X0			 "0x%02X"
#define U16P0X0			 "0x%04X"
#define U32P0X0			"0x%08lX"

/* signed integer is mean less on print X, but defining for common usage (for example on Logs). */
#define  S8Px			U8Px
#define S16Px			U16Px
#define S32Px			U32Px
#define  S8PX			U8PX
#define S16PX			U16PX
#define S32PX			U32PX

#define  S8Px0			U8Px0
#define S16Px0			U16Px0
#define S32Px0			U32Px0
#define  S8PX0			U8PX0
#define S16PX0			U16PX0
#define S32PX0			U32PX0

#define  S8P0x0			U8P0x0
#define S16P0x0			U16P0x0
#define S32P0x0			U32P0x0
#define  S8P0X0			U8P0X0
#define S16P0X0			U16P0X0
#define S32P0X0			U32P0X0

/* ref to: https://stackoverflow.com/questions/25860850/what-is-the-difference-between-f-and-lf-in-c
 * For printf, arguments of type float are promoted to double so both %f and %lf are used for double. For scanf, you should use %f for float and %lf for double.
 * */
/* 
 * "%0.2lf", 0.123 -> 0.12 (zero padded min. width of 0, 2 decimal places).
 * "%6.2lf", 0.123 -> __0.12 (space padded min. width of 6, 2 decimal places).
 * "%06.2lf", 0.123 -> 000.12 (zero padded min. width of 6, 2 decimal places).
 * "%0.6lf", 0.123 -> 0.123000
 * */
#define floatP			"%f"
#define doubleP			"%lf"


#endif // ESF_PRINT_TYPE_H_