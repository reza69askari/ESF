/*
 * esf_xmega_spi.h
 *
 * @Created: 7/24/2022 15:54:55
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_ATXMEAG_IO_H_
#  error "Include <MPUs/esf_io.h> instead of this file."
#endif

#ifndef ESF_XMEGA_SPI_H_
#define ESF_XMEGA_SPI_H_

#define SPI_CS_pin_bm				PIN4_bm
#define SPI_CS_pin_bp				PIN4_bp
#define SPI_MOSI_pin_bm				PIN5_bm
#define SPI_MOSI_pin_bp				PIN5_bp
#define SPI_MISO_pin_bm				PIN6_bm
#define SPI_MISO_pin_bp				PIN6_bp
#define SPI_SCK_pin_bm				PIN7_bm
#define SPI_SCK_pin_bp				PIN7_bp

#include <asf.h>
#include <compiler.h>

#define IO_SPI_BASE_OFFSET			IOPORT_PORTC
#define IO_SPI_BASE_ADDRESS			(uintptr_t)&SPIC//0x08C0
#define IO_SPI_OFFSET				((uintptr_t)&SPID - IO_SPI_BASE_ADDRESS)//0x09C0

#ifdef _SPI_MASTER_H_

//#include <spi_master.h>
static inline uint8_t spi_read_write_single (SPI_t *spi, uint8_t data)
{
	uint8_t responce;
	spi_write_single(spi, data);
	while (!spi_is_rx_full(spi)) {
	}
	spi_read_single(spi, &responce);
	return responce;
}

static inline SPI_t* io_portid_to_spi(port_id_t port)
{
	//ioport_id_pin_to_port(port)
	port -= IO_SPI_BASE_OFFSET;
	return (SPI_t *)((uintptr_t)IO_SPI_BASE_ADDRESS + (port * IO_SPI_OFFSET));
}

static inline ioport_pin_t spi_config_pins (SPI_t *spi, spi_flags_t mode)
{
	uint8_t ioport = 0;
	switch ((uint16_t)spi)
	{
#ifdef SPIC
		case (uint16_t)&SPIC: ioport = IOPORT_PORTC; break;
#endif // SPIC
#ifdef SPID
		case (uint16_t)&SPID: ioport = IOPORT_PORTD; break;
#endif // SPID
#ifdef SPIE
		case (uint16_t)&SPIE: ioport = IOPORT_PORTE; break;
#endif // SPIE
#ifdef SPIF
		case (uint16_t)&SPIF: ioport = IOPORT_PORTF; break;
#endif // SPIF
	}
	gpio_configure_group(ioport, SPI_CS_pin_bm | SPI_MOSI_pin_bm, IO_Output | IO_InitHigh);
	IO_ConfigureFlags_t _sckFlags = IO_Output;
	if (mode == 0) { _sckFlags |= IO_InitLow | IO_SenseRising; }
	else if (mode == 1) { _sckFlags |= IO_InitLow | IO_SenseFalling; }
	else if (mode == 2) { _sckFlags |= IO_InitHigh | IO_SenseFalling; }
	else if (mode == 3) { _sckFlags |= IO_InitHigh | IO_SenseRising; }
	gpio_configure_group(ioport, SPI_SCK_pin_bm, _sckFlags);
	gpio_configure_group(ioport, SPI_MISO_pin_bm, IO_Input);
	return (ioport * 8 + SPI_CS_pin_bp); //! return cs_pin
}

#endif // _SPI_MASTER_H_


#endif // ESF_XMEGA_SPI_H_