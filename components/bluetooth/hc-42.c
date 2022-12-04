/*
 * hc-42.c
 *
 * @Created: 10/13/2022 10:44:05
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */


//! >>>>>>> HC-42-Library
#include <conf_board.h>
#include <esf_compiler.h>
#include "hc-42.h"
#include <utils/esf_wait.h>

//! >>>>>>>>>>>>>>>> Checker >>>>>>>>>>>>>>>>
#pragma region Checker

//! >>>>>>> Requires-Check
#ifndef HC42_LPIN_pin
#	error 'HC42_LPIN_pin' is required
#endif // HC42_LPIN_pin
#pragma endregion Checker
//! <<<<<<<<<<<<<<<< .Checker <<<<<<<<<<<<<<<<

#define CNF_HC42_WARM_UP			350 //! 350mS

//! >>>>>>> HC42_CMDs_enum
string_P(hc42__cmd_CHECK			, "AT");
string_P(hc42__cmd_VERSION			, "AT+VERSION");
string_P(hc42__cmd_NAME				, "AT+NAME=%s");
string_P(hc42__cmd_NAME_P			, "AT+NAME=%S");
string_P(hc42__cmd_RFPM				, "AT+RFPM="S8P);
string_P(hc42__cmd_UART				, "AT+UART="U32P);
string_P(hc42__cmd_AINT				, "AT+AINT="U16P);
string_P(hc42__cmd_PM				, "AT+PM="U8P);
string_P(hc42__cmd_SLEEP			, "AT+SLEEP");
string_P(hc42__cmd_PD				, "AT+PD");
string_P(hc42__cmd_LED1M			, "AT+LED1M="U8P);
string_P(hc42__cmd_LED2M			, "AT+LED2M="U8P);
string_P(hc42__cmd_DEFAULT			, "AT+DEFAULT");
string_P(hc42__cmd_ROLE				, "AT+ROLE=%c");
string_P(hc42__cmd_RESET			, "AT+RESET");
string_P(hc42__cmd_IBEN				, "AT+IBEN="U8P);
string_P(hc42__cmd_IBUUID			, "AT+IBUUID=%s");
string_P(hc42__cmd_IBUUID_P			, "AT+IBUUID=%S");
string_P(hc42__cmd_IBRSSI			, "AT+IBRSSI="U8PX);
string_P(hc42__cmd_IBMAJ			, "AT+IBMAJ="U16PX);
string_P(hc42__cmd_IBMIN			, "AT+IBMIN="U16PX);
string_P(hc42__cmd_ADDR				, "AT+ADDR");
string_PArray(hc42_cmds_txt) = {
	hc42__cmd_CHECK		,
	hc42__cmd_VERSION	,
	hc42__cmd_NAME		,
	hc42__cmd_NAME_P	,
	hc42__cmd_RFPM		,
	hc42__cmd_UART		,
	hc42__cmd_AINT		,
	hc42__cmd_PM		,
	hc42__cmd_SLEEP		,
	hc42__cmd_PD		,
	hc42__cmd_LED1M		,
	hc42__cmd_LED2M		,
	hc42__cmd_DEFAULT	,
	hc42__cmd_ROLE		,
	hc42__cmd_RESET		,
	hc42__cmd_IBEN		,
	hc42__cmd_IBUUID	,
	hc42__cmd_IBUUID_P	,
	hc42__cmd_IBRSSI	,
	hc42__cmd_IBMAJ		,
	hc42__cmd_IBMIN		,
	hc42__cmd_ADDR		,
};

status_code_t hc42_init(sio_t* io)
{
	LOG_METHOD(hc42_init);
	__hc42_io = io;
	
	gpio_configure_pin(HC42_LPIN_pin, IO_Output | IO_InvertEnabled | IO_InitLow);
	
	gpio_configure_pin(HC42_RESET_pin, IO_Output | IO_InvertEnabled | IO_InitLow);
	gpio_configure_pin(HC42_KEY_pin, IO_Output | IO_InvertDisable | IO_InitLow);
	gpio_configure_pin(HC42_LINK_pin, IO_Input | IO_InvertEnabled);
	gpio_configure_pin(HC42_LEDCON_pin, IO_Input | IO_InvertDisable);
	gpio_configure_pin(HC42_LED1_pin, IO_Input | IO_InvertDisable);
	
	hc42_write_cmd(HC42_CMD_IBUUID_P, PSTR("0000FFE0-0000-1000-8000-00805F9B34FB"));
	
	return STATUS_OK;
}

void hc42_log_cmds(void)
{
	printf_P(PSTR("HC42 available commands:"));
	for (hc42_cmd_t i = 0; i < __HC42_CMDs_END; i++)
	{
		printf_P(PSTR(NL"\t %02d: %S"), i, string_PReadString(&hc42_cmds_txt[i]));
	}
}

void hc42_switch(State_t state)
{
	if (state == Off)
	{
		gpio_set_pin_low(HC42_LPIN_pin);
		Wait(10);
		hc42_write_cmd(HC42_CMD_PD);
	}
	else
	{
		gpio_set_pin_high(HC42_LPIN_pin);
		Wait(50);
		gpio_set_pin_low(HC42_LPIN_pin);
		Wait(CNF_HC42_WARM_UP);
	}
}

void hc42_write_cmd(hc42_cmd_t cmdId, ...)
{
	va_list args; va_start(args, cmdId);
	hc42_write_vP(string_PReadString(&hc42_cmds_txt[cmdId]), args);
	va_end(args);
}

void hc42_ask_cmd(hc42_cmd_t cmdId)
{
	string _cmd = (string)malloc(12);
	sprintf_P(_cmd, string_PReadString(&hc42_cmds_txt[cmdId]));
	int _i = StrIndexOf_Char(_cmd, '=');
	if (_i > 0 && _i < 12) { _cmd[_i] = '\0'; }
	LogString(_cmd);
	hc42_write(_cmd);
	free(_cmd);
}
