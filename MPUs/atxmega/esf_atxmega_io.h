/*
 * esf_atxmega_io.h
 *
 * Created: 12/17/2017 15:51:19
 *  Author: MohammadReza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_ATXMEAG_IO_H_
#define ESF_ATXMEAG_IO_H_

#include <stdio.h>

//! > require ASF must include
#ifndef CONF_CLOCK_H_INCLUDED
#	warning "Add 'System Clock Control (service)' in ASF"
#endif
#ifndef XMEGA_DRIVERS_CPU_RESET_CAUSE_H //COMMON_DRIVERS_CPU_RESET_CAUSE_H
#	warning "Add 'CPU specific features (driver)' in ASF"
#endif
#ifndef NVM_H
#	warning "Add 'NVM - Non Volatile Memory (driver)' in ASF"
#endif

#define DMA_CHANNEL_0				0
#define DMA_CHANNEL_1				1
#define DMA_CHANNEL_2				2
#define DMA_CHANNEL_3				3

#define ioport_create_pin(port_id, pin) (port_id * 8 + (pin))

#ifdef _XMEGA_GPIO_H_

#define gpio_set_pin_level(pin, lvl) ioport_set_pin_level(pin, (lvl))
//! > Pin
#define PIN_on(pin)					gpio_set_pin_high(pin)
#define PIN_off(pin)				gpio_set_pin_low(pin)
#define PIN_tgl(pin)				gpio_toggle_pin(pin)
#define PIN_lvl(pin, lvl)			gpio_set_pin_level(pin, lvl)
//! > LED
#define Led_on(pin)					PIN_on(pin)
#define Led_off(pin)				PIN_off(pin)
#define Led_tgl(pin)				PIN_tgl(pin)
#define Led_lvl(pin, lvl)			PIN_lvl(pin, lvl)

#endif // _XMEGA_GPIO_H_

#pragma region IO_Check

static inline bool is_usart(void* io)
{
	switch ((uintptr_t)io)
	{
		case (uintptr_t)&USARTC0:
		case (uintptr_t)&USARTC1:
		case (uintptr_t)&USARTD0:
		case (uintptr_t)&USARTD1:
		case (uintptr_t)&USARTE0:
		case (uintptr_t)&USARTE1:
		case (uintptr_t)&USARTF0:
		case (uintptr_t)&USARTF1:
			return true;
	}
	return false;
}
static inline bool is_spi(void* io)
{
	switch ((uintptr_t)io)
	{
		case (uintptr_t)&SPIC:
		case (uintptr_t)&SPID:
		case (uintptr_t)&SPIE:
		case (uintptr_t)&SPIF:
		return true;
	}
	return false;
}
static inline bool is_twi(void* io)
{
	switch ((uintptr_t)io)
	{
		case (uintptr_t)&TWIC:
		case (uintptr_t)&TWID:
		case (uintptr_t)&TWIE:
		case (uintptr_t)&TWIF:
		return true;
	}
	return false;
}
static inline bool is_usb(void* io)
{
	switch ((uintptr_t)io)
	{
		case (uintptr_t)&USB:
		return true;
	}
	return false;
}
#pragma endregion IO_Check

#ifdef XMEGA_SYSCLK_H_INCLUDED
static inline void eventsystem_init(void)
{
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
}

static inline bool sysclk_is_module_enable (enum sysclk_port_id port, uint8_t id)
{
	return (*((uint8_t *)&PR.PRGEN + port) & id) == 0x00? true: false;
}
#endif // XMEGA_SYSCLK_H_INCLUDED

#ifdef DRIVERS_DMA_DMA_H
__always_inline static uint16_t dma_channel_get_transfer_count(dma_channel_num_t num)
{
	return ((DMA_CH_t *)(dma_get_channel_address_from_num(num)))->TRFCNT;
}
#endif // DRIVERS_DMA_DMA_H

#ifdef ADC_H
__always_inline static void adcch_write_input (ADC_t *adc, uint8_t ch_mask, uint8_t pos)
{
	static ADC_CH_t *adc_ch;
	static irqflags_t flags;
	adc_ch = adc_get_channel(adc, ch_mask);
	flags = cpu_irq_save();
	adc_ch->MUXCTRL = pos << ADC_CH_MUXPOS_gp;
	cpu_irq_restore(flags);
}
#endif // ADC_H

#ifdef TIMEOUT_H
//! tms useful in RTC-Clock of ULP and RTC-Div 1 and Timeout-Tick and Source = 1024
//#define tms(value)		(value / 10)
#endif

#ifdef NVM_H
#include <stdio.h>

//! Watchdog Enable & period > 8CLK
//! BOD (in Power down & Active) Enable & Level > 1V6
//! Startup time > 0mS
//! if not defined CONF_FUSE_SKIP_RSTDISBL (in conf_board.h) -> Reset pin must be disable (Reset disable = Enable)
static inline void fuse_check (void)
{
	uint8_t hang = 0x00;
	NVM_FUSES_t fuses;
	fuses.FUSEBYTE1 = nvm_fuses_read(FUSEBYTE1);
	fuses.FUSEBYTE2 = nvm_fuses_read(FUSEBYTE2);
	fuses.FUSEBYTE4 = nvm_fuses_read(FUSEBYTE4);
	fuses.FUSEBYTE5 = nvm_fuses_read(FUSEBYTE5);
	
#ifdef NVM_FUSES_WDPER_gm
	if ((fuses.FUSEBYTE1 & NVM_FUSES_WDPER_gm) == WDPER_8CLK_gc) { hang |= (1 << 0); }
#else
	if ((fuses.FUSEBYTE1& NVM_FUSES_WDP_gm) == WDP_8CLK_gc) { hang |= (1 << 0); }
#endif
	if ((fuses.FUSEBYTE4 & NVM_FUSES_WDLOCK_bm)) { hang |= (1 << 1); }
	
	if ((fuses.FUSEBYTE2 & NVM_FUSES_BODPD_gm) == BODPD_DISABLED_gc) { hang = (1 << 2); }
	if ((fuses.FUSEBYTE5 & NVM_FUSES_BODACT_gm) == BODACT_DISABLED_gc) { hang = (1 << 3); }
#ifdef NVM_FUSES_BODLEVEL_gm
	if ((fuses.FUSEBYTE5 & NVM_FUSES_BODLEVEL_gm) == BODLEVEL_1V6_gc) { hang = (1 << 4); }
#else
	if ((fuses.FUSEBYTE5 & NVM_FUSES_BODLVL_gm) == BODLVL_1V6_gc) { hang = (1 << 4); }
#endif
	
#ifdef NVM_FUSES_STARTUPTIME_gm
	if ((fuses.FUSEBYTE4 & NVM_FUSES_STARTUPTIME_gm) == STARTUPTIME_0MS_gc) { hang = (1 << 5); }
#else
	if ((fuses.FUSEBYTE4 & NVM_FUSES_SUT_gm) == SUT_0MS_gc) { hang = (1 << 5); }
#endif // NVM_FUSES_STARTUPTIME_gm
	
#ifndef CONF_FUSE_SKIP_RSTDISBL
	if ((fuses.FUSEBYTE4 & NVM_FUSES_RSTDISBL_bm)) { hang = (1 << 6); }
#endif // CONF_FUSE_SKIP_RSTDISBL
	
	if (hang) { printf_P(PSTR("Fuse bits is not set correctly: 0x%02X"), hang); }
	while (hang) {
		asm("nop");
	}
}

typedef union deviceInfo {
	struct nvm_device_id deviceID;
	struct nvm_device_serial deviceSerial;
} deviceInfo_t;
#endif // NVM_H

//! >>>>>>> rest related esf files
#include "include/esf_xmega_tc.h"
#include "include/esf_xmega_usart.h"
#include "include/esf_xmega_spi.h"
#include "include/esf_xmega_peripherals.h"
#include "include/esf_xmega_dma.h"


#endif /* ESF_ATXMEAG_IO_H_ */