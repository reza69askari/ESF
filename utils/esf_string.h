/*
 * esf_string.h
 *
 * Created: 10/6/2016 7:19:34 PM
 *  Author: MohammadReza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_STRING_H_
#define ESF_STRING_H_

#include <string.h>
#include <compiler.h>

typedef char* string;

/* get pointer of '_char' in 'str'. */
#define StrPointOf_Char(str, _char)				strchr(str, _char)
/* get pointer of 'txt' in 'str'. */
#define StrPointOf(str, txt, ignoreCase)		((ignoreCase) ? strcasestr(str, txt) : strstr(str, txt))
/* get pointer of any of 'characters' in 'str' */
#define StrPointOfAny(str, characters)			strpbrk(str, characters)

/* test if 'str' has text of 'txt' on it. */
#define StrContain(str, txt, ignoreCase)		((StrPointOf(str, txt, ignoreCase)) != NULL)
/* test if 'str' is started with 'txt'. */
#define StrStartWith(str, txt, ignoreCase)		((ignoreCase) ? (strncasecmp(str, txt, strlen(txt)) == 0) : (strncmp(str, txt, strlen(txt)) == 0))
/* test if 'str' is started by any char of 'looking' ? */
#define StrStartedAny(str, characters)			(strchr(characters, ((str)[0])) != NULL)//(StrPointOfAny(str, characters) == str)

/* Number of characters while str is not equal to any of look */
#define StrlenUntil(str, look)					strcspn(str, look)
/* Number of characters while str is equal to any of look */
#define StrlenWhile(str, look)					strspn(str, look)
/* Take string while it's characters not equal to any of look */
#define StrTakeUntil(str, src, look)			strncpy(str, src, strcspn(src, look))
/* Take string while it's characters equal to any of look */
#define StrTakeWhile(str, src, look)			strncpy(str, src, strspn(src, look))

/*
 * return index of 'value' from begin of 'str'
 * if 'value' is not found, return 0
 */
static inline size_t StrIndexOf_Char (const string str, const char value)
{
	char* po = strchr(str, value);
	return (po) ? (po - str) : 0;
}

/**
 * return pointer after 'value' in 'str',
 * if 'value' not found in 'str', returning NULL.
 */
static inline char* StrPointAfter_(const string str, const string value, bool ignoreCase)
{
	string _p = StrPointOf(str, value, ignoreCase);
	return (_p) ? (_p + strlen(value)) : NULL;
}
#define StrPointAfter(str, value, ignoreCase)	StrPointAfter_(str, (const string)value, ignoreCase)

/**
 * Skip char of \p value in string of \p str as long as \p char_SkipNum
 * this method return NULL if \p value is not found in \p str
 * 
 * Ex: "+CLCC: 1,1,4,0,0,\"+xxxxxxxxxxx\",145,\"\"", '"', 0
 *	return point to begin of "+xxxxxxxxxxx\",145,\"\""
 * Ex: "+CLCC: 1,1,4,0,0,\"+xxxxxxxxxxx\",145,\"\"", '"', 1
 *	return point to begin of ",145,\"\""
 */
static inline char* StrSkipChar (string str, char value, uint8_t char_SkipNum)
{
	char* po = str;
	do
	{
		char* _po2 = StrPointOf_Char(po, value);
		if (_po2) { po = _po2 + 1; }
		else { po = str; break; }
	} while (char_SkipNum--);
	return (po == str ? NULL : po);
}

/**
 * copy range between two of \p value in \p str to \p refStr
 * use \p char_SkipNum to skip(ignore) how much do you need of \p value
 * return point of \pvalue in \p str if Suggest, else NULL
 * 
 * Ex: "+CLCC: 1,1,4,0,0,\"+xxxxxxxxxxx\",145,\"\"", buf, '"', 0
 *	buf = "+xxxxxxxxxxx"
 */
static inline char* StrSubstring_Char_r (string str, string refStr, char value, uint8_t char_SkipNum)
{
	char* po = StrSkipChar(str, value, char_SkipNum);
	if (po && refStr)
	{
		size_t _s = StrIndexOf_Char(po, value);
		strncpy(refStr, po, _s);
		return po;
	}
	return po;
}

/**
 * split string of \p str by char array of \p split,
 * after first use, send NULL for parameter 1 to get rest parts.
 * 
 * Ex: str = "+CLCC: 1,,\"+xxxxxxxxxxx\",145,\"\"", split = ",\""
 *	StrSplit(str, split) => return "+CLCC: 1"
 *	StrSplit(NULL, split) => return ""
 *	StrSplit(NULL, split) => return ""
 *	StrSplit(NULL, split) => return "+xxxxxxxxxxx"
 *	StrSplit(NULL, split) => return ""
 *	StrSplit(NULL, split) => return "145"
 *	StrSplit(NULL, split) => return ""
 *	StrSplit(NULL, split) => return ""
 *	StrSplit(NULL, split) => return NULL
 * */
static inline char* StrSplit(string str, const string split)
{
	static char* po;
	if (str) { po = str; }
	return strsep(&po, split);
}

/**
 * split string of \p str by char array of \p split,
 * after first use, send NULL for parameter 1 to get rest parts.
 * 
 * Ex: str = "+CLCC: 1,,\"+xxxxxxxxxxx\",145,\"\"", split = ",\""
 *	StrSplitText(str, split) => return "+CLCC: 1"
 *	StrSplitText(NULL, split) => return "+xxxxxxxxxxx"
 *	StrSplitText(NULL, split) => return "145"
 *	StrSplitText(NULL, split) => return NULL
 * */
#define StrSplitText(str, split)	strtok(str, split)


#endif /* ESF_STRING_H_ */