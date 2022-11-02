/*
 * esf_stdio.h
 *
 * @Created: 7/21/2022 16:28:45
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_STDIO_H_
#define ESF_STDIO_H_

//! > Required
#include <stdio.h>
#include <stdarg.h>
#include <esf_compiler.h>
#include <MPUs/esf_io.h>

//! > Optional
#include <ctype.h>

//! >>>>>>> Print

#define NewLine						"\n" //\r\n
#define NL							NewLine

#define PUTS_P(str)					puts_P(PSTR(str))
#define FPUTS_P(str, stream)		fputs_P(PSTR(str), stream)

static inline int vprintf_P(const char *fmt, va_list ap) { return vfprintf_P(stdout, fmt, ap); }

#define PRINTF_P(fmt, ...)			printf_P(PSTR(fmt), ##__VA_ARGS__)
#define SPRINTF_P(s, fmt, ...)		sprintf_P(s, PSTR(fmt), ##__VA_ARGS__)
#define FPRINTF_P(stream, fmt, ...)	fprintf_P(stream, PSTR(fmt), ##__VA_ARGS__)
#define VFPRINTF_P(stream, fmt, ap)	vfprintf_P(stream, PSTR(fmt), ap)
#define SNPRINTF_P(s, n, fmt, ...)	snprintf_P(s, n, PSTR(fmt), ##__VA_ARGS__)
#define VSPRINTF_P(s, fmt, ap)		vsprintf_P(s, PSTR(fmt), ap)
#define VSNPRINTF_P(s, n, fmt, ap)	vsnprintf_P(s, n, PSTR(fmt), ap)

//! >>>>>>> Scan

#define scan_readLine(buf)			fgets(buf, sizeof(buf), stdin)
#define scan_param(str, ...)		do { scanf(str, __VA_ARGS__); while (scan_hasAny()) { getchar(); } } while (0); //! to eat up the `\n` stored in stdin buffer
#define scan_get_param(title, format, ...) do { printf_P(PSTR(NL title)); scan_param(format, __VA_ARGS__); } while (0);

#define SCANF_P(fmt, ...)			scanf_P(PSTR(fmt), ##__VA_ARGS__)
#define FSCANF_P(stream, fmt, ...)	fscanf_P(stream, PSTR(fmt), ##__VA_ARGS__)
#define SSCANF_P(buf, fmt, ...)		sscanf_P(buf, PSTR(fmt), ##__VA_ARGS__)
#define VFSCANF_P(stream, fmt, ap)	vfscanf_P(stream, PSTR(fmt), ap)

//! >>>>>>> Stream

typedef FILE sio_t, StreamIO;
typedef int	(*put_t)(char c, sio_t* io);
typedef int	(*get_t)(sio_t* io);

typedef enum Stream_IO_Type_enum
{
	IO_IS_USART,
	IO_IS_USB,
	IO_IS_SPI,
	IO_IS_TWI,
} StreamIOType, stream_type_t;

typedef struct stream_advanced_struct
{
	void* io;
	test_pt hasData;
	stream_type_t type;
	U8 exflags;
#define __S_DAR						0x0001	/* Don't auto apply return line ('\r') before new line. by default '\n' -> "\r\n" */
#define __S_ANL						0x0002	/* Avoid new line. '\n' -> "" */
#define SCnf_RNTR					0x0002	/* replace new line to return line. '\n' -> "\r" */
} stream_advanced_t, UserStreamData, udata_t;

#define stream_get_udata(stream)	((udata_t*)(((sio_t*)(stream))->udata))

#ifdef _USART_H_
static inline int stream_putc_usart(char c, sio_t* stream)
{
	udata_t* _ud = stream_get_udata(stream);
	USART_t* _io = (USART_t*)_ud->io;
	if (c == '\n')
	{
		if (!HasFlag(_ud->exflags, __S_DAR)) { usart_putchar(_io, '\r'); }
		if (HasFlag(_ud->exflags, __S_ANL)) { return 0; }
	}
	usart_putchar(_io, c);
	return 0;
}
static inline int stream_getc_usart(sio_t* stream) { return (int)usart_getchar((USART_t*)(stream_get_udata(stream)->io)); }
static inline bool scan_hasAny_usart(void* stream) { return usart_rx_is_complete((USART_t*)(stream_get_udata(stream)->io)); }
#endif // _USART_H_

