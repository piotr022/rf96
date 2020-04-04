/*
 * rf96.c
 *
 *  Created on: Mar 26, 2020
 *      Author: Piotr
 */

#include <rf96.h>
#include "stm32f3xx.h"
#include "gpio.h"
#include "printMsg.h"
#include "spi.h"




void rf_set_params(rf96_config * _conf)
{
	uint32_t val = (_conf->freqency_u32) / F_STEP;
	spiWrite(RegFreqLsb, val & 0xFF);
	spiWrite(RegFreqMid, (val >> 8) & 0xFF);
	spiWrite(RegFreqMsb, (val >> 16) & 0xFF);
}



uint8_t rf96();
