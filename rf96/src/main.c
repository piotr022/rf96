/*
 * main.c
 *
 *  Created on: Mar 24, 2020
 *      Author: Piotr Lewandowski
 */
#include "stm32f3xx.h"
#include "gpio.h"
#include "printMsg.h"
#include "spi.h"
#include "rf96.h"
/* Private macro */
/* Private variables */
_Bool isUpdated_b = 0;
uint32_t delay =0;
uint8_t packet_u8[20][16];

/* Private function prototypes */
void clockInit();

/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	clockInit();
	spiInit();
	usart_1_enable();
	uint8_t regVal = 0;
uint32_t cnter =0;

gpio_pin_cfg(GPIOB, 0, GPIO_IN_PULL_UP); //interupt on pin D0
SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;	//przypisanie szyny B do EXTI0
	EXTI->IMR |= EXTI_IMR_MR0;
	EXTI->FTSR |= EXTI_FTSR_TR0;
/////////////////////////////////////for intterupt enf

uint32_t freq_u32 =0;
uint8_t rssi_u8 = 0;
spiWrite(0x06,0x64);
spiWrite(0x07,0xC0);
spiWrite(0x08,0x11);

freq_u32 = 	(spiRead(0x08) |
			(spiRead(0x07) << 8) |
			(spiRead(0x06) << 16) ) * (float)61.035;

spiWrite(RegOpMode, FSK_SLEEP_MODE);
//spiWrite(0x01,0b00101101);
spiWrite(RegPaConfig, 0b11111000);	//power select
spiWrite(RegBitRateMsb, 0x1A);
spiWrite(RegBitRateLsb, 0x0B);
spiWrite(RegRxBw, 0b00001110);
spiWrite(RegFdevLsb, 0x4E);
spiWrite(RegPreambleLsb, 40);
spiWrite(RegPaRamp, (0x9 | (0b01 << 5)));
spiWrite(RegDioMapping1, (0b01 << 6));	//enabling preamble detect pn DIO0
spiWrite(RegDioMapping2, 0b1);			//setting to preamble detect
//spiWrite(RegFifoThresh, 0x80);
//spiWrite(RegSyncConfig, (spiRead(RegSyncConfig) & 0b00111111));
//while(!(spiRead(RegOpMode) & FSK_SBY_MODE)){};
spiWrite(RegOpMode, FSK_SBY_MODE);

//spiWrite(LR_RegFifoAddrPtr, spiRead(LR_RegFifoTxBaseAddr) );	//0x80


spiWrite(RegOpMode,FSK_RX_MODE);
//SysTick_Config(8000000);
NVIC_EnableIRQ(EXTI0_IRQn);
	while (1)
	{
		cnter++;
		if(cnter > 100)
		{
		//	rssi_u8 = spiRead(0x1B);

			cnter=0;

		}

	}

  return 0;
}

void clockInit()
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN|RCC_AHBENR_GPIOCEN;

}


void SysTick_Handler(void)
{
	spiWrite(RegOpMode, FSK_SBY_MODE);
	uint8_t *packet_ptr = &packet_u8;
	packet_u8[0][0] = 0x86;
	packet_u8[0][1] = 0x35;
	packet_u8[0][2] = 0xF4;
	packet_u8[0][3] = 0x40;
	packet_u8[0][4] = 0x93;
	packet_u8[0][5] = 0xDF;
	packet_u8[0][6] = 0x1A;
	packet_u8[0][7] = 0x60;
		packet_u8[0][8] = 0x86;
		packet_u8[0][9] = 0x35;
		packet_u8[0][10] = 0xF4;
		packet_u8[0][11] = 0x40;
		packet_u8[0][12] = 0x93;
		packet_u8[0][13] = 0xDF;
		packet_u8[0][14] = 0x1A;
		packet_u8[0][15] = 0x60;
			packet_u8[1][0] = 0x86;
			packet_u8[1][1] = 0x35;
			packet_u8[1][2] = 0xF4;
			packet_u8[1][3] = 0x40;
			packet_u8[1][4] = 0x93;
			packet_u8[1][5] = 0xDF;
			packet_u8[1][6] = 0x1A;
			packet_u8[1][7] = 0x60;
				packet_u8[1][8] = 0x86;
				packet_u8[1][9] = 0x35;
				packet_u8[1][10] = 0xF4;
				packet_u8[1][11] = 0x40;
				packet_u8[1][12] = 0x93;
				packet_u8[1][13] = 0xDF;
				packet_u8[1][14] = 0x1A;
				packet_u8[1][15] = 0x60;
					packet_u8[1][0] = 0x86;
					packet_u8[2][1] = 0x35;
					packet_u8[2][2] = 0xF4;
					packet_u8[2][3] = 0x40;
					packet_u8[2][4] = 0x93;
					packet_u8[2][5] = 0xDF;
					packet_u8[2][6] = 0x1A;
					packet_u8[2][7] = 0x60;
						packet_u8[2][8] = 0x86;
						packet_u8[2][9] = 0x35;
						packet_u8[2][10] = 0xF4;
						packet_u8[2][11] = 0x40;
						packet_u8[2][12] = 0x93;
						packet_u8[2][13] = 0xDF;
						packet_u8[2][14] = 0x1A;
						packet_u8[2][15] = 0x60;

						packet_u8[3][0] = 0x86;
						packet_u8[3][1] = 0x35;
						packet_u8[3][2] = 0xF4;
						packet_u8[3][3] = 0x40;
						packet_u8[3][4] = 0x93;
						packet_u8[3][5] = 0xDF;
						packet_u8[3][6] = 0x1A;
						packet_u8[3][7] = 0x60;
	//spiWrite(RegOpMode,FSK_TX_MODE);
	for(int i=0; i <3; i++)
		for(int j=0; j <16; j++)
			spiWrite(RegFIFO,packet_u8[i][j]);

	spiWrite(RegOpMode,FSK_TX_MODE);
	uint8_t status = spiRead(RegIrqFlags2);
	uint32_t timout_u32 = 0;
	while(!(status & (0b1<<3))){
		__DSB();__DSB();__DSB();__DSB();
		status = spiRead(RegIrqFlags2);
		if(timout_u32 > 5000UL)
			break;
		timout_u32++;
	}
	//spiWrite(0x12, (0b0 << 2));
	//
	//spiWrite(RegOpMode, FSK_SBY_MODE);
	spiWrite(RegOpMode, FSK_SLEEP_MODE);
}

__attribute__ ((interrupt)) void EXTI0_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR0){
		EXTI->PR = EXTI_PR_PR0;
		printMsg("RSSI: %i, IRQ1 %x, IRQ2 %x\n",(-1*spiRead(0x11)/2),spiRead(RegIrqFlags1),spiRead(RegIrqFlags2));
	}
}

