/*
 * esf_dma.c
 *
 * Created: 12/18/2017 11:00:13
 *  Author: MohammadReza Askari reza69.askari@gmail.com
 */

#include "esf_xmega_dma.h"
#include <status_codes.h>
#include <dma.h>

void dma_init(void)
{
	static bool _IsInitialized = false;
	if (!_IsInitialized)
	{
		LOG_FUNCTION();
		_IsInitialized = true;
		dma_enable();
	}
}

//static dma_channel_config_t _channels_config[DMA_NUMBER_OF_CHANNELS];
//void dma_channel_capture_config (dma_channel_num_t ch_num)
//{
//	dma_channel_read_config(ch_num, &_channels_config[ch_num]);
//}
//void dma_channel_load_config (dma_channel_num_t ch_num){...}
//size_t dma_channel_read_size (dma_channel_num_t ch_num)
//{
//	DMA_CH_t *ch = dma_get_channel_address_from_num(ch_num);
//#ifdef CONFIG_HAVE_HUGEMEM
//#error "NotImplementedCodeException"
//#endif
//	return ((ch->DESTADDR1 << 8) + (ch->DESTADDR0)) - _channels_config[ch_num].destaddr16;
//}

status_code_t dma_channel_reload_config (dma_channel_num_t ch_num, dma_channel_config_t* conf)
{
	bool isChannelEnable = dma_channel_is_enabled(ch_num);
	if (isChannelEnable) { dma_channel_disable(ch_num); }
	while (dma_channel_is_busy(ch_num)) {
		//! Wait
	}
	DMA_CH_t *ch = dma_get_channel_address_from_num(ch_num);
	//! Clear channel error flag (occurred when disable channel while the channel is busy)
	ch->CTRLB |= DMA_CH_ERRIF_bm;
	
	dma_channel_write_config(ch_num, conf);//[ch_num]
	if (isChannelEnable) { dma_channel_enable(ch_num); }
	
	return STATUS_OK;
}

bool dma_channel_is_pending(dma_channel_num_t num)
{
	uint8_t busy_pending = DMA.STATUS;
	busy_pending &= (1 << (num + DMA_CH0PEND_bp));
	
	return busy_pending ? true : false;
}

status_code_t dma_reload_destination(U8 dma_channel, void* buffer, size_t buffer_size)
{
	dma_channel_config_t _cnf;
	//! >>>>>>> Reload
	irqflags_t iflags = cpu_irq_save();
	dma_channel_read_config(dma_channel, &_cnf);
	bool _wasEnable = dma_channel_is_enabled(dma_channel);
	if (_wasEnable) { dma_channel_disable(dma_channel); }
	while (dma_channel_is_busy(dma_channel)) {
		nop();
	}
	dma_channel_set_transfer_count(&_cnf, buffer_size);
	dma_channel_set_destination_address(&_cnf, (uint16_t)(uintptr_t)buffer);
	dma_channel_write_config(dma_channel, &_cnf);
	if (_wasEnable) { dma_channel_enable(dma_channel); }
	
	////! Wait until DMA-Channel is *PENDING*
	////! DON’T use Busy, just wait for pending to true setter on Count & Destination
	//U16 timeout;
	//WaitUntil_Action(dma_channel_is_pending(dma_channel), timeout, 1000);
	//if (!timeout) { return ERR_TIMEOUT; }
	////! Set Block count; size-of(GUI-BufferSize)
	//dma_channel_write_transfer_count(dma_channel, buffer_size);
	////Set Destination; Into GUI-Buffer[]
	//dma_channel_write_destination(dma_channel, (uint16_t)(uintptr_t)buffer);
	
	cpu_irq_restore(iflags);
	return STATUS_OK;
}
