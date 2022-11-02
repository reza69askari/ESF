/*
 * esf_io.h
 *
 * @Created: 7/11/2022 6:57:06
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_IO_H_
#define ESF_IO_H_

#include <compiler.h>
#include <asf.h>

//! >>>>>>>>>>>>>>>> IOPORT_Helper <<<<<<<<<<<<<<<<
#ifdef IOPORT_H

typedef ioport_pin_t pin_t;
typedef enum IO_Configuration_Flags_enum
{
	IO_Input						= IOPORT_DIR_INPUT,
	IO_Output						= IOPORT_DIR_OUTPUT,
	
	/** \name Initial Output State Flags */
	IO_InitLow						= IOPORT_INIT_LOW,		/*!< Initial Output State Low */
	IO_InitHigh						= IOPORT_INIT_HIGH,		/*!< Initial Output State High */
	
	/** \name Input/Sense Configuration Flags */
	IO_SenseBothEdges				= IOPORT_BOTHEDGES,		/*!< Sense Both Edges */
	IO_SenseRising					= IOPORT_RISING,		/*!< Sense Rising Edge */
	IO_SenseFalling					= IOPORT_FALLING,		/*!< Sense Falling Edge */
	IO_SenseLowLevel				= IOPORT_LEVEL,			/*!< Sense Low Level */
	#if XMEGA_E
	IO_SenseForce					= IOPORT_FORCE_ENABLE,	/*!< Sense Force Input Enable Low Level */
	#endif
	IO_InputBufferDisable			= IOPORT_INPUT_DISABLE,	/*!< Input Buffer Disabled */
	
	/** \name Output and Pull Configuration Flags */
	IO_Totem						= IOPORT_TOTEM,			/*!< Normal push/pull output */
	IO_BusKeeper					= IOPORT_BUSKEEPER,		/*!< Bus Keeper */
	IO_PullDOWN						= IOPORT_PULL_DOWN,		/*!< Pull-Down (when input) */
	IO_PullUP						= IOPORT_PULL_UP,		/*!< Pull-Up (when input) */
	IO_WiredOR						= IOPORT_WIRED_OR,		/*!< Wired OR */
	IO_WiredAND						= IOPORT_WIRED_AND,		/*!< Wired AND */
	IO_WiredOR_PullDOWN				= IOPORT_WIRED_OR_PULL_DOWN,	/*!< Wired OR and Pull-Down */
	IO_WiredAND_PullUP				= IOPORT_WIRED_AND_PULL_UP,		/*!< Wired AND and Pull-Up */
	
	/** \name Inverted I/O Configuration Flags */
	IO_InvertEnabled				= IOPORT_INV_ENABLED,	/*!< I/O is Inverted */
	IO_InvertDisable				= IOPORT_INV_DISABLE,	/*!< I/O is Not Inverted */
	
	/** \name Slew Rate Limit Configuration Flags */
	IO_SRL_Enabled					= IOPORT_SRL_ENABLED,	/*!< Slew Rate Limit Enabled */
	IO_SRL_Disabled					= IOPORT_SRL_DISABLED,	/*!< Slew Rate Limit Disabled */
} IO_ConfigureFlags_t;

#endif // IOPORT_H

//! >>>>>>>>>>>>>>>> Timer/Counter <<<<<<<<<<<<<<<<
#ifdef _TC_H_

/**
 * \brief Select clock for a specified TC and resolution.
 *
 * This function configures the clock selection, as prescaled CLKper, for a
 * specified TC that gives a resolution at least as low as the one specified.
 * The resolution of a TC is synonymous with its clock frequency.
 *
 * \note It is also possible to clock TCs with event channels. This is not
 * handled by this implementation.
 *
 * \param tc   ID of TC to get clock selection for.
 * \param resolution Desired resolution for the TC in Hz.
 */
static inline void tc_set_max_resolution(volatile void *tc, uint32_t resolution)
{
	uint32_t tc_clk_rate = sysclk_get_per_hz() / resolution;

	if ((tc_clk_rate / 1) <= (uint32_t)UINT16_MAX) {
		tc_write_clock_source(tc, TC_CLKSEL_DIV1_gc);
	} else if ((tc_clk_rate / 2) <= (uint32_t)UINT16_MAX) {
		tc_write_clock_source(tc, TC_CLKSEL_DIV2_gc);
	} else if ((tc_clk_rate / 4) <= (uint32_t)UINT16_MAX) {
		tc_write_clock_source(tc, TC_CLKSEL_DIV4_gc);
	} else if ((tc_clk_rate / 8) <= (uint32_t)UINT16_MAX) {
		tc_write_clock_source(tc, TC_CLKSEL_DIV8_gc);
	} else if ((tc_clk_rate / 64) <= (uint32_t)UINT16_MAX) {
		tc_write_clock_source(tc, TC_CLKSEL_DIV64_gc);
	} else if ((tc_clk_rate / 256) <= (uint32_t)UINT16_MAX) {
		tc_write_clock_source(tc, TC_CLKSEL_DIV256_gc);
	} else {
		tc_write_clock_source(tc, TC_CLKSEL_DIV1024_gc);
	}
}
static inline void tc_set_periode_and_source (volatile void *tc, uint32_t tc_tick_hz)
{
	tc_set_max_resolution(tc, tc_tick_hz);
	//tc_write_period(tc, (tc_get_resolution(tc) / tc_tick_hz));
	tc_write_period(tc, (tc_get_resolution(tc) / tc_tick_hz) -1);
}

#ifdef PWM_H

/* To support multi channel, correcting config,
 * use 'pwm_start(...)' to run the PWM.
 **/
static inline void pwm_init_multi_channels (struct pwm_config *config, enum pwm_tc_t tc,
	enum pwm_channel_t channel, uint16_t freq_hz, enum tc_cc_channel_mask_enable_t channels)
{
	pwm_init(config, tc, channel, freq_hz);
	config->cc_mask = channels;
}
static inline void pwm_start_channel(struct pwm_config *config, enum tc_cc_channel_t channel_index, uint8_t duty_cycle_scale)
{
	config->channel = channel_index;
	pwm_start(config, duty_cycle_scale);
}

#endif // PWM_H
#endif // _TC_H_

//! >>>>>>>>>>>>>>>> SPI Master <<<<<<<<<<<<<<<<
#ifdef _SPI_MASTER_H_

__always_inline static void spi_write_byte(SPI_t* spi, const U8 value)
{
	spi_write_packet(spi, &value, 1); //Dummy write
}
__always_inline static void spi_write_dummy(SPI_t* spi, const U8 value, size_t len)
{
	while (len--) {
		spi_write_byte(spi, value);
	}
}
__always_inline static void spi_read_byte(SPI_t* spi, U8* data)
{
	spi_read_packet(spi, data, 1);
}
static inline void spi_read_packet_dummy(SPI_t* spi, U8* data, size_t len, U8 dummy)
{
	while (len--) {
		spi_write_single(spi,dummy); //Dummy write
		while (!spi_is_rx_full(spi)) {
		}
		spi_read_single(spi, data);
		data++;
	}
}

#endif // _SPI_MASTER_H_

//! >>>>>>>>>>>>>>>> MPUs-IO <<<<<<<<<<<<<<<<
#if XMEGA
#	include <MPUs/atxmega/esf_atxmega_io.h>
#else
#	error ArgumentException
#endif


#endif // ESF_IO_H_