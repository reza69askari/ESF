/*
 * ad7190_spi.h
 * > Connection of AD7190 to SPI pripheral.
 *
 * @Created: 8/23/2022 20:44:32
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */
//////////////////////////////////////////////////////////////////////////
//! #Requires:
// 
//! #Options:
// 
//////////////////////////////////////////////////////////////////////////

#ifndef AD7190_SPI_H_
#define AD7190_SPI_H_

#include <conf_board.h>
#include <esf_compiler.h>
#include <util/delay.h>

static SPI_t* _AD7190_SPI;
static struct spi_device AD7190_Conf = { .id = AD7190_CS_pin };
#define AD7190_Device	(&AD7190_Conf)

static inline void ad7190_spi_init(void)
{
	LOG_FUNCTION();
	_AD7190_SPI = io_portid_to_spi(AD7190_PORT_id);
	
	spi_config_pins(_AD7190_SPI, SPI_MODE_3);
	spi_master_init(_AD7190_SPI);
	//! Chip is limit to 10MHz
	spi_master_setup_device(_AD7190_SPI, &AD7190_Conf, SPI_MODE_3, 1000000, 0);//8000000
	spi_enable(_AD7190_SPI);
	
	LogParameter(_AD7190_SPI);
	LogParameter(_AD7190_SPI->CTRL);
}

__always_inline static void ad7190_spi_select_device(void)
{
	spi_select_device(_AD7190_SPI, AD7190_Device);
	_delay_us(100);
}

__always_inline static void ad7190_spi_deselect_device(void)
{
	spi_deselect_device(_AD7190_SPI, AD7190_Device);
}

__always_inline static void ad7190_spi_write_packet(const U8* data, size_t len)
{
	spi_write_packet(_AD7190_SPI, data, len);
}

__always_inline static void ad7190_spi_write_byte(const U8 data)
{
	spi_write_byte(_AD7190_SPI, data);
}

__always_inline static void ad7190_spi_read_dummy(size_t len)
{
	spi_write_dummy(_AD7190_SPI, 0x00, len);
}

__always_inline static void ad7190_spi_read_packet(U8* data, size_t len)
{
	spi_read_packet_dummy(_AD7190_SPI, data, len, 0x00);
}

inline void AD7190_Active(void) { ad7190_spi_select_device(); }
inline void AD7190_Deactive(void) { ad7190_spi_deselect_device(); }
static inline void AD7190_Reset(void) { spi_write_dummy(_AD7190_SPI, 0xFF, 10); }


#endif // AD7190_SPI_H_