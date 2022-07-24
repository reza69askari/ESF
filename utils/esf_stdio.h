/*
 * esf_stdio.h
 *
 * @Created: 7/21/2022 16:28:45
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_STDIO_H_
#define ESF_STDIO_H_

#include <stdio.h>
#include <stdarg.h>
#include <compiler.h>
#include <esf_compiler.h>
#include <MPUs/esf_io.h>

//! > Optional
#include <ctype.h>

//! >>>>>>> Print

#define NewLine			"\r\n"
#define NL				NewLine

//static inline void print(const char *str) { printf(str); }
static inline int vprintf_P(const char *fmt, va_list ap) { return vfprintf_P(stdout, fmt, ap); }

//! >>>>>>> Scan

#define scan_readLine(buf)						fgets(buf, sizeof(buf), stdin)
#define scan_param(str, ...)					do { scanf(str, __VA_ARGS__); while (scan_hasAny()) { getchar(); } } while (0); //! to eat up the `\n` stored in stdin buffer
#define scan_get_param(title, format, ...)		do { printf_P(PSTR(NL title)); scan_param(format, __VA_ARGS__); } while (0);

//! >>>>>>> Stream

typedef FILE sio_t, StreamIO;
typedef int	(*put_t)(char c, sio_t* io);
typedef int	(*get_t)(sio_t* io);

#ifdef _USART_H_
static inline int stream_putc_usart(char c, sio_t* stream)
{
	if (c != '\r') {
		if (c == '\n') { usart_putchar((USART_t*)(stream->udata), '\r'); }
		usart_putchar((USART_t*)(stream->udata), c);
	}
	return 0;
}
static inline int stream_getc_usart(sio_t* stream) { return (int)usart_getchar((USART_t*)(stream->udata)); }
static inline bool scan_hasAny_usart(void* stream) { return usart_rx_is_complete((USART_t*)(((sio_t*)stream)->udata)); }
#endif // _USART_H_

#ifdef _UDI_CDC_H_
static inline int stream_putc_usb(char c, sio_t* stream)
{
	if (!udi_cdc_is_tx_ready()) { return ERR_FLUSHED; }
	if (c != '\r') {
		if (c == '\n') { udi_cdc_putc('\r'); }
		udi_cdc_putc(c);
	}
	return 0;
}
static inline int stream_getc_usb(sio_t* stream) { return udi_cdc_getc(); }
static inline bool scan_hasAny_usb(void* stream) { return udi_cdc_is_rx_ready(); }
#endif // _UDI_CDC_H_

static inline test_pt __capture_ready_fun__(test_pt event)
{
	static test_pt _ev;
	if (event != NULL) { _ev = event; }
	return _ev;
}
#define scan_hasAny()	((__capture_ready_fun__(NULL))(((stdin)->udata)))

#define StreamRead			__SRD	/**< fdev_setup_stream() with read intent */
#define StreamWrite			__SWR	/**< fdev_setup_stream() with write intent */
#define StreamReadWrite    (__SRD|__SWR)	/**< fdev_setup_stream() with read/write intent */

#define stream_builder(s, io, f)	\
	do { \
		(s)->put = __get_putter__(io); \
		(s)->get = __get_getter__(io); \
		(s)->flags = f; \
		(s)->udata = (io); \
	} while(0)
#define STREAM_BUILDER(io, p, g, f)	\
	{ \
		.put = p, \
		.get = g, \
		.flags = f, \
		.udata = (io), \
	}
#define stream_builder_full(s, io, p, g, f)	\
	do { \
		(s)->put = p; \
		(s)->get = g; \
		(s)->flags = f; \
		(s)->udata = (io); \
	} while(0)

#define stream_putc(c, sio_t)		((sio_t)->put)(c, sio_t)
#define stream_getc(c, sio_t)		((sio_t)->get)(sio_t)

/* Invoke it next to 'sysclk_init()'
 * 
 * IRQ-enable is required on USB
 * */
static inline status_code_t stream_init(void* io, U32 baudrate)
{
	static sio_t printOut;
	static sio_t printIn;
	
	if (0) {}
#ifdef _UDI_CDC_H_
	else if (is_usb(io))
	{
		udc_start();
		__capture_ready_fun__(scan_hasAny_usb);
		stream_builder_full(&printOut, io, stream_putc_usb, NULL, StreamWrite);
		stream_builder_full(&printIn, io, NULL, stream_getc_usb, StreamRead);
	}
#endif // _UDI_CDC_H_
#ifdef _USART_H_
	else if (is_usart(io))
	{
		usart_init((usart_if)io, baudrate);
		__capture_ready_fun__(scan_hasAny_usart);
		stream_builder_full(&printOut, io, stream_putc_usart, NULL, StreamWrite);
		stream_builder_full(&printIn, io, NULL, stream_getc_usart, StreamRead);
	}
#endif // _USART_H_
	else { Exception(ArgumentOutOfRangeException); return ERR_FLUSHED; }
	stdout = &printOut;
	stdin = &printIn;
	return STATUS_OK;
}

__always_inline static put_t __get_putter__(void* io)
{
	return 
		#ifdef _USART_H_
		is_usart(io) ? stream_putc_usart : 
		#endif // _USART_H_
		#ifdef _UDI_CDC_H_
		is_usb(io) ? stream_putc_usb : 
		#endif // _UDI_CDC_H_
		NULL;
}
__always_inline static get_t __get_getter__(void* io)
{
	return 
		#ifdef _USART_H_
		is_usart(io) ? stream_getc_usart : 
		#endif // _USART_H_
		#ifdef _UDI_CDC_H_
		is_usb(io) ? stream_getc_usb : 
		#endif // _UDI_CDC_H_
		NULL;
}


#endif // ESF_STDIO_H_