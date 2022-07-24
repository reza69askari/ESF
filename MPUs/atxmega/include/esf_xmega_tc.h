/*
 * esf_xmega_tc.h
 *
 * @Created: 7/24/2022 15:39:51
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_ATXMEAG_IO_H_
#  error "Include <MPUs/esf_io.h> instead of this file."
#endif

#ifndef ESF_XMEGA_TC_H_
#define ESF_XMEGA_TC_H_

#include <asf.h>
#include <esf_compiler.h>

#ifdef _TC_H_

//#include <tc.h>
static inline bool tc_is_enable (volatile void *tc)
{
	bool tcIsEnable = false;
#ifdef TCC0
	if ((uintptr_t) tc == (uintptr_t) & TCC0) {
		tcIsEnable = sysclk_is_module_enable(SYSCLK_PORT_C, SYSCLK_TC0) && sysclk_is_module_enable(SYSCLK_PORT_C, SYSCLK_HIRES);
	} else
#endif
#ifdef TCC1
	if ((uintptr_t) tc == (uintptr_t) & TCC1) {
		tcIsEnable = sysclk_is_module_enable(SYSCLK_PORT_C, SYSCLK_TC1) && sysclk_is_module_enable(SYSCLK_PORT_C, SYSCLK_HIRES);
	} else
#endif
#ifdef TCD0
	if ((uintptr_t) tc == (uintptr_t) & TCD0) {
		tcIsEnable = sysclk_is_module_enable(SYSCLK_PORT_D, SYSCLK_TC0) && sysclk_is_module_enable(SYSCLK_PORT_D, SYSCLK_HIRES);
	} else
#endif
#ifdef TCD1
	if ((uintptr_t) tc == (uintptr_t) & TCD1) {
		tcIsEnable = sysclk_is_module_enable(SYSCLK_PORT_D, SYSCLK_TC1) && sysclk_is_module_enable(SYSCLK_PORT_D, SYSCLK_HIRES);
	} else
#endif
#ifdef TCE0
	if ((uintptr_t) tc == (uintptr_t) & TCE0) {
		tcIsEnable = sysclk_is_module_enable(SYSCLK_PORT_E, SYSCLK_TC0) && sysclk_is_module_enable(SYSCLK_PORT_E, SYSCLK_HIRES);
	} else
#endif
#ifdef TCE1
	if ((uintptr_t) tc == (uintptr_t) & TCE1) {
		tcIsEnable = sysclk_is_module_enable(SYSCLK_PORT_E, SYSCLK_TC1) && sysclk_is_module_enable(SYSCLK_PORT_E, SYSCLK_HIRES);
	} else
#endif
#ifdef TCF0
	if ((uintptr_t) tc == (uintptr_t) & TCF0) {
		tcIsEnable = sysclk_is_module_enable(SYSCLK_PORT_F, SYSCLK_TC0) && sysclk_is_module_enable(SYSCLK_PORT_F, SYSCLK_HIRES);
	} else
#endif
#ifdef TCF1
	if ((uintptr_t) tc == (uintptr_t) & TCF1) {
		tcIsEnable = sysclk_is_module_enable(SYSCLK_PORT_F, SYSCLK_TC1) && sysclk_is_module_enable(SYSCLK_PORT_F, SYSCLK_HIRES);
	} else
#endif
	{
	}
	return tcIsEnable;
}
typedef enum tc_cc_channel_t tc_ccx_channel_t;
static inline enum tc_cc_channel_t tc_channel_mask_to_index (enum tc_cc_channel_mask_enable_t channel_mask)
{
	switch (channel_mask)
	{
		case TC_CCAEN: return TC_CCA;
		case TC_CCBEN: return TC_CCB;
		case TC_CCCEN: return TC_CCC;
		case TC_CCDEN: return TC_CCD;
	}
	return 0;
}
static inline enum tc_cc_channel_mask_enable_t tc_channel_index_to_mask (enum tc_cc_channel_t channel_index)
{
	switch (channel_index)
	{
		case TC_CCA: return TC_CCAEN;
		case TC_CCB: return TC_CCBEN;
		case TC_CCC: return TC_CCCEN;
		case TC_CCD: return TC_CCDEN;
	}
	return 0;
}
static inline volatile void* ioport_pin_to_tc_base (ioport_pin_t _pin)
{
	ioport_port_t port_id = arch_ioport_pin_to_port_id(_pin) | 
						((arch_ioport_pin_to_index(_pin) > 3)? 0x10: 0x00);
	switch (port_id)
	{
		case IOPORT_PORTC: return &TCC0;
		case IOPORT_PORTC | 0x10: return &TCC1;
		case IOPORT_PORTD: return &TCD0;
		case IOPORT_PORTD | 0x10: return &TCD1;
		case IOPORT_PORTE: return &TCE0;
#ifdef TCE1
		case IOPORT_PORTE | 0x10: return &TCE1;
#endif // TCE1
#ifdef TCF0
		case IOPORT_PORTF: return &TCF0;
#endif // TCF0
#ifdef TCF1
		case IOPORT_PORTF | 0x10: return &TCF1;
#endif // TCF1
		default: return NULL;
	}
}
static inline enum tc_cc_channel_t ioport_pin_to_tc_channel (ioport_pin_t _pin)
{
	uint8_t pin_index = arch_ioport_pin_to_index(_pin);
	switch (pin_index)
	{
		case 1:
		case 5:
		return TC_CCB;
		case 2:
		return TC_CCC;
		case 3:
		return TC_CCD;
		//case 0:case 4:return TC_CCA;
		default:
		return TC_CCA;
	}
}
static inline ioport_pin_t ioport_tc_base_to_pin_base (volatile void *tc)
{
	if (tc == &TCC0) { return IOPORT_CREATE_PIN(PORTC, PIN0_bp); }
	else if (tc == &TCC1) { return IOPORT_CREATE_PIN(PORTC, PIN4_bp); }
	else if (tc == &TCD0) { return IOPORT_CREATE_PIN(PORTD, PIN0_bp); }
	else if (tc == &TCD1) { return IOPORT_CREATE_PIN(PORTD, PIN4_bp); }
	else if (tc == &TCE0) { return IOPORT_CREATE_PIN(PORTE, PIN0_bp); }
#ifdef TCE1
	else if (tc == &TCE1) { return IOPORT_CREATE_PIN(PORTE, PIN4_bp); }
#endif
#ifdef TCF0
	else if (tc == &TCF0) { return IOPORT_CREATE_PIN(PORTF, PIN0_bp); }
#endif
#ifdef TCF1
	else if (tc == &TCF1) { return IOPORT_CREATE_PIN(PORTF, PIN4_bp); }
#endif
	else { return 0; }
}
static inline void tc_pwm_set_duty_cycle_percent(ioport_pin_t pin, uint8_t value)
{
	//! value: 0-255
	volatile void *tc = NULL;
	enum tc_cc_channel_t tcch;
	tc = ioport_pin_to_tc_base(pin);
	if (tc == NULL) { return; }
	tcch = ioport_pin_to_tc_channel(pin);
	tc_write_cc_buffer(tc, tcch, 
					(uint16_t)(((uint32_t)tc_read_period(tc) *
					(uint32_t)value) / 255));
}

#endif // _TC_H_

#ifdef PWM_H

//#include <pwm.h>
static inline void pwm_write_duty_cycle_percent (enum tc_cc_channel_t channel, struct pwm_config *config,
		uint8_t duty_cycle_scale)
{
	Assert( duty_cycle_scale <= 100 );
	tc_write_cc_buffer(config->tc, channel,
			(uint16_t)(((uint32_t)config->period *
			(uint32_t)duty_cycle_scale) / 100));
}

#endif // PWM_H


#endif // ESF_XMEGA_TC_H_