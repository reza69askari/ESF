/*
 * hc-42.h
 *
 * @Created: 10/13/2022 10:11:48
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */
//////////////////////////////////////////////////////////////////////////
//! #Requires:
//#define HC42_LPIN_pin	IOPORT_CREATE_PIN(PORTJ, PIN6_bp)
//////////////////////////////////////////////////////////////////////////

#ifndef HC_42_H_
#define HC_42_H_

#include <esf_compiler.h>

typedef enum hc42_commands_enum
{
	HC42_CMD_CHECK		,	/* AT */
	HC42_CMD_VERSION	,	/* AT+VERSION	R-	OK+VERSION=HC-42 V2.0.180601	www.hc01.com	 */
	HC42_CMD_NAME		,	/* AT+NAME=xxxx	RW	*HC-42*	max-length=12char	 */
	HC42_CMD_NAME_P		,	/* AT+NAME=xxxx	RW	*HC-42*	max-length=12char	 */
	HC42_CMD_RFPM		,	/* AT+RFPM=xx	RW	(unit: dBm) *4*, 3, 0, -4, -8, -12, -16, -20, -40	 */
	HC42_CMD_UART		,	/* AT+UART=xxxx	RW	1200, 2400, 4800, *9600*, 19200, 38400, 57600, 115200, 230400	 */
	/* AT+AINT=xx	RW	20ms-10000ms (multiple of 10) *Default value: 200ms*
	 * Broadcast interval(mS)			200 	500 	1000	2000	5000	10000
	 * Average operating current(?A)	70  	29  	16  	11  	6   	4.5
	 * */
	HC42_CMD_AINT		,
	HC42_CMD_PM			,	/* AT+PM=x		RW	*0*:power up & full speed, 1: power on & turn on low power consumption	 */
	HC42_CMD_SLEEP		,	/* AT+SLEEP		-W	 */
	HC42_CMD_PD			,	/* AT+PD		-W	 */
	HC42_CMD_LED1M		,	/* AT+LEDnM=x	RW	n=1,2 x=1:OFF, 2: ????, 3: ON	 */
	HC42_CMD_LED2M		,	/* AT+LEDnM=x	RW	n=1,2 x=1:OFF, 2: ????, 3: ON	 */
	HC42_CMD_DEFAULT	,	/* AT+DEFAULT	-W	 */
	HC42_CMD_ROLE		,	/* AT+ROLE=x	RW	*S:slave*, *P:peripheral*, M:maste, C:centeral	S same as P, M same as C	 */
	HC42_CMD_RESET		,	/* AT+RESET		-W	 */
	HC42_CMD_IBEN		,	/* AT+IBEN=x	RW	*0:OFF*, 1:ON	 */
	HC42_CMD_IBUUID		,	/* AT+IBUUID=********-****-****-****-************	RW	*FDA50693-A4E2-4FB1-AFCF-C6EB07647825*	 */
	HC42_CMD_IBUUID_P	,	/* AT+IBUUID=********-****-****-****-************	RW	*FDA50693-A4E2-4FB1-AFCF-C6EB07647825*	 */
	HC42_CMD_IBRSSI		,	/* AT+IBRSSI=x	RW	*0xC3*	 */
	HC42_CMD_IBMAJ		,	/* AT+IBMAJ=xxx	RW	*0x27C6*	 */
	HC42_CMD_IBMIN		,	/* AT+IBMIN=xxx	RW	*0x8B06*	 */
	
	__HC42_CMDs_END,
} hc42_cmd_t;

StreamIO* __hc42_io;

status_code_t hc42_init(sio_t* io);

#define hc42_write_package(buf, len) \
		fwrite((U8*)buf, sizeof(U8), len, __hc42_io)

#define hc42_write(buf, ...)		\
		fprintf(__hc42_io, buf, ##__VA_ARGS__)

#define hc42_write_P(bufP, ...)		\
		fprintf_P(__hc42_io, bufP, ##__VA_ARGS__)

#define hc42_write_vP(bufP, args)		\
		vfprintf_P(__hc42_io, bufP, args)

void hc42_log_cmds(void);

void hc42_switch(State_t state);

void hc42_write_cmd(hc42_cmd_t cmdId, ...);

void hc42_ask_cmd(hc42_cmd_t cmdId);



#endif // HC_42_H_