/*
 * ESF-Subcodes.h
 *
 * Created: 01/10/2017 08:17:54 AM
 *  Author: MohammadReza Askari reza69.askari@gmail.com
 */

/*
 * if you going to use ESF library, include this lib in top of main.c (after asf.h and init.h)
 * #Mind to add Atmel-ESF Directories in your project
 * And #Mind to include any used esf-library in init.h
#include <ESF-Subcodes.h>
 */


#ifndef ESF_SUBCODES_H_
#define ESF_SUBCODES_H_

//! >>>>>>>>>>>>>>>> Boards <<<<<<<<<<<<<<<<
//#include "esf_io.h"
#ifdef ESF_IO_H_
//#	include "esf_io.c"
#endif // ESF_IO_H_

#ifndef DONT_USE_BOARD_GENERATOR
# include "Boards/EraBoards-Init.h"
# ifdef UserBoardName
#  include "Boards/EraBoards-Main.c"
//#  include "Boards/EraBoards-Init.c"
#  if (UserBoardName != JustSimulate)
#   include "Boards/CommandCenter.c"
#  endif // (UserBoardName == JustSimulate)
# endif
#endif

//#if XMEGA
//#elif MEGA
//#elif UC3
//#elif SAM
//#endif

//! >>>>>>>>>>>>>>>> Common <<<<<<<<<<<<<<<<
//! #Note: #OnError: 'ioport_pin_t' undeclared: make sure 'GPIO(service)' is added by 'ASF-Wizard'

//! if used #include <esf_common.h>

//! if used #include <Services/esf_print.h>
#ifdef ESF_PRINT_H_
# include "Services/esf_print.c"
#endif // ESF_PRINT_H_

//! if used #include <Services/esf_string.h>
#ifdef ESF_STRING_H_
//# include "Services/esf_string.c"
#endif // ESF_STRING_H_

//! if used #include <Services/esf_math.h>
#ifdef ESF_MATH_H_
# include "Services/esf_math.c"
#endif // ESF_MATH_H_

#if (defined CONF_RTC_H && !defined CONFIG_RTC_SOURCE)
#error "set clock source for RTC in conf_clock.h"
#endif // CONF_RTC_H

//! >>>>>>>>>>>>>>>> ST-Family <<<<<<<<<<<<<<<<
#if STM8

//! Load other subcodes
#include <../drivers-include.h>

//! if used #include <STM8/ioport/ioport_compat.h>
# ifdef IOPORT_COMPAT_H_
# include "STM8/ioport/ioport_compat.c"
# endif // IOPORT_COMPAT_H_

//! if used #include <STM8/esf_stm8_assert.h>
# ifdef ESF_STM8_ASSERT_H_
# include "STM8/esf_stm8_assert.c"
# endif // ESF_STM8_ASSERT_H_

#endif // STM8

//! >>>>>>>>>>>>>>>> MEGA <<<<<<<<<<<<<<<<
//#if MEGA
#if AVR8_PART_IS_DEFINED(ATmega8)//MEGA //! To check other MEGA devices; just checking tested device

//! #Note: #OnError: insert below lines in '[Project]\src\ASF\common\services\ioport\mega\ioport.h' (Line 79; after 'IOPORT_BASE_ADDRESS_H 0x100')
/*
//! >>>>>>>>>>>>>>>> mega8 ioport currect
#if AVR8_PART_IS_DEFINED(ATmega8)
#undef IOPORT_BASE_ADDRESS
#undef IOPORT_PORT_OFFSET
#undef IOPORT_PORTB
#undef IOPORT_PORTC
#undef IOPORT_PORTD
#define IOPORT_BASE_ADDRESS 0x30
#define IOPORT_PORT_OFFSET  0x03
#define IOPORT_PORTB    2
#define IOPORT_PORTC    1
#define IOPORT_PORTD    0
#endif // ATmega8
*/

# ifdef ATMEGA_CAN_BUS_H_
# include "ATmega_CAN_Bus.c"
# endif // ATMEGA_CAN_BUS_H_

# ifdef ATMEGA_EEPROM_H_
# include "ATmega_EEPROM.c"
# endif // ATMEGA_EEPROM_H_

# ifdef TC_TIMEOUT_H
# include "ATmega_Timeout.c"
# endif // TC_TIMEOUT_H

# ifdef ATMEGA8_TC_H_
# include "ATmega8_TC.c"
# endif // ATMEGA8_TC_H_

# ifdef ATMEGA8_ADC_H_
# include "ATmega8_ADC.c"
# endif // ATMEGA8_ADC_H_

# ifdef ESF_MEGA_USART_H_
# include "ATmega/esf_mega_usart.c"
# endif // ESF_MEGA_USART_H_

#endif // MEGA

//! >>>>>>>>>>>>>>>> XMEGA <<<<<<<<<<<<<<<<
#if XMEGA
# include "MPUs/atxmega/esf_atxmega_io.h"

# ifdef ESF_XMEGA_USART_H_
//# include "ATxmega/esf_xmega_usart.c"
# endif // ESF_XMEGA_USART_H_

# ifdef MRA_AES_H_
# include "ATxmega/MrA_AES.c"
# endif // MRA_AES_H_

//! if used #include <esf_dma.h>
# ifdef ESF_DMA_H_
# include "MPUs/atxmega/include/esf_xmega_dma.c"
# endif // ESF_DMA_H_

//! if used #include <esf_xrgb.h>
# ifdef ESF_XRGB_H_
# include "ATxmega/esf_xrgb.c"
# endif // ESF_XRGB_H_

#endif // XMEGA

//! >>>>>>>>>>>>>>>> Components <<<<<<<<<<<<<<<<
//#include <components/ESF-Components.c>//""
//#include "Component/ESF-Components.h"

//! >>>>>>>>>>>>>>>> Services <<<<<<<<<<<<<<<<
//#include "Services/ESF-Services.h"

//! >>>>>>>>>>>>>>>> Utilities <<<<<<<<<<<<<<<<
//#include "Utilities/ESF-Utilities.h"


#endif /* ESF_SUBCODES_H_ */