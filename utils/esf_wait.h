/*
 * esf_wait.h
 *
 * @Created: 7/28/2022 20:24:33
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */

#ifndef ESF_Wait_Helper_H_
#define ESF_Wait_Helper_H_

#include <asf.h>
#include <util/delay.h>
#include <esf_compiler.h>

typedef enum IO_Delay_Set_Enum
{
	IO_WithoutDelay_gc	= 0,
	IO_ShortestWait_gc	= 10,
	IO_ShortWait_gc		= 50,
	IO_Wait_gc			= 100,
	IO_LongWait_gc		= 500,
} IO_Delay_t; //! in milliseconds (mS)

/* Wait in mS
 * 
 * can be use by 'IO_Delay_t' */
static inline void Wait(U16 delay)
{
	while (delay--) { asm("nop"); _delay_ms(1); }
}

/**
 * \brief wait for condition be OK (wait until condition is NOT 'STATUS_OK')
 * 
 * \param condition
 * \param timeout_us
 * 
 * \return bool; 'T' if time out happend, else 'F'
 */
static inline status_code_t WaitUntil (event_t condition, uint16_t timeout_us)
{
	do {
		asm("nop");
		_delay_us(1);
	} while(condition() != STATUS_OK && timeout_us--);
	if (!timeout_us) { return ERR_TIMEOUT; }
	return STATUS_OK;
}

/* Wait until 'func(...) != STATUS_OK' or timeout
 * 
 * #Options: after this method, do follow code if you want
 * <code>
	U16 _timeout;
	WaitUntil_Action(data_is_ready(), _timeout, 2000);
	if (!timeout_us) { return ERR_TIMEOUT; }
	return STATUS_OK;
 * </code>
 */
#define WaitUntil_Action(func, timeout_us, timeout_initialize)	do { \
		timeout_us = timeout_initialize; \
		do { asm("nop"); _delay_us(1); } while(func != STATUS_OK && timeout_us--); \
	} while (0);

/**
 * \brief wait for condition be OK (wait until condition is NOT 'STATUS_OK')
 * 
 * \param condition
 * \param timeout_ms
 * 
 * \return bool; 'T' if time out happend, else 'F'
 */
static inline status_code_t WaitUntil_ms (event_t condition, uint16_t timeout_ms)
{
	do {
		asm("nop");
		_delay_ms(1);
	} while(condition() != STATUS_OK && timeout_ms--);
	if (!timeout_ms) { return ERR_TIMEOUT; }
	return STATUS_OK;
}

#ifdef IOPORT_H

//! #Note: For 'ioport_pin_t' add 'GPIO(service)' in 'ASF-Wizard'

/* 
 * \param bHigh: 'T' wait for pin be HIGH (wait while pin is low), 'F' wait for pin be LOW (wait while pin is high)
 * */
static inline status_code_t WaitForPin_Level (ioport_pin_t pin, bool reachHigh, uint16_t timeout_us)
{
	do {
		asm("nop");
		_delay_us(1);
	} while(reachHigh == gpio_pin_is_low(pin) && timeout_us--);
	if (!timeout_us) { return ERR_TIMEOUT; }
	return STATUS_OK;
}
/* wait for pin be HIGH (wait while pin is low) */
static inline status_code_t WaitForPin_HI (ioport_pin_t pin, uint16_t timeout_us)
{
	return WaitForPin_Level(pin, HIGH, timeout_us);
}
/* wait for pin be LOW (wait while pin is high) */
static inline status_code_t WaitForPin_LO (ioport_pin_t pin, uint16_t timeout_us)
{
	return WaitForPin_Level(pin, LOW, timeout_us);
}

#endif // IOPORT_H


#endif // ESF_Wait_Helper_H_