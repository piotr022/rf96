/*
 * spi.h
 *
 *  Created on: Mar 24, 2020
 *      Author: Piotr Lewandowski
 */

#ifndef SPI_H_
#define SPI_H_
#define DELAY_TIME 0UL


void spiInit();
uint8_t spiWrite(uint8_t, uint8_t);
uint16_t spiRead(uint8_t);

#endif /* SPI_H_ */
