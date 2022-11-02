/*
 * esf_xmega_usart.h
 *
 * @Created: 10/6/2016 4:00:03 PM
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */
//////////////////////////////////////////////////////////////////////////
//! #Requires:
// #define USART_SERIAL_CHAR_LENGTH		USART_CHSIZE_8BIT_gc
// #define USART_SERIAL_PARITY			USART_PMODE_DISABLED_gc
// #define USART_SERIAL_STOP_BIT		1//false
//! #Options:
// 
//////////////////////////////////////////////////////////////////////////

#ifndef ESF_ATXMEAG_IO_H_
#  error "Include <MPUs/esf_io.h> instead of this file."
#endif

#ifndef ESF_XMEGA_USART_H_
#define ESF_XMEGA_USART_H_

#include <asf.h>

#define USART_RX0_bm			PIN2_bm
#define USART_RX0_bp			PIN2_bp
#define USART_TX0_bm			PIN3_bm
#define USART_TX0_bp			PIN3_bp
#define USART_RX1_bm			PIN6_bm
#define USART_RX1_bp			PIN6_bp
#define USART_TX1_bm			PIN7_bm
#define USART_TX1_bp			PIN7_bp

#ifdef _USART_SERIAL_H_

#include <config/conf_usart_serial.h>
//#include <usart_serial.h>
static inline bool usart_init(usart_if usart, uint32_t baudrate)
{
	usart_rs232_options_t usart_options = {
		.baudrate = baudrate,//USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};
	if (0) {}
	#ifdef USARTC0
	else if((uint16_t)usart == (uint16_t)&USARTC0) {
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, USART_RX0_bp), IO_Input);
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, USART_TX0_bp), IO_Output | IO_InitHigh);
	}
	#endif
	#ifdef USARTC1
	else if((uint16_t)usart == (uint16_t)&USARTC1) {
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, USART_RX1_bp), IO_Input);
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, USART_TX1_bp), IO_Output | IO_InitHigh);
	}
	#endif
	#ifdef USARTD0
	else if((uint16_t)usart == (uint16_t)&USARTD0) {
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, USART_RX0_bp), IO_Input);
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, USART_TX0_bp), IO_Output | IO_InitHigh);
	}
	#endif
	#ifdef USARTD1
	else if((uint16_t)usart == (uint16_t)&USARTD1) {
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, USART_RX1_bp), IO_Input);
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, USART_TX1_bp), IO_Output | IO_InitHigh);
	}
	#endif
	#ifdef USARTE0
	else if((uint16_t)usart == (uint16_t)&USARTE0) {
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTE, USART_RX0_bp), IO_Input);
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTE, USART_TX0_bp), IO_Output | IO_InitHigh);
	}
	#endif
	#ifdef USARTE1
	else if((uint16_t)usart == (uint16_t)&USARTE1) {
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTE, USART_RX1_bp), IO_Input);
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTE, USART_TX1_bp), IO_Output | IO_InitHigh);
	}
	#endif
	#ifdef USARTF0
	else if((uint16_t)usart == (uint16_t)&USARTF0) {
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTF, USART_RX0_bp), IO_Input);
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTF, USART_TX0_bp), IO_Output | IO_InitHigh);
	}
	#endif
	#ifdef USARTF1
	else if((uint16_t)usart == (uint16_t)&USARTF1) {
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTF, USART_RX1_bp), IO_Input);
		ioport_configure_pin(IOPORT_CREATE_PIN(PORTF, USART_TX1_bp), IO_Output | IO_InitHigh);
	}
	#endif
	return usart_serial_init(usart, &usart_options);
}

#endif // _USART_SERIAL_H_


#endif /* ESF_XMEGA_USART_H_ */