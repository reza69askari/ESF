/*
 * esf_string_progmem.h
 *
 * @Created: 7/1/2022 8:30:22
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_STRING_PROGMEM_H_
#define ESF_STRING_PROGMEM_H_

#include <progmem.h>

#define PGMP						PSTR("%S")

/* String into program-memory area
 * Ex: string_P(txtGuide0, "Welcome to help center");
 */
#define string_P(var, str)			static const char var[] PROGMEM = str

/* Same as 'string_P(...)' include parameters name taker
 * Ex: string_PS(txtGuide0, SetupPage_gc); //txtGuide0 = "SetupPage_gc"
 */
#define string_PS(micro)			string_P(txt ## micro, STR(micro))

/* Array of strings into program-memory area
 * collect strings into one area, and control them by index
 * Ex: string_PArray(txtGuides) = { txtGuide0, txtGuide1, }; print_P(txtGuides[1]);
 */
#define string_PArray(var)			static const char *const var[] PROGMEM

/* to get parameter from program memory (_ as function parameter _)
 * Ex: printf("Welcome %S", string_PReadMemory(&users[2]))
 */
#define string_PReadMemory(mem)		(wchar_t *)pgm_read_word(&mem)
#define string_PReadParam(mem)		string_PReadMemory(mem)
#define string_PRP(mem)				string_PReadParam(mem)

/* to get string from program memory and pass it to function (** as function format **)
 * Ex: printf(string_PReadString(&GuidMessages[2]))
 */
#define string_PReadString(mem)		(PGM_P)pgm_read_word(mem)
#define string_PReadFormat(mem)		string_PReadString(mem)
#define string_PRF(mem)				string_PReadFormat(mem)
#define string_PReadChar(mem)		pgm_read_byte(mem)


#endif // ESF_STRING_PROGMEM_H_