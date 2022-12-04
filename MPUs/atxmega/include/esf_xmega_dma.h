/*
 * esf_dma.h
 *
 * Created: 12/18/2017 10:59:01
 *  Author: MohammadReza Askari reza69.askari@gmail.com
 * @git-url: https://github.com/reza69askari/ESF
 */
/*
 * REPEAT	SINGLE	REPCNT	Trigger		Flag set after	Channel disabled after
 * 0		0		0		Block		1 block			1 block
 * 0		0		1		Block		1 block			1 block
 * 0		0		n > 1	Block		1 block			1 block
 * 0		1		0		BURSTLEN	1 block			1 block
 * 0		1		1		BURSTLEN	1 block			1 block
 * 0		1		n > 1	BURSTLEN	1 block			1 block	:if REPEAT=0, REPCNT is count down but not effect on transfer
 * 1		0		0		Block		Each block		Never	//! 'Each block' is wrong, in descrip of 'REPCNT == 0' (and real test) channel is never disable
 * 1		0		1		Transaction	1 block			1 block	//! Same as first row
 * 1		0		n > 1	Transaction	n blocks		n blocks
 * 1		1		0		BURSTLEN	Each block		Never
 * 1		1		1		BURSTLEN	1 block			1 block
 * 1		1		n > 1	BURSTLEN	n blocks		n blocks
 */
//! #Mind: Repeat is cleared by hardware at the beginning of the last block transfer (in REPCNT).
//! #Mind: The REPCNT register should be configured before setting the REPEAT bit.

#ifndef ESF_ATXMEAG_IO_H_
#  error "Include <MPUs/esf_io.h> instead of this file."
#endif

#if 0
#include "esf_xmega_dma.c"
#endif

#ifndef ESF_DMA_H_
#define ESF_DMA_H_

#include <dma.h>
#include <esf_compiler.h>

void dma_init(void);

typedef struct dma_channel_config dma_channel_config_t;

void dma_channel_capture_config (dma_channel_num_t ch_num);
void dma_channel_load_config (dma_channel_num_t ch_num);
size_t dma_channel_read_size (dma_channel_num_t ch_num);

bool dma_channel_is_pending(dma_channel_num_t num);
status_code_t dma_reload_destination(U8 dma_channel, void* buffer, size_t buffer_size);
status_code_t dma_channel_reload_config (dma_channel_num_t ch_num, dma_channel_config_t* conf);


#endif /* ESF_DMA_H_ */