#ifdef _UDI_CDC_H_
static inline int stream_putc_usb(char c, sio_t* stream)
{
	udata_t* _ud = stream_get_udata(stream);
	if (!udi_cdc_is_tx_ready()) { return ERR_FLUSHED; }
	if (c == '\n')
	{
		if (!HasFlag(_ud->exflags, __S_DAR)) { udi_cdc_putc('\r'); }
		if (HasFlag(_ud->exflags, __S_ANL)) { return 0; }
	}
	udi_cdc_putc(c);
	return 0;
}
static inline int stream_getc_usb(sio_t* stream) { return udi_cdc_getc(); }
static inline bool scan_hasAny_usb(void* stream) { return udi_cdc_is_rx_ready(); }
#endif // _UDI_CDC_H_

#define StreamRead					(__SRD)	/**< fdev_setup_stream() with read intent */
#define StreamWrite					(__SWR)	/**< fdev_setup_stream() with write intent */
#define StreamReadWrite				(__SRD|__SWR)	/**< fdev_setup_stream() with read/write intent */

#define stream_putc(c, stream)		((stream)->put)(c, stream)
#define stream_getc(stream)			((stream)->get)(stream)
#define stream_hasData(stream)		((stream_get_udata(stream)->hasData)(stream))
#define scan_hasAny()				(stream_hasData(stdin))

/* 
 * 'stream': memory to capture info, NULL is welcome (malloc is using)
 * 'io': required, the peripheral used as stream-io
 * 'io_init': optional method to initialize io.
 * 'setAsDefault': make this stream as default stdio stream, known as Terminal. (usable on printf and scanf)
 * 
 * #Mind: IRQ-enable is required on USB
 * #Note: to use this as Terminal initializer, call it next to 'sysclk_init()'
 * 
 * Ex: #USB
	static status_code_t io_init(void* io) {
		udc_start();
		pmic_init();
		Enable_global_interrupt();
		return STATUS_OK;
	}
	static void system_init(void) {
		sysclk_init();
		stream_init(NULL, &USB, io_init, true, 0);
		Log_Init();
	}
	int main (void) {
		system_init();
		board_init();
		application_init();
		//! initialization is completed, running main-loop
		main_loop();
	}
 * Ex: #USART
	static status_code_t io_init(void* io) {
		usart_init(io, 115200);
		return STATUS_OK;
	}
	static void system_init(void) {
		sysclk_init();
		stream_init(NULL, &USARTC0, io_init, true, 0);
		Log_Init();
	}
	int main (void) {
		system_init();
		board_init();
		application_init();
		//! initialization is completed, running main-loop
		main_loop();
	}
 * */
static inline sio_t* stream_init(sio_t* stream, void* io, event_pt io_init, bool setAsDefault, U8 exflags)
{
	Assert(io);
	if (!stream)
	{
		stream = (sio_t*)malloc(sizeof(sio_t));
		stream->udata = (udata_t*)malloc(sizeof(udata_t));
	}
	if (!(stream->udata))
	{
		stream->udata = (udata_t*)malloc(sizeof(udata_t));
	}
	udata_t* _udata = stream->udata;
	_udata->io = io;
	_udata->exflags = exflags;
	if (0) {}
#ifdef _UDI_CDC_H_
	else if (is_usb(io))
	{
		fdev_setup_stream(stream, stream_putc_usb, stream_getc_usb, StreamReadWrite);
		_udata->type = IO_IS_USB;
		_udata->hasData = scan_hasAny_usb;
	}
#endif // _UDI_CDC_H_
#ifdef _USART_H_
	else if (is_usart(io))
	{
		fdev_setup_stream(stream, stream_putc_usart, stream_getc_usart, StreamReadWrite);
		_udata->type = IO_IS_USART;
		_udata->hasData = scan_hasAny_usart;
	}
#endif // _USART_H_
	else { Exception(ArgumentOutOfRangeException); return NULL; }
	stream->udata = _udata;
	if (setAsDefault)
	{
		stdout = stream; //! Read and Write: all three standard streams will be identical.
		stdin = stream;
	}
	if (io_init) { io_init(io); }
	return stream;
}


#endif // ESF_STDIO_H_