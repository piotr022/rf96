/*
 * spi.c
 *
 *  Created on: Mar 24, 2020
 *      Author: Piotr Lewandowski
 */

#include "stm32f3xx.h"
#include "gpio.h"
#include "spi.h"
//////////////////////////////SPI//////////////////////////////////////////

void spiInit()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN;		//en clock for gpio a and spi1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	__DSB(); __DSB();

	gpio_pin_cfg(GPIOB,13,GPIO_OUT_PP_50MHz);		//PB13 is
	GPIOB->ODR |= GPIO_ODR_13;						//soft NSS pin

	GPIOA->MODER |= GPIO_MODER_MODER4_1|GPIO_MODER_MODER5_1|	//setting alternate fx
					GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1;	//for PA 4,5,6,7
	GPIOA->AFR[0] = (0b0101 << 16) | (0b0101 << 20) |
					(0b0101 << 24) | (0b0101 << 28);		//setting AF5(0101) for this pins

	SPI1->CR1 |= SPI_CR1_BR_1|SPI_CR1_BR_0				//PI_CR1_BR_0|SPI_CR1_BR_2
				|SPI_CR1_MSTR|SPI_CR1_SSM; 	//setting baudrate to Fpclk/128 + master mode
	SPI1->CR2 |= SPI_CR2_SSOE|SPI_CR2_DS;	//rx buffer not empty interrupt enable + hw ss enable
											//+ nss pulse, cont. transfer + 16data	interrupt for rx ne SPI_CR2_RXNEIE
}

uint8_t spiWrite(uint8_t reg_u8, uint8_t val_u8)
{
	uint8_t recTrash_u8;
	if(reg_u8 & (1 << 7))
		return 2;						//error, reg_u8 is bigger than 7 bits
	reg_u8 |= (0b1 << 7);				//msb = 1 if reg write operation
	GPIOB->ODR &= ~GPIO_ODR_13;			//nss low
	SPI1->CR1 |= SPI_CR1_SPE;			//enabling spi
	SPI1->DR = val_u8 | (reg_u8 << 8);	//if MSB is 1, rf96 is in register write mode
	                                    //sending value to be set in register

	while(SPI1->SR & SPI_SR_BSY){};		//waiting until all data is transmitted
	while(SPI1->SR & SPI_SR_RXNE)
			recTrash_u8 = SPI1->DR;		//clearing buffer
	SPI1->CR1 &= ~SPI_CR1_SPE;			//disabling spi
	GPIOB->ODR |= GPIO_ODR_13;			//nss high
	return 0;
}

uint16_t spiRead(uint8_t reg_u8)
{

	if(reg_u8 & (1 << 7))
			return 0xFF;	//error, reg_u8 is bigger than 7 bits

	uint8_t recTrash_u8;
	GPIOB->ODR &= ~GPIO_ODR_13;					//nss low
	SPI1->CR1 |= SPI_CR1_SPE;					//enabling spi
	while(SPI1->SR & SPI_SR_RXNE)
			recTrash_u8 = SPI1->DR;			//clearing buffer

	SPI1->DR = reg_u8 << 8;
	while(!(SPI1->SR & SPI_SR_RXNE)){};	//waiting for rx
	uint16_t rec_u16 = SPI1->DR;		//recived data
	SPI1->CR1 &= ~SPI_CR1_SPE;			//disabling spi
	GPIOB->ODR |= GPIO_ODR_13;			//nss high

	return rec_u16;				//returning recieved value

}
