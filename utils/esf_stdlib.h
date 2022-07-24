/*
 * esf_stdlib.h
 *
 * @Created: 7/21/2022 16:15:57
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_STDLIB_H_
#define ESF_STDLIB_H_

#include <stdlib.h>
#include <compiler.h>
#include <esf_compiler.h>
#include "esf_string.h"


//! > InternalUsage
#pragma region InternalUsage

static string* ___commem_d_ (string str, bool dotAsSeparate, int* outBase)
{
	static string po; //! Base pointer for all parse strings, so return is address to this.
	if (str) { po = str; }	//! update pointer is str is not NULL
	if (outBase == NULL)
	{
		po = StrPointOfAny(po, "0+-.123456789");
		return &po;
	}
	string _po2 = StrPointOfAny(po, "123456789");
	string _po3 = strstr(po, "0x");
	string _po4 = strstr(po, "0b");
	*outBase = 16;
	//! if string was not started by 0x.., try to find 0b..
	if (!_po3 || (_po4 && _po4 < _po3)) { _po3 = _po4; *outBase = 2; }
	if ((_po3 && !_po2) || (_po3 && _po2 > _po3))
	{	//! if 0x.. or 0b.. are founded
		po = _po3 + 2;
		return &po;
	}
	*outBase = 10;
	_po3 = StrPointOf_Char(po, '0');
	if (!_po2 || (_po3 && _po3 < _po2))
	{	//! first number founded is "0", Ex: PER@0*1*123#
		po = _po3;
		return &po;
	}
	po = _po2;
	if (po && StrStartedAny(po -1, (dotAsSeparate? "+-": "+-.")))
	{ (po)--; }	//! Get sing or period before
	return &po;
}

static void ___skipNum__ (string* po, bool dotAsSeparator)
{
	if (!po || !(*po)) { return; }
	if ((**po) == '-' || (**po) == '+') { (*po)++; }
	while (StrStartedAny((*po), "1234567890ABCDEF.")) {	//! Skip current num in str
		if ((*po)++ && (**po) == '.' && dotAsSeparator) { break; }
	}
}

#pragma endregion InternalUsage
//! .InternalUsage <

/*
 * Ex: str = ALM123.456*789
 * parse_S16(str)  => 123
 * parse_S16(NULL) => 456
 * parse_S16(NULL) => 789
 */
/*
 * String.Get Long (Hex and binary are valid)
 * to continues in a string put that string on first call and NULL in next call
 * Ex: str = "+ABC325"
 * parse_long(str)  => 325
 * Ex: str = "+ABC.825+456"
 * parse_long(str)  => 0
 * parse_long(NULL) => 456
 *
 * Ex: str = "12"			=> 12
 * Ex: str = "0b00001100"	=> 12
 * Ex: str = "0x0C"			=> 12
 * */
static int32_t parse_S32 (string str)
{
	int base;
	string *po = ___commem_d_(str, false, &base);
	if (!(*po)) { return 0; }
	int32_t out_num = strtol(*po, NULL, base);
	___skipNum__(po, false);
	return out_num;
}
__always_inline static int16_t parse_S16 (string str) { return (int16_t)parse_S32(str); }
__always_inline static int8_t parse_S8 (string str) { return (int8_t)parse_S32(str); }

/*
 * String.Get Unsigned-Long (Hex and binary are valid)
 * same as parse_S32()
 */
static uint32_t _parse_U32 (string str, bool dotAsSeparator)
{
	int base;
	string *po = ___commem_d_(str, dotAsSeparator, &base);
	if (!(*po)) { return 0; }
	uint32_t out_num = strtoul(*po, NULL, base);
	___skipNum__(po, dotAsSeparator);
	return out_num;
}
__always_inline static uint16_t parse_U32 (string str) { return (uint16_t)_parse_U32(str, false); }
__always_inline static uint16_t parse_U16 (string str) { return (uint16_t)parse_U32(str); }
__always_inline static uint8_t parse_U8 (string str) { return (uint8_t)parse_U32(str); }

/*
 * String.Get double
 * to continues in a string put that string on first call and NULL in next call
 * Ex: str = "+ABC.825+456"
 * parse_double(str) => 0.825
 * parse_double(NULL) => 456
 */
__unused static double parse_double (string str)
{
	string* po = ___commem_d_(str, false, NULL);
	if (!(*po)) { return 0; }
	double out_num = atof(*po);
	___skipNum__(po, false);
	return out_num;
}

__always_inline static uint16_t parse_num (string str) { return (uint16_t)_parse_U32(str, true); }
static uint8_t* parse_nums (string _str, uint8_t *refBuf, size_t len)
{
	if (!_str || !refBuf || len == 0) { return NULL; }
	while (len--) {
		*(refBuf++) = (uint8_t)parse_num(_str);
		_str = NULL;
	}
	return refBuf;
}

/*
 * \p refBuf:	a buffer with capacity of 4 elements
 */
static uint8_t* parse_ip4_i (string _str, uint8_t *refBuf)
{
	if (!_str || !refBuf) { return NULL; }
	return parse_nums(_str, refBuf, 4);
}
/*
 * \p refBuf:	a buffer with capacity of 4 elements
 */
__unused static uint8_t *parse_ip4 (string _str, uint8_t *refBuf)
{
	if (!_str || !refBuf) { return NULL; }
	parse_ip4_i(_str, refBuf);
	convert_32_bit_to_byte_array(Swap32(convert_byte_array_to_32_bit(refBuf)), refBuf);
	return refBuf;
}

/* aaRRGGBB
 * Ex: FFA05D69
 **/
__unused static uint32_t parse_color (string str)
{
	if (str && str[0] == '#') { str++; }
	return strtoul(str, NULL, 16);
}


#endif // ESF_STDLIB_H_