/*
 * esf_xmega_peripherals.h
 *
 * @Created: 10/13/2022 18:11:24
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_ATXMEAG_IO_H_
#  error "Include <MPUs/esf_io.h> instead of this file."
#endif

#ifndef ESF_XMEGA_PERIPHERALS_H_
#define ESF_XMEGA_PERIPHERALS_H_

#include <asf.h>

#define _PER_TC_EVSYS_Init(ev_ch, name, pin)	{ \
		EVSYS_CH ## ev_ch ## MUX = EVSYS_CHMUX_PORT ## name ## _PIN ## pin ## _gc; \
	}
#define _PER_CREATE_EVSYS(ev_ch)				TC_EVSEL_CH ## ev_ch ## _gc	//TC_EVSEL_CH0_gc
#define _PER_CREATE_TC(name, part)				&(TC ## name ## part)		//&TCC0
#define _PER_CREATE_USART(name, part)			&(USART ## name ## part)	//&USARTC0
#define _PER_CREATE_TRGSRC(name, part)			DMA_CH_TRIGSRC_USART ## name ## part ## _RXC_gc	//DMA_CH_TRIGSRC_USARTC0_RXC_gc

#define PERIPHERAL_TC_EVSYS_Init_Pin(ev_ch, name, pin)	_PER_TC_EVSYS_Init(ev_ch, name, pin)
#define __PER_TC_EVSYS_Init(ev_ch, name, part)	_PER_TC_EVSYS_Init(ev_ch, name, USART_RX ## part ## _bp)
#define PERIPHERAL_TC_EVSYS_Init_UsartRX(ev_ch, name, part)	(((part) == 0) ? \
		(_PER_TC_EVSYS_Init(ev_ch, name, 2)) : \
		(_PER_TC_EVSYS_Init(ev_ch, name, 6))) //EVSYS_CH1MUX = EVSYS_CHMUX_PORTC_PIN ## pin ## _gc
//#define PERIPHERAL_TC_EVSYS_Init_Part(ev_ch, name, part)	__PER_TC_EVSYS_Init(ev_ch, name, (part == 0? USART_RX0_bp: USART_RX1_bp))
#define PERIPHERAL_CREATE_EVSYS(ev_ch)			_PER_CREATE_EVSYS(ev_ch)		//TC_EVSEL_CH0_gc
#define PERIPHERAL_CREATE_TC(name, part)		_PER_CREATE_TC(name, part)		//&TCC0
#define PERIPHERAL_CREATE_USART(name, part)		_PER_CREATE_USART(name, part)	//&USARTC0
#define PERIPHERAL_CREATE_TRGSRC(name, part)	_PER_CREATE_TRGSRC(name, part)	//DMA_CH_TRIGSRC_USARTC0_RXC_gc


#endif // ESF_XMEGA_PERIPHERALS_H_