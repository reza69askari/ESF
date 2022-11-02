/*
 * AD7190.c
 *
 * @Created: 4/13/2022 2:44:54
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */


//! >>>>>>> AD7190-Component-Library
#include <conf_board.h>
#include "ad7190.h"
#include "ad7190_spi.h"
#include <utils/esf_wait.h>

//! >>>>>>>>>>>>>>>> Checker >>>>>>>>>>>>>>>>
#pragma region Checker

//! >>>>>>> Requires-Check

#ifndef AD7190_PORT_id
#error "define AD7190 Port-id: Ex: #define AD7190_PORT_id IOPORT_PORTC"
#endif
#if CONFIG_SPI_MASTER_DUMMY != 0x00
#error "Wrong conf, dummy must be 0x00 for AD7190"
#endif

//! >>>>>>> Options-Check

#ifndef AD7190_CS_pin
#	if (AD7190_PORT_id == IOPORT_PORTC)
#		define AD7190_CS_pin					IOPORT_CREATE_PIN(PORTC, SPI_CS_pin_bp)
#	elif (AD7190_PORT_id == IOPORT_PORTD)
#		define AD7190_CS_pin					IOPORT_CREATE_PIN(PORTD, SPI_CS_pin_bp)
#	elif (AD7190_PORT_id == IOPORT_PORTE)
#		define AD7190_CS_pin					IOPORT_CREATE_PIN(PORTE, SPI_CS_pin_bp)
#	elif (AD7190_PORT_id == IOPORT_PORTF)
#		define AD7190_CS_pin					IOPORT_CREATE_PIN(PORTF, SPI_CS_pin_bp)
#	else
#		error "Not Defined"
#	endif
#endif // AD7190_CS_pin

#ifndef Config_AD7190_ClockHz
#	warning "4920Hz Clock is selected for code!"
#	define Config_AD7190_ClockHz	4920000UL
#	if ((defined Config_AD7190_ClockSource) && (Config_AD7190_ClockSource == AD7190_CLKSEL_Int4920Hz_SQW))
#		define AD7190_ClockSource	AD7190_CLKSEL_Int4920Hz_SQW
#	else
#		define AD7190_ClockSource	AD7190_CLKSEL_Int4920Hz //! Default
#	endif
#else
#	if ((defined Config_AD7190_ClockSource) && (Config_AD7190_ClockSource == AD7190_CLKSEL_ExtXTAL))
#		define AD7190_ClockSource	AD7190_CLKSEL_ExtXTAL
#	else
#		define AD7190_ClockSource	AD7190_CLKSEL_ExtClock
#	endif
#endif // Config_AD7190_ClockHz

#pragma endregion Checker
//! <<<<<<<<<<<<<<<< .Checker <<<<<<<<<<<<<<<<

/* Will set to last requested value on 'AD7190_Init(...)'
 * */
static U32 __AD7190_DefaultConfig = 0x000117;
/* Will set to last requested value on 'AD7190_Init(...)'
 * */
static U32 __AD7190_DefaultMode = 0x080060;

static U32 __AD7190_LastConfig = 0x000117;
static U32 __AD7190_LastMode = 0x080060;
static PORT_t *_AD7190_PORT = NULL;
static ioport_pin_t _AD7190_DRDY_pin;
static U32 __AD7190_Timeout = 24000;
static Union32 __ad7190_mem;

static void _AD7190_DefaultRegisters(void)
{
	//! Default Config-Register= 0x000117: Chop-Off, Ref-IN1, CH-IN 1&2, Burn-off, RefDetect-Off, Buffer-ON, Bipolar, Gain-128 (±39.06 mV)
	__AD7190_LastConfig = 0x000117;
	LogParameter(__AD7190_LastConfig);
	//! Default Mode-Register= 0x080060: Continuous, TransmitStatus-Off, Int-4.92MHz, SINC4, Parity-Off, Single-Off, REJ60-Off, FS=96(60h) (ODR=50.048828125Hz)
	__AD7190_LastMode = 0x080060;
	LogParameter(__AD7190_LastMode);
	//! '24000' Startup Output Data Rate is 50.04Hz
	__AD7190_Timeout = 34000;
	LogParameter(__AD7190_Timeout);
}

static U16 _AD7190_CalculateDataRate(U16 clock)
{
	U16 _ODR; /* Output Data Rate */
	//__AD7190_Timeout = 1000000 / clock;
	//__AD7190_Timeout += __AD7190_Timeout / (100/20); //! +20%
	//if (__AD7190_Timeout < 250) { __AD7190_Timeout = 250; }
	LogParameter(__AD7190_Timeout);
	
	if (true)//(!(__AD7190_LastConfig & AD7190_CONFIG_CHOP_bm))
	{	//! chop is off
		_ODR = ((U32)Config_AD7190_ClockHz /16 /64) / clock;
	}
	else
	{	//! chop is on
		_ODR = ((U32)Config_AD7190_ClockHz /16 /64) / (((__AD7190_LastMode & AD7190_MODE_SINC3_bm) ? 3 : 4) * clock);
	}
	Assert(_ODR <= AD7190_MODE_PER_max);
	if (_ODR < AD7190_MODE_PER_min) { _ODR = AD7190_MODE_PER_min; }
	else if (_ODR > AD7190_MODE_PER_max) { _ODR = AD7190_MODE_PER_max; }
	
	return (_ODR << AD7190_MODE_PER_gp) & AD7190_MODE_PER_gm;
}

void AD7190_Init(U16 samples
	, AD7190_Refrence_t refrence
	, AD7190_Channels_t inputs
	, AD7190_Polarity_t polarity
	, AD7190_Gain_t gain
	, AD7190_Operation_t operation
	, AD7190_Options_t options)
{
	LOG_FUNCTION();
	
	//! > Config IO
	
	gpio_configure_pin(AD7190_CS_pin, IO_Output | IO_InitHigh);
	gpio_configure_pin(AD7190_Sync_pin, IO_Output | IOPORT_INV_ENABLED | IO_InitLow);
	_AD7190_PORT = ioport_id_pin_to_port(AD7190_PORT_id);
	_AD7190_DRDY_pin = ioport_create_pin(AD7190_PORT_id, SPI_MISO_pin_bp);
	LogParameter(_AD7190_DRDY_pin);
	ad7190_spi_init();
	gpio_configure_pin(_AD7190_DRDY_pin, IO_Input | IO_InvertDisable | IO_SenseFalling);
#ifdef Config_AD7190_UseIRQ
	pmic_disable_level(PMIC_LVL_HIGH);
	_AD7190_PORT->INTCTRL &= ~PORT_INT1LVL_gm;
	_AD7190_PORT->INTCTRL |= PORT_INT1LVL_HI_gc;
	_AD7190_PORT->INT1MASK = SPI_MISO_pin_bm; //! MISO:Pin6
#endif // AD7190_DRDY_vect
	
	//! > Default configuration of AD7190-Chip
	
	//TODO: #ToDo Chop, by 'xxDataRate(...)'
	//TODO: #ToDo what about options ???? SINC3 and rest ( //TODO: #ToDo maybe SINC3 by 'xxDataRate(...)' too !!?? )
	
	_AD7190_DefaultRegisters();
	AD7190_Active();
	AD7190_ExecuteCommand(AD7190_CMD_Reset);
	AD7190_ExecuteCommand(AD7190_CMD_Sync);
	__AD7190_DefaultConfig = (refrence & AD7190_CONFIG_REFDET_bm)
		| (inputs & AD7190_CONFIG_CHx_gm)
		| (polarity & AD7190_CONFIG_POL_bm)
		| (gain & AD7190_CONFIG_GAIN_gm);
	LogParameter(__AD7190_DefaultConfig);
	AD7190_Write(AD7190_REG_CONFIG_gc, __AD7190_DefaultConfig);
	__AD7190_DefaultMode = (operation & AD7190_MODE_OP_gm)
		| AD7190_ClockSource
		| (options & AD7190_Options_gm)
		| (U32)_AD7190_CalculateDataRate(samples);
	LogParameter(__AD7190_DefaultMode);
	AD7190_Write(AD7190_REG_MODE_gc, __AD7190_DefaultMode);
	AD7190_Deactive();
}

status_code_t AD7190_WaitDRDY(void)
{
	status_code_t _s;
	LOG_ASSERT(!gpio_pin_is_low(AD7190_Device->id), IOException);
	
	_s = WaitForPin_LO(_AD7190_DRDY_pin, __AD7190_Timeout);
	if (_s == ERR_TIMEOUT) { LogWarning(STR(TimeoutException) " #0101"); }
	return _s;
}
status_code_t AD7190_WaitDRDY_Pulse(U32 value)
{
	status_code_t _s;
	LOG_ASSERT(!gpio_pin_is_low(AD7190_Device->id), IOException);
	/* RDY-Signal is not generating, if chip be on Idle-mode (or not activated). */
	if (__AD7190_LastMode & AD7190_OPS_Idle_gc) { return STATUS_OK; }
	if (value == 0) { value = __AD7190_Timeout; }
	
	if (gpio_pin_is_low(_AD7190_DRDY_pin)) { 
		_s = WaitForPin_HI(_AD7190_DRDY_pin, value);
		if (_s == ERR_TIMEOUT) { LogWarning(STR(TimeoutException) " #0102"); }
	}
	_s = WaitForPin_LO(_AD7190_DRDY_pin, value);
	if (_s == ERR_TIMEOUT) { LogWarning(STR(TimeoutException) " #0103"); }
	return _s;
}

void AD7190_WriteChannels(AD7190_Channels_t chs, bool exeSync)
{
	LOG_FUNCTION();
	LogParameter(chs);
	
	__AD7190_LastConfig &= ~AD7190_CONFIG_CHx_gm;
	__AD7190_LastConfig |= (chs & AD7190_CONFIG_CHx_gm);
	AD7190_Write(AD7190_REG_CONFIG_gc, __AD7190_LastConfig);
	if (exeSync) { AD7190_ExecuteCommand(AD7190_CMD_Sync); }
}

void AD7190_WriteSampleRate(U16 clock)
{
	LOG_FUNCTION();
	LogParameter(clock);
	
	U16 clockPER = _AD7190_CalculateDataRate(clock);
	__AD7190_LastMode &= ~AD7190_MODE_PER_gm;
	__AD7190_LastMode |= clockPER;
	AD7190_Write(AD7190_REG_MODE_gc, __AD7190_LastMode);
}
void AD7190_WriteOperationMode(AD7190_Operation_t opm)
{
	LOG_FUNCTION();
	
	__AD7190_LastMode &= ~AD7190_MODE_OP_gm;
	__AD7190_LastMode |= (opm & AD7190_MODE_OP_gm);
	AD7190_Write(AD7190_REG_MODE_gc, __AD7190_LastMode);
}

void AD7190_ExecuteCommand(AD7190_Commands_t cmd)
{
	LOG_FUNCTION();
	LogParameter(cmd);
	LOG_ASSERT(!gpio_pin_is_low(AD7190_Device->id), IOException);
	
	//AD7190_WaitDRDY();
	AD7190_WaitDRDY_Pulse(0);
	
	switch (cmd)
	{
		case AD7190_CMD_Reset:
		//! write 10-empty byte & Wait(100mS)
		AD7190_Reset();
		Wait(100);
		_AD7190_DefaultRegisters();
		break;
		
		case AD7190_CMD_Sync:
		//! hold Sync-pin for 200mS & Wait(10mS)
		gpio_set_pin_high(AD7190_Sync_pin);
		Wait(200);
		gpio_set_pin_low(AD7190_Sync_pin);
		Wait(10);
		break;
		
		case AD7190_CMD_Reload:
		AD7190_ExecuteCommand(AD7190_CMD_Reset);
		AD7190_ExecuteCommand(AD7190_CMD_Sync);
		//! Load initialize value (captured on 'AD7190_Init(...)')
		LogParameter(__AD7190_DefaultConfig);
		AD7190_Write(AD7190_REG_CONFIG_gc, __AD7190_DefaultConfig);
		LogParameter(__AD7190_DefaultMode);
		AD7190_Write(AD7190_REG_MODE_gc, __AD7190_DefaultMode);
		//TODO: #ToDo$Important reload-cmd !!?? *** using on reset-cmd or reload-cmd !!?? mind: update 'Channels.c' for this ***
		break;
		
		case AD7190_CMD_ContinuousRead:
		if ((__AD7190_LastMode & AD7190_MODE_OP_gm) != AD7190_OPS_ContinuousConversion_gc) {
			AD7190_WriteOperationMode(AD7190_OPS_ContinuousConversion_gc);
		}
		case AD7190_CMD_ExitRead:
		ad7190_spi_write_byte(cmd);
		break;
		
	}
}

static U8 _GetRegSize(AD7190_Registers_t reg)
{
	switch (reg & (~AD7190_Registers_Read_bm))
	{
		case AD7190_REG_COMM_gc: //// case AD7190_REG_STATUS_gc:
		//case AD7190_REG_STATUS_DRDY_gc:
		case AD7190_REG_ID_gc: case AD7190_REG_GPOCON_gc:
		return 1;
		//case AD7190_REG_MODE_gc: case AD7190_REG_CONFIG_gc:
		//case AD7190_REG_DATA_gc: case AD7190_REG_OFFSET_gc:
		//case AD7190_REG_FULLSCALE_gc:
		default:
		return 3;
	}
}

void AD7190_Write(AD7190_Registers_t reg, U32 value)
{
	LOG_FUNCTION();
	LogParameter(reg);
	LogParameter(value);
	LOG_ASSERT(!gpio_pin_is_low(AD7190_Device->id), IOException);
	
	AD7190_WaitDRDY_Pulse(0);
	__ad7190_mem.u32 = value;
	__ad7190_mem.u8[3] = reg & (~AD7190_Registers_Read_bm);
	__ad7190_mem.u32 = Swap32(__ad7190_mem.u32);
	LogParameter(__ad7190_mem.u32);
	ad7190_spi_write_packet(__ad7190_mem.u8, (_GetRegSize(reg) +1));
	
	switch (reg)
	{
		case AD7190_REG_MODE_gc:
			//! RDY goes high when the calibration(s) is initiated and returns low when the calibration is complete.
			if (value & AD7190_CAL_InternalZero_gc) {
				LogTrace("Calib...");
				__AD7190_LastMode = value; //! keep value different than idle for DRDY-pulse
				AD7190_WaitDRDY_Pulse(2000000);
				//! After calibration, mode is setting to idle (by chip)
				value = (value & ~AD7190_MODE_OP_gm) | AD7190_OPS_Idle_gc;
			}
			__AD7190_LastMode = value;
			break;
		case AD7190_REG_CONFIG_gc:
			__AD7190_LastConfig = value;
			break;
		default:
			break;
	}
	
	//AD7190_WaitDRDY_Pulse();
	U32* _b = AD7190_Read32(reg);
	LogParameter(*_b);
	LogIf_lvl(Error, *_b != value, "Validation failed!");
}

U8* AD7190_Read(AD7190_Registers_t reg)
{
	LOG_FUNCTION();
	LOG_ASSERT(!gpio_pin_is_low(AD7190_Device->id), IOException);
	__ad7190_mem.u32 = 0;
	//AD7190_WaitDRDY();
	AD7190_WaitDRDY_Pulse(0);
	reg = (reg |AD7190_Registers_Read_bm);
	LogParameter(reg);
	ad7190_spi_write_byte(reg);
	ad7190_spi_read_packet(__ad7190_mem.u8, _GetRegSize(reg));
	__ad7190_mem.u32 = Swap32(__ad7190_mem.u32) >> 8;
	return __ad7190_mem.u8;
}

U8* AD7190_Get24bitData(void)
{
	//LOG_ASSERT(!gpio_pin_is_low(AD7190_Device->id), IOException);
	ad7190_spi_read_packet(__ad7190_mem.u8, AD7190_ReadSize);
	return __ad7190_mem.u8;
}

//TODO: #ToDo read without IRQ, wait for RDY & ...

//! >>>>>>> Interrupt
#ifdef Config_AD7190_UseIRQ

/* AD7190: data is ready 
 * 
 * IRQ: AD7190-DRDY - IRQ-HI-LVL
 * */
__always_inline static void ChannelReadyInterrupt(void)
{
	//! Capture/Disable/Restore pin interrupt (prevent stack overflow on spi-read)
	U8 mask = _AD7190_PORT->INT1MASK;
	_AD7190_PORT->INT1MASK = 0;
	//! Chip read 24-bit
	ad7190_spi_read_packet(__ad7190_mem.u8, AD7190_ReadSize);//AD7190_Get24bitData();
	_AD7190_PORT->INT1MASK = mask;
}
#if (AD7190_PORT_id == IOPORT_PORTC)
	ISR(PORTC_INT1_vect) { ChannelReadyInterrupt(); }
#elif (AD7190_PORT_id == IOPORT_PORTD)
	ISR(PORTD_INT1_vect) { ChannelReadyInterrupt(); }
#elif (AD7190_PORT_id == IOPORT_PORTE)
	ISR(PORTE_INT1_vect) { ChannelReadyInterrupt(); }
#elif (AD7190_PORT_id == IOPORT_PORTF)
	ISR(PORTF_INT1_vect) { ChannelReadyInterrupt(); }
#else
#	error "Not Defined"
#endif

#endif // Config_AD7190_UseIRQ